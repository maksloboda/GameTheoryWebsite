#include <iostream>
#include <api/json.hpp>
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

      json = nlohmann::json({
        {"CurrentPlayer", current_player},
        {"Width", field_width},
        {"Height", field_height},
        {"FlattenedField", v},
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
          field_data[y][x] = flat_field_data[x + y * uwidth];
        }
      }
      return core::GameState(
        core::Field(field_data),
        is_r,
        1,
        core::PassType::NO_PASS);
    }
  };

  template <>
  struct adl_serializer<core::Move> {
    static core::Move from_json(const nlohmann::json &json) {
      int x = json.at("X").get<int>();
      int y = json.at("Y").get<int>();
      return core::Move(0, x, y);
    }

    static void to_json(nlohmann::json &json, const core::Move &m) {
      json = nlohmann::json({
        {"X", m.x},
        {"Y", m.y},
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
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameInfo, id, players_joined, game_name, event_clock, state)
};

template<typename T, size_t idx, size_t total>
std::tuple<> convert_all_json(T &tuple, nlohmann::json &src_array) {
  static_assert(idx == total);
  return std::make_tuple();
}

template<typename T, size_t idx, size_t total, typename Arg, typename... Args>
std::tuple<Arg, Args...> convert_all_json(T &tuple, nlohmann::json &src_array) {

  Arg target = src_array[idx].get<Arg>();

  return std::tuple_cat(std::make_tuple(std::move(target)),
    convert_all_json<T, idx + 1, total, Args...>(tuple, src_array));

}

template<typename R, typename... Args>
std::string call_with_json(R (*f)(Args...), nlohmann::json &from) {
  constexpr size_t arg_count = sizeof...(Args);
  if (!from.is_array()) {
    throw std::runtime_error("Malformed call");
  }
  if (from.size() != arg_count) {
    std::stringstream ss;
    ss << "Missing arguments, expected " << arg_count << " got " << from.size();
    throw std::runtime_error(ss.str());
  }

  std::tuple<Args...> decoded_args = convert_all_json
    <decltype(decoded_args), 0, arg_count, Args...>(decoded_args, from);

  R result = std::apply(f, decoded_args);

  return nlohmann::json(result).dump();
}

bool is_start_state_valid(core::GameState state) {
  return !state.is_terminal();
}

std::string join_game(GameInfo info, std::string pid) {
  auto &pj = info.players_joined;
  if (std::find(pj.begin(), pj.end(), pid) != pj.end()) {
    throw std::runtime_error("Player is already in the game");
  }
  if (pid != "R" && pid != "C") {
    throw std::runtime_error("No such player");
  }
  return pid;
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
      gi.event_clock += 1;
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
    return call_with_json(is_start_state_valid, params);
  } else if (method == "joinGame") {
    return call_with_json(join_game, params);
  } else if (method == "addEvent") {
    return call_with_json(add_event, params);
  }
  return method;
}

}