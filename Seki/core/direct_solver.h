#ifndef DIRECT_SOLVER_H
#define DIRECT_SOLVER_H

#include <stdlib.h>
#include <vector>
#include <array>
#include <iostream>

namespace core {

// Maximum width and height of the field
constexpr size_t max_field_dim = 6;

// Class that contains all the data about the Move
// can be compared by value
class Move {
public:
  float value;
  int x, y;
  bool is_pass;

  Move(float value, int x, int y);

  // Compares values
  bool operator>(const Move &other) const;
  bool operator>=(const Move &other) const;
  bool operator<(const Move &other) const;
  bool operator<=(const Move &other) const;

};

// Contains state of the field and utility functions
class Field {
private:
  std::array<int, 2> shape;
  // [row][col]
  std::array<std::array<int, max_field_dim>, max_field_dim> data;
  std::array<int, max_field_dim> row_sum, col_sum;
public:
  Field(const std::vector<std::vector<int>> &new_data);

  void add(int x, int y, int v);

  int get(int x, int y) const;

  std::array<int, 2> get_shape() const;

  int get_min_row() const;
  int get_min_col() const;

  bool has_zero_row() const;
  bool has_zero_col() const;

  const std::array<int, max_field_dim> &get_row_sum() const;
  const std::array<int, max_field_dim> &get_col_sum() const;

  friend std::ostream &operator<< (std::ostream &s, const Field &f);

};

enum PassType {
  NO_PASS = 0,
  R_PASS = 1,
  C_PASS = 2,
  ANY_PASS = 3,
};

enum SekiType {
  SEKI = 0,
  DSEKI,
};

// Stores everything needed for the current state of the game
class GameState {
private:
  Field field;
  bool is_r;
  int depth;
  int pass_count;
  const SekiType type;
  const PassType pass_policy;
public:

  GameState(Field field, bool is_r, int depth, SekiType p_type,
      PassType p_pass_policy);

  const Field &get_field() const;
  bool get_is_r() const;
  int get_depth() const;
  int get_pass_count() const;

  void apply_move(const Move &m);

  float get_value() const;

  bool is_terminal() const;

  enum Status {
    Ongoing,
    R_won,
    C_won,
    Draw,
  };

  Status get_status() const;

  SekiType get_type() const { return type; }
  PassType get_passtype() const { return pass_policy; }

  std::vector<Move> get_moves() const;
};

// Class that solves the game
class SekiSolver {
private:
  GameState state;
public:

  // Amount of states that were looked at on the last call of find optimal
  int unrolled;

  SekiSolver(GameState p_state);

  void apply_move(Move m);

  Move _find_optimal_impl(const GameState &state, Move alpha, Move beta);

  Move find_optimal();

  const GameState &get_state() const;

};

}

#endif // DIRECT_SOLVER_H
