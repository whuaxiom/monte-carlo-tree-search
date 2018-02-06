//
// Created by Axiom on 2018/1/31.
//

#ifndef MCTS_GAME_H
#define MCTS_GAME_H

#include "common.h"
#include "board.h"
#include "mcts_self_player.h"
#include "data.h"

class Game {
public:
  Board* board = nullptr;
  Game(unsigned int height, unsigned int width, unsigned int n_in_row) {
    board = new Board(height, width, n_in_row);
  }

  auto start_self_play(MCTSSelfPlayer* player, double temperature) {
    board->init_board();
    /*
    std::vector<std::vector<int>> state = {
        {1, 2, 1, 2, 1, 0}, {0, 1, 0, 2, 2, 1}, {0, 2, 0, 1, 1, 2}, {1, 1, 2, 0, 0, 2}, {2, 2, 0, 0, 1, 1}, {2, 2, 2, 1, 1, 1}
    };
    auto current_player = 1;
    board->init_board(state, current_player);
    */
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
    auto data = new GameStates();
    while (true) {
      auto sensible_moves = board->get_legal_positions();
      if (sensible_moves.empty()) {
        break;
      }

      std::cout << "current_player: " << board->get_current_player() << " calculate move" << std::endl;
      board->print();
      auto tmp = player->get_action(board, temperature);
      auto move = tmp.first;
      auto move_probability = tmp.second;

      // store the data
      data->length += 1;
      data->states.push_back(board->get_current_state());
      data->probability.push_back(move_probability);
      data->current_players.push_back(board->get_current_player());

      std::cout << "player: " << board->get_current_player() << ", move at: " << move << std::endl;
      board->do_move(move);
      board->print();
      auto winner = board->has_a_winner();
      std::cout << "winner: " << winner << std::endl;
      if (winner >= 0) {
        // winner from the perspective of the current player of each state
        data->winners.clear();
        for (auto it : data->current_players) {
          if (it == winner) data->winners.push_back(+1.0);
          else data->winners.push_back(-1.0);
        }
        player->reset_player();
        break;
      }
      break;
    }
    return data;
  }
};

#endif //MCTS_GAME_H
