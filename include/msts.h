//
// Created by Axiom on 2018/1/30.
//

#ifndef MCTS_MSTS_H
#define MCTS_MSTS_H

#include "common.h"
#include "node.h"
#include "board.h"

class MCTS {
private:
  TreeNode* root;
  double c_puct;
  int n_simulation;

  double select_time = 0;
  double evaluate_time = 0;
  double check_time = 0;
  double expand_time = 0;
  double update_time = 0;
  double rollout_time = 0;

  double copy_time = 0;
  double simulation_time = 0;

public:
  MCTS(double pre_c_puct = 5.0, int pre_n_simulation = 10000) {
    root = new TreeNode(nullptr, 1.0);
    c_puct = pre_c_puct;
    n_simulation = pre_n_simulation;
  }

  void print_time() {
    std::cout << "copy: " << copy_time << ", simulation: " << simulation_time << std::endl;
    copy_time = 0;
    simulation_time = 0;

    std::cout << "select: " << select_time << ", evaluate: " << evaluate_time << ", check: " << check_time
              << ", expand: " << expand_time << ", update: " << update_time
              << ", rollout_time: " << rollout_time << std::endl;
    select_time = 0;
    evaluate_time = 0;
    check_time = 0;
    expand_time = 0;
    update_time = 0;
    rollout_time = 0;
  }

  auto policy_value_function_bak(Board& state) {
    std::unordered_map<int, real_t> action_probability;
    auto pro = 1.0 / state.get_legal_positions().size();
    for (auto it : state.get_legal_positions()) {
      // action_probability[it] = 1.0 / state.get_legal_positions().size(); // very very slow
      action_probability[it] = pro;
    }
    return std::make_pair(action_probability, 0.0);
  }

  auto policy_value_function(Board& state) {
    std::vector<int> actions;
    std::vector<real_t> action_probability;
    auto pro = 1.0 / state.get_legal_positions().size();
    for (auto it : state.get_legal_positions()) {
      actions.push_back(it);
      action_probability.push_back(pro);
    }
    return std::make_tuple(actions, action_probability, 0.0);
  }

  auto rollout_policy_function(Board& state) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::vector<int> actions;
    std::vector<real_t> action_probability;
    for (auto it : state.get_legal_positions()) {
      actions.push_back(it);
      action_probability.push_back(dis(gen));
    }
    return std::make_pair(actions, action_probability);
  }

  auto select_max_move(std::vector<int>& actions, std::vector<real_t>& action_probability) {
    auto move = 0;
    for (auto it = 1; it < actions.size(); ++it) {
      if (action_probability[move] < action_probability[it]) {
        move = it;
      }
    }
    return actions[move];
  }

  auto evaluate_rollout(Board& state) {
    auto player = state.get_current_player();
    auto winner = 0;
    auto limits = 1000;
    while (limits--) {
      winner = state.has_a_winner();
      if (winner >= 0) {
        break;
      }
      auto res = rollout_policy_function(state);
      auto move = select_max_move(res.first, res.second);
      state.do_move(move);
    }
    if (limits < 0) {
      std::cout << "rollout reached limit" << std::endl;
    }
    if (winner >= 0) {
      if (winner == player) return +1.0;
      else return -1.0;
    }
    else {
      return 0.0;
    }
  }

  void simulation(Board& state) {
    auto t0 = Clock::now();
    auto node = root;
    while (!node->is_leaf()) {
      auto ans = node->select(c_puct);
      state.do_move(ans.first);
      node = ans.second;
    }
    auto t1 = Clock::now();
    select_time += elapsed_seconds(t1, t0);

    auto res = policy_value_function(state);
    auto t2 = Clock::now();
    evaluate_time += elapsed_seconds(t2, t1);

    /*
    auto action_probability = res.first;
    auto value = res.second;
    */

    auto actions = std::get<0>(res);
    auto action_probability = std::get<1>(res);
    auto value = std::get<2>(res);

    auto winner = state.has_a_winner(); // -1, 0, 1, 2
    auto t3 = Clock::now();
    check_time += elapsed_seconds(t3, t2);

    if (winner >= 0) { // end
      if (winner == 0) value = 0.0;
      else if (winner == state.get_current_player()) value = +1.0;
      else value = -1.0;
    }
    else {
      auto t4 = Clock::now();
      // node->expand(action_probability);
      node->expand(actions, action_probability);
      auto t5 = Clock::now();
      expand_time += elapsed_seconds(t5, t4);
      value = evaluate_rollout(state);
      auto t6 = Clock::now();
      rollout_time += elapsed_seconds(t6, t5);
    }

    auto t7 = Clock::now();
    node->backup(-value);
    auto t8 = Clock::now();
    update_time += elapsed_seconds(t8, t7);
  }

  auto trans(std::vector<double>& action_visits, double temperature) {
    double sum = 0.0;
    for (double &action_visit : action_visits) {
      double pro = pow(action_visit, temperature);
      action_visit = pro;
      sum += pro;
    }
    for (double &action_visit : action_visits) {
      action_visit /= sum;
    }
    return action_visits;
  }

  auto get_move_probability(Board* state, double temperature) {
    for (auto it = 0; it < n_simulation; ++it) {
      auto t0 = Clock::now();
      Board tmp = *(state);
      auto t1 = Clock::now();
      copy_time += elapsed_seconds(t1, t0);
      simulation(tmp);
      auto t2 = Clock::now();
      simulation_time += elapsed_seconds(t2, t1);
    }
    std::vector<int> actions;
    std::vector<double> action_visits;
    for (auto it : root->get_children()) {
      actions.push_back(it.first);
      action_visits.push_back(it.second->get_n());
    }
    std::cout << fullstr(actions) << std::endl;
    std::cout << fullstr(action_visits) << std::endl;
    std::vector<double> action_probability = trans(action_visits, temperature);
    std::cout << fullstr(action_probability) << std::endl;
    return std::make_pair(actions, action_probability);
  }

  void update_with_move(int last_move) {
    auto children = root->get_children();
    if (children.find(last_move) != children.end()) {
      root = children[last_move];
      root->set_parent(nullptr);
    }
  }

  void reset() {
    root = new TreeNode(nullptr, 1.0);
  }
};


#endif //MCTS_MSTS_H
