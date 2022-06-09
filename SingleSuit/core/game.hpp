#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>

namespace core {
struct Move {
  int Card;
  bool DoTake;
};

struct GameState {
  std::vector<int> FirstPlayerSet;
  std::vector<int> SecondPlayerSet;
  std::string CurrentPlayer;
  std::string Type;
  std::optional<int> LastCard;
  int depth{1};

  bool is_min_player() const {
    return CurrentPlayer == "A";
  }

  bool is_terminal() const {
    return (FirstPlayerSet.empty() || SecondPlayerSet.empty()) &&
        !LastCard.has_value();
  }

  float get_value() const {
    if (!is_terminal()) {
      throw std::runtime_error("value is not defined for non terminal");
    }
    if (FirstPlayerSet.empty() && SecondPlayerSet.empty()) {
      if (Type == "d-singlesuit") {
        return 0;
      } else {
        float sign = CurrentPlayer == "A" ? 1 : -1;
        return sign / depth;
      }
    } else if (FirstPlayerSet.empty()) {
      return -1.0f / depth;
    } else if (SecondPlayerSet.empty()) {
      return 1.0f / depth;
    }
    throw std::runtime_error("get_value is bugged");
  }

  std::vector<Move> get_moves() const {
    const std::vector<int> &cards = CurrentPlayer == "A" ? FirstPlayerSet : SecondPlayerSet;
    std::vector<Move> result;
    if (LastCard.has_value()) {
      int last_card_val = *LastCard;
      result.push_back({
        0,
        true
      });
      for (auto card : cards) {
        if (card > last_card_val) {
          result.push_back({
            card,
            false
          });
        }
      }
    } else {
      for (auto card : cards) {
        result.push_back({
          card,
          false
        });
      }
    }
    return result;
  }

  void apply_move(Move m) {
    ++depth;
    std::vector<int> &cards = CurrentPlayer == "A" ? FirstPlayerSet : SecondPlayerSet;
    if (m.DoTake && !LastCard.has_value()) {
      throw std::runtime_error("Cant take without LastCard");
    }
    if (m.DoTake && LastCard.has_value()) {
      cards.push_back(*LastCard);
      LastCard = std::nullopt;
    } else {
      auto it = std::find(cards.begin(), cards.end(), m.Card);
      if (it == cards.end()) {
        throw std::runtime_error("No such card");
      }
      cards.erase(it);
      if (LastCard.has_value()) {
        if (m.Card <= *LastCard) {
          throw std::runtime_error("Card is smaller");
        }
        LastCard = std::nullopt;
      } else {
        LastCard = m.Card;
      }
    }
    
    CurrentPlayer = CurrentPlayer == "A" ? "B" : "A";
  }

};
}