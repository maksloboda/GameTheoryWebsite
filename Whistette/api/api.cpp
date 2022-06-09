#include <iostream>
#include <api/json.hpp>
#include <core/game.hpp>
#include <tuple>
#include <functional>
#include <api/detail.hpp>
#include <genericgamesolver/include/ggs.hpp>

namespace nlohmann {
  template <>
  struct adl_serializer<core::GameState> {
    static void to_json(nlohmann::json &json, const core::GameState &state) {
      json = nlohmann::json({
        {"FirstPlayerSet", state.FirstPlayerSet},
        {"SecondPlayerSet", state.SecondPlayerSet},
        {"CurrentPlayer", state.CurrentPlayer},
        {"FirstScore", state.FirstScore},
        {"SecondScore", state.SecondScore},
        {"LastCard", state.LastCard.has_value() ? nlohmann::json(*state.LastCard) : nlohmann::json(nullptr)}
      });
    }

    static core::GameState from_json(const nlohmann::json &json) {
      core::GameState result;
      result.FirstPlayerSet = json.at("FirstPlayerSet").get<std::vector<int>>();
      result.SecondPlayerSet = json.at("SecondPlayerSet").get<std::vector<int>>();
      result.CurrentPlayer = json.at("CurrentPlayer").get<std::string>();
      result.FirstScore = json.at("FirstScore").get<int>();
      result.SecondScore = json.at("SecondScore").get<int>();
      auto lc_json = json.at("LastCard");
      result.LastCard = lc_json.is_null() ? std::nullopt : std::make_optional(lc_json.get<int>());
      return result;
    };
  };
}

namespace core {
  // NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
  //   GameState,
  //   FirstPlayerSet,
  //   SecondPlayerSet,
  //   CurrentPlayer,
  //   Type,
  //   LastCard
  // );

  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    Move,
    Card
  );
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
  if (state.FirstPlayerSet.size() != state.SecondPlayerSet.size()) {
    throw std::runtime_error("Size mismatch");
  }
  return !state.is_terminal();
}

GameInfo join_game(GameInfo info, std::string pid) {
  auto &pj = info.players_joined;
  if (std::find(pj.begin(), pj.end(), pid) != pj.end()) {
    throw std::runtime_error("Player is already in the game");
  }
  if (pid != "A" && pid != "B") {
    throw std::runtime_error("No such player");
  }

  // The second player is joining
  if (pj.size() == 1) {
    info.is_ready = true;
  }

  return info;
}

core::Move find_optimal_move(GameInfo gi) {
  auto state = nlohmann::json::parse(gi.state).get<core::GameState>();
  auto move = ggs::OptimalMoveSolver<core::GameState, core::Move>::find_optimal_move(state);
  return move;
}

GameInfo add_event(GameInfo gi, std::string pid, std::string mv_enc) {
  if (gi.players_joined.size() != 2) {
    throw std::runtime_error("Game must be full");
  }
  auto mv = nlohmann::json::parse(mv_enc).get<core::Move>();
  auto state = nlohmann::json::parse(gi.state).get<core::GameState>();
  auto current_pid = state.CurrentPlayer;
  if (current_pid == pid) {
    auto moves = state.get_moves();
    auto it = std::find_if(moves.begin(), moves.end(),
      [&mv](const core::Move &other) {
        return other.Card == mv.Card;
      });
    if (it != moves.end()) {
      state.apply_move(mv);
      gi.state = nlohmann::json(state).dump();
      if (state.is_terminal()) {
        gi.is_finished = true;
        float value = state.get_value();
        if (value < 0) {
          gi.winner = "A";
        } else if (value > 0) {
          gi.winner = "B";
        } else {
          gi.winner = "";
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

GameInfo advance(GameInfo gi) {
  // Player loses immidiately
  auto state = nlohmann::json::parse(gi.state).get<core::GameState>();
  gi.is_finished = true;
  gi.winner = state.CurrentPlayer == "A" ? "B" : "A";
  return gi;
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
  } else if (method == "findOptimalMove") {
    return detail::call_with_json(find_optimal_move, params);
  }  else if (method == "advance") {
    return detail::call_with_json(advance, params);
  }
  return method;
}

}