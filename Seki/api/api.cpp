#include <iostream>
#include <api/json.hpp>
#include <api/detail.hpp>
#include <tuple>
#include <functional>
#include <core/core.h>

namespace nlohmann {
  template <>
  struct adl_serializer<core::GameState> {
    static void to_json(nlohmann::json &json, const core::GameState &state) {
      auto current_player = state.get_is_r() ? "R" : "C";
      auto field_width = state.get_field().get_shape()[1];
      auto field_height = state.get_field().get_shape()[0];

      std::vector<int> v;
      v.reserve(field_width * field_height);

      for (int y = 0; y < field_height; ++y) {
        for (int x = 0; x < field_width; ++x) {
          v.push_back(state.get_field().get(x, y));
        }
      }

      auto type = api::detail::match<core::SekiType, std::string, 2>(
        {core::SekiType::SEKI, core::SekiType::DSEKI},
        {"seki", "dseki"},
        state.get_type());
      
      if (!type) {
        throw std::runtime_error("Invalid game type");
      }

      auto passtype = api::detail::match<core::PassType, std::string, 4>(
        {
          core::PassType::R_PASS, core::PassType::C_PASS,
          core::PassType::ANY_PASS, core::PassType::NO_PASS,
        },
        {"R", "C", "RC", ""},
        state.get_passtype());

      if (!passtype) {
        throw std::runtime_error("Invalid pass type");
      }

      json = nlohmann::json({
        {"CurrentPlayer", current_player},
        {"Width", field_width},
        {"Height", field_height},
        {"FlattenedField", v},
        {"Type", *type},
        {"Pp", *passtype},
      });
    }

    static core::GameState from_json(const nlohmann::json &json) {
      auto current_player = json.at("CurrentPlayer").get<std::string>();
      bool is_r = current_player == "R";
      if (current_player != "R" && current_player != "C") {
        throw std::runtime_error("Invalid player specifided");
      }

      auto width = json.at("Width").get<int>();
      auto height = json.at("Height").get<int>();
      if (width < 1 || height < 1
        || static_cast<size_t>(width) > core::max_field_dim
        || static_cast<size_t>(height) > core::max_field_dim) {
        throw std::runtime_error("Invalid field size");
      }

      auto uwidth = static_cast<size_t>(width);
      auto uheight = static_cast<size_t>(height);

      auto flat_field_data = json.at("FlattenedField").get<std::vector<int>>();
      if (flat_field_data.size() != uwidth * uheight) {
        throw std::runtime_error("Field size mismatch");
      }

      std::vector<std::vector<int>> field_data(uheight, std::vector<int>(uwidth, 0));

      for(size_t y = 0; y < uheight; ++y) {
        for(size_t x = 0; x < uwidth; ++x) {
          auto value = flat_field_data[x + y * uwidth];
          if (value < 0) {
            throw std::runtime_error("Negative items are not allowed");
          }
          field_data[y][x] = value;
        }
      }

      auto type = api::detail::match<std::string, core::SekiType, 2>(
        {"seki", "dseki"},
        {core::SekiType::SEKI, core::SekiType::DSEKI},
        json.at("Type").get<std::string>());
      
      if (!type) {
        throw std::runtime_error("Invalid game type");
      }

      auto pass = api::detail::match<std::string, core::PassType, 4>(
        {"R", "C", "RC", ""},
        {
          core::PassType::R_PASS, core::PassType::C_PASS,
          core::PassType::ANY_PASS, core::PassType::NO_PASS,
        },
        json.at("Pp").get<std::string>());

      if (!pass) {
        throw std::runtime_error("Invalid pass type");
      }

      return core::GameState(
        core::Field(field_data),
        is_r,
        1,
        *type,
        *pass);
    }
  };

  template <>
  struct adl_serializer<core::Move> {
    static core::Move from_json(const nlohmann::json &json) {
      int x = json.at("X").get<int>();
      int y = json.at("Y").get<int>();
      auto result = core::Move(0, x, y);
      result.is_pass = json.at("IsPass").get<bool>();
      return result;
    }

    static void to_json(nlohmann::json &json, const core::Move &m) {
      json = nlohmann::json({
        {"X", m.x},
        {"Y", m.y},
        {"IsPass", m.is_pass},
      });
    }
  };

}

namespace api {

struct GameInfo {
  std::string id;
  std::vector<std::string> players_joined;
  std::string game_name;
  int event_clock;
  std::string state;
  bool is_ready;
  bool is_finished;
  std::string winner;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameInfo, id, players_joined, game_name,
      event_clock, state, is_ready, is_finished, winner)
};

bool is_start_state_valid(std::string enc_state) {
  auto state = nlohmann::json::parse(enc_state).get<core::GameState>();
  return !state.is_terminal();
}

GameInfo join_game(GameInfo info, std::string pid) {
  auto &pj = info.players_joined;
  if (std::find(pj.begin(), pj.end(), pid) != pj.end()) {
    throw std::runtime_error("Player is already in the game");
  }
  if (pid != "R" && pid != "C") {
    throw std::runtime_error("No such player");
  }

  // The second player is joining
  if (pj.size() == 1) {
    info.is_ready = true;
  }

  return info;
}

GameInfo add_event(GameInfo gi, std::string pid, std::string mv_enc) {
  if (gi.players_joined.size() != 2) {
    throw std::runtime_error("Game must be full");
  }
  auto mv = nlohmann::json::parse(mv_enc).get<core::Move>();
  auto state = nlohmann::json::parse(gi.state).get<core::GameState>();
  auto current_pid = state.get_is_r() ? "R" : "C";
  if (current_pid == pid) {
    auto moves = state.get_moves();
    auto it = std::find_if(moves.begin(), moves.end(),
      [&mv](const core::Move &other) {
        return other.x == mv.x && other.y == mv.y;
      });
    if (it != moves.end()) {
      state.apply_move(mv);
      gi.state = nlohmann::json(state).dump();
      if (state.is_terminal()) {
        gi.is_finished = true;
        switch (state.get_status())
        {
        case core::GameState::Status::R_won:
          gi.winner = "R";
          break;
        case core::GameState::Status::C_won:
          gi.winner = "C";
          break;
        case core::GameState::Status::Draw:
          gi.winner = "";
        break;
        default:
          throw std::runtime_error("Status is broken");
        }
        
      }
      return gi;
    } else {
      throw std::runtime_error("Invalid move");  
    }
  } else {
    throw std::runtime_error("Invalid pid");
  }
}

std::string call_function(std::string details) {
  nlohmann::json json = nlohmann::json::parse(details);
  auto method = json.at("method").get<std::string>();
  auto params = json.at("params");
  if (method == "isStartStateValid") {
    return detail::call_with_json(is_start_state_valid, params);
  } else if (method == "joinGame") {
    return detail::call_with_json(join_game, params);
  } else if (method == "addEvent") {
    return detail::call_with_json(add_event, params);
  }
  return method;
}

}