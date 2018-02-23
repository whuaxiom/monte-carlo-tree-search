//
// Created by Axiom on 2018/1/31.
//

#ifndef MCTS_GAME_H
#define MCTS_GAME_H

#include "common.h"
#include "board.h"
#include "mcts_self_player.h"
#include "data.h"

#include "states.pb.h"

class Game {
public:
  Board* board = nullptr;
  Game(unsigned int height, unsigned int width, unsigned int n_in_row) {
    board = new Board(height, width, n_in_row);
  }

  auto start_self_play(MCTSSelfPlayer* player, double temperature) {
    board->init_board();
    /*
    std::vector<std::string> image = {
        "00000000",
        "00021200",
        "00211100",
        "00012200",
        "21111200",
        "00000000",
        "00020000",
        "00000000"
    };
    auto current_player = 1;
    board->init_from_image(image, current_player);
    */
    mcts::Samples data;
    while (true) {
      auto sensible_moves = board->get_legal_positions();
      if (sensible_moves.empty()) {
        break;
      }

      std::cout << "current_player: " << board->get_current_player() << " calculate move" << std::endl;
      board->print();

      auto tmp = player->get_action(board, temperature);
      auto move = tmp.first;
      auto probability = tmp.second;

      auto cur = data.add_samples();
      // store the data
      for (auto it : board->get_current_state()) {
        cur->add_state(it);
      }
      for (auto it : probability) {
        cur->add_probability(it);
      }
      cur->set_current_player(board->get_current_player());
      cur->set_winner(0.0);

      std::cout << "player: " << board->get_current_player() << ", move at: " << move << std::endl;

      board->do_move(move);
      board->print();
      auto winner = board->has_a_winner();

      std::cout << "winner: " << winner << std::endl;
      if (winner >= 0) {
        // winner from the perspective of the current player of each state
        for (auto it = 0; it < data.samples_size(); ++it) {
          auto mm = data.mutable_samples(it);
          if (mm->current_player() == winner) mm->set_winner(+1.0);
          else mm->set_winner(-1.0);
        }
        player->reset_player();
        break;
      }
      // break;
    }
    return data;
  }
};

#endif //MCTS_GAME_H
