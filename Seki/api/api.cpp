#include <iostream>
#include <api/json.hpp>
#include <core/core.h>
#include <tuple>
#include <functional>
#include <api/detail.hpp>
#include <api/serialization.hpp>

namespace api {

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

core::Move find_optimal_move(GameInfo gi) {
  auto state = nlohmann::json::parse(gi.state).get<core::GameState>();
  auto solver = core::SekiSolver(state);
  return solver.find_optimal();
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
        auto winner = detail::match<core::GameState::Status, std::string, 3>(
          {
            core::GameState::Status::R_won,
            core::GameState::Status::C_won,
            core::GameState::Status::Draw,
          },
          {"R", "C", ""},
          state.get_status()
        );
        if (!winner) {
          throw std::runtime_error("Status is broken");
        }
        gi.winner = *winner;
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
  } else if (method == "findOptimalMove") {
    return detail::call_with_json(find_optimal_move, params);
  }
  return method;
}

}