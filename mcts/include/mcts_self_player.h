//
// Created by Axiom on 2018/1/30.
//

#ifndef MCTS_MCTS_SELF_PLAY_H
#define MCTS_MCTS_SELF_PLAY_H

#include "common.h"
#include "msts.h"

class MCTSSelfPlayer {
public:
  int player;
  bool is_self_play;

  MCTS *mcts = nullptr;

  MCTSSelfPlayer(double c_puct, int n_simulation, bool self_play = false) {
    player = 0;
    mcts = new MCTS(c_puct, n_simulation);
    is_self_play = self_play;
  }

  void set_player(int p) {
    player = p;
  }

  void reset_player() {
    mcts->reset();
  }

  auto select_move(const std::vector<int>& actions, const std::vector<real_t>& action_probability) {
    assert(!actions.empty());
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double pro = dis(gen);
    double cumulativeProbability = 0.0;
    for (auto it = 0; it < action_probability.size(); ++it) {
      cumulativeProbability += action_probability[it];
      if (cumulativeProbability >= pro) {
        return actions[it];
      }
    }
    return -1;
  }

  auto select_max_move(const std::vector<int>& actions, const std::vector<real_t>& action_probability) {
    assert(!actions.empty());
    auto move = 0;
    for (auto it = 1; it < actions.size(); ++it) {
      if (action_probability[move] < action_probability[it]) {
        move = it;
      }
    }
    return actions[move];
  }

  auto get_action(Board *board, double temperature) {
    auto ans = mcts->get_move_probability(board, temperature);
    auto actions = ans.first;
    auto action_probability = ans.second;
    std::vector<real_t> move_probability(board->get_positions(), 0.0);
    for (auto it = 0; it < actions.size(); ++it) {
      move_probability[actions[it]] = action_probability[it];
    }
    // auto move = select_move(actions, action_probability);
    auto move = select_max_move(actions, action_probability);
    if (is_self_play) {
      mcts->update_with_move(move);
    } else {
      mcts->reset();
    }
    return std::make_pair(move, move_probability);
  }
};


#endif //MCTS_MCTS_SELF_PLAY_H
