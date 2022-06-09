#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

namespace core {
struct Move {
  int Card;
};

struct GameState {
  std::vector<int> FirstPlayerSet;
  std::vector<int> SecondPlayerSet;
  std::string CurrentPlayer;
  int FirstScore;
  int SecondScore;
  std::optional<int> LastCard;

  bool is_min_player() const {
    return CurrentPlayer == "A";
  }

  bool is_terminal() const {
    return (FirstPlayerSet.empty() && SecondPlayerSet.empty());
  }

  float get_value() const {
    if (!is_terminal()) {
      throw std::runtime_error("value is not defined for non terminal");
    }
    if (FirstPlayerSet.empty() && SecondPlayerSet.empty()) {
        return SecondScore - FirstScore;
    }
    throw std::runtime_error("get_value is bugged");
  }

  std::vector<Move> get_moves() const {
    const std::vector<int> &cards = CurrentPlayer == "A" ? FirstPlayerSet : SecondPlayerSet;
    std::vector<Move> result;
    for (auto card : cards) {
      result.push_back({
        card
      });
    }
    return result;
  }

  void apply_move(Move m) {
    std::vector<int> &cards = CurrentPlayer == "A" ? FirstPlayerSet : SecondPlayerSet;
    auto it = std::find(cards.begin(), cards.end(), m.Card);
    if (it == cards.end()) {
      throw std::runtime_error("No such card");
    }
    auto swap_player = [&]() {
      if (!is_terminal()) {
        CurrentPlayer = CurrentPlayer == "A" ? "B" : "A";
      }
    };
    cards.erase(it);
    if (LastCard.has_value()) {
      bool does_first_win = false;
      if (m.Card <= *LastCard) {
        does_first_win = CurrentPlayer == "B";
        swap_player();
      } else {
        does_first_win = CurrentPlayer == "A";
      }
      if (does_first_win) {
        FirstScore += 1;
      } else {
        SecondScore += 1;
      }
      LastCard = std::nullopt;
    } else {
      LastCard = m.Card;
      swap_player();
    }
  }

};
}