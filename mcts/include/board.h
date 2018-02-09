//
// Created by Axiom on 2018/1/30.
//

#ifndef MCTS_BOARD_H
#define MCTS_BOARD_H

#include "common.h"

class Board {
private:
  unsigned height;
  unsigned width;
  unsigned n_in_row;

  unsigned moves;

  int players[2] = {1, 2};

  int dir[4][2] = {{0, 1}, {1, 0}, {1, 1}, {1, -1}};

  int current_player; // 0, 1
  int last_move;

  std::unordered_map<int, int> states; // key: move, value: player
  std::unordered_set<int> legal_positions;

public:
  Board(unsigned h, unsigned w, unsigned n) {
    assert(h >= n && w >= n);
    height = h;
    width = w;
    n_in_row = n;
    moves = h * w;
    current_player = 0;
    last_move = -1;
  }

  void init_board(int start_player = 0) {
    current_player = start_player;
    last_move = -1;
    states.clear();
    for (auto it = 0; it < moves; ++it) {
      legal_positions.insert(it);
    }
  }

  void init_board(const std::vector<std::vector<int>>& board, int start_player = 0) {
    current_player = start_player;
    last_move = -1;
    states.clear();
    for (auto it = 0; it < moves; ++it) {
      legal_positions.insert(it);
    }
    for (auto it = 0; it < height; ++it) {
      for (auto jt = 0; jt < width; ++jt) {
        int player = board[it][jt];
        if (player > 0) {
          int move = it * width + jt;
          states[move] = player;
          legal_positions.erase(move);
        }
      }
    }
  }

  void init_from_image(const std::vector<std::string>& image, int start_player = 0) {
    std::vector<std::vector<int>> board(height, std::vector<int>(width, 0));
    for (auto it = 0; it < height; ++it) {
      for (auto jt = 0; jt < width; ++jt) {
        if (image[it][jt] == '1') board[it][jt] = 1;
        else if (image[it][jt] == '2') board[it][jt] = 2;
        else board[it][jt] = 0;
      }
    }
    init_board(board, start_player);
  }

  auto get_positions() {
    return height * width;
  }

  auto get_current_player() {
    return players[current_player];
  }

  auto get_legal_positions() {
    return legal_positions;
  }

  void do_move(int move) {
    // std::cout << "w: " << move / width << ", h: " << move % width << std::endl;
    states[move] = players[current_player];
    legal_positions.erase(move);
    current_player = 1 - current_player;
    last_move = move;
  }

  auto get_current_state() {
    std::vector<real_t> res;
    std::vector<real_t> current(moves, 0.0);
    std::vector<real_t> opposite(moves, 0.0);
    for (auto it : states) {
      auto move = it.first;
      auto player = it.second;
      if (player == players[current_player]) current[move] = 1.0;
      else opposite[move] = 1.0;
    }
    res.insert(res.end(), current.begin(), current.end());
    res.insert(res.end(), opposite.begin(), opposite.end());
    std::vector<real_t> last(moves, 0.0);
    if (last_move >= 0) {
      last[last_move] = 1.0;
    }
    res.insert(res.end(), last.begin(), last.end());
    std::vector<real_t> winner(moves, 0.0);
    if (states.size() % 2 == 0) {
      std::fill(winner.begin(), winner.end(), 1.0);
    }
    res.insert(res.end(), winner.begin(), winner.end());
    return res;
  }

  void print(const std::vector<std::vector<int>>& board) {
    for (auto it = 0; it < height; ++it) {
      for (auto jt = 0; jt < width; ++jt) {
        std::cout << board[it][jt];
      }
      std::cout << std::endl;
    }
  }

  void print() {
    std::vector<std::vector<int>> board(height, std::vector<int>(width, 0));
    for (auto it : states) {
      auto h = it.first / width, w = it.first % width;
      board[h][w] = it.second;
    }
    print(board);
  }

  auto check(const std::vector<std::vector<int>>& board) {
    for (auto it = 0; it < height; ++it) {
      for (auto jt = 0; jt < width; ++jt) {
        auto player = board[it][jt];
        if (player != 0) {
          for (auto kt = 0; kt < 4; ++kt) {
            auto winner = player;
            for (auto nn = 0; nn < n_in_row; ++nn) {
              auto x = it + dir[kt][0] * nn;
              auto y = jt + dir[kt][1] * nn;
              if (x < 0 || x >= height || y < 0 || y >= width || board[x][y] != player) {
                winner = -1;
                break;
              }
            }
            if (winner > 0) {
              // std::cout << "winner: " << winner << ", it: " << it << ", jt: " << jt << ", dir: " << kt << std::endl;
              return winner;
            }
          }
        }
      }
    }
    return -1;
  }

  auto has_a_winner() {
    std::vector<std::vector<int>> board(height, std::vector<int>(width, 0));
    for (auto it : states) {
      auto h = it.first / width, w = it.first % width;
      board[h][w] = it.second;
    }
    auto winner = check(board);
    if (winner > 0) {
      return winner;
    }
    else {
      if (legal_positions.empty()) {
        return 0; // tie
      }
      else {
        return -1;
      }
    }
  }
};

#endif //MCTS_BOARD_H
