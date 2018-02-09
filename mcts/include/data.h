//
// Created by Axiom on 2018/2/4.
//

#ifndef MCTS_DATA_H
#define MCTS_DATA_H

#include "common.h"

class GameState {
public:
  tensor state;
  std::vector<real_t> probability;
  int current_player;
  double winner;

  GameState() {
    state.clear();
    probability.clear();
    current_player = 0;
    winner = 0.0;
  }
};

#endif //MCTS_DATA_H
