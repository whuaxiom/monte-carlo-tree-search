//
// Created by Axiom on 2018/1/30.
//

#ifndef MCTS_NODE_H
#define MCTS_NODE_H

#include "common.h"

class TreeNode {
private:
  TreeNode* parent;
  std::unordered_map<int, TreeNode*> children;
  double p;
  double n;
  double u;
  double w;
  double q;

public:
  TreeNode(TreeNode* prior_parent, double prior_p) {
    parent = prior_parent;
    children.clear();
    p = prior_p;
    n = 0;
    u = 0;
    w = 0;
    q = 0;
  }

  void set_parent(TreeNode* pre_parent) {
    parent = pre_parent;
  }

  auto get_value(double c_puct) {
    u = c_puct * p * sqrt(parent->n) / (1 + n);
    return q + u;
  }

  auto select(double c_puct) {
    double max = -std::numeric_limits<double>::infinity();
    std::pair<int, TreeNode*> ans;
    for (auto it : children) {
      double value = it.second->get_value(c_puct);
      if (max < value) {
        max = value;
        ans = it;
      }
    }
    return ans;
  }

  void expand(const std::unordered_map<int, real_t>& action_priors) {
    for (auto it : action_priors) {
      if (children.find(it.first) == children.end()) {
        children[it.first] = new TreeNode(this, it.second);
      }
    }
  }

  void expand(const std::vector<int>& actions, const std::vector<real_t>& action_priors) {
    for (auto it = 0; it < actions.size(); ++it) {
      if (children.find(actions[it]) == children.end()) {
        children[actions[it]] = new TreeNode(this, action_priors[it]);
      }
    }
  }

  void update(double leaf_value) {
    n += 1;
    w += leaf_value;
    q = w / n;
  }

  void backup(double leaf_value) {
    if (!is_root()) parent->backup(-leaf_value);
    update(leaf_value);
  }

  bool is_leaf() {
    return children.empty();
  }

  bool is_root() {
    return parent == nullptr;
  }

  auto get_children() {
    return children;
  }

  auto get_n() {
    return n;
  }

};

#endif //MCTS_NODE_H
