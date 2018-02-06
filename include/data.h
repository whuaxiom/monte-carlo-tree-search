//
// Created by Axiom on 2018/2/4.
//

#ifndef MCTS_DATA_H
#define MCTS_DATA_H

#include "common.h"

class GameStates {
public:
  int length;
  std::vector<tensor> states;
  std::vector<std::vector<real_t>> probability;
  std::vector<int> current_players;
  std::vector<real_t> winners;

  GameStates() {
    length = 0;
    states.clear();
    probability.clear();
    current_players.clear();
    winners.clear();
  }
};

#endif //MCTS_DATA_H
