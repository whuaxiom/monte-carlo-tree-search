#include <iostream>
#include <tuple>
#include <vector>
#include <random>

#include "../include/board.h"
#include "../include/data.h"
#include "../include/mcts_self_player.h"
#include "../include/game.h"

std::tuple<bool, int> foo() {
  return std::make_tuple(true, 1);
}

std::tuple<int, int> foo_tuple() {
  return std::make_tuple(1, -1); // Always works
}

struct A { int a; A(int a_) : a(a_) {}; };

auto func() { return std::make_tuple(A(1), 2, 3); }

int test_tuple() {
  auto [a, b, c] = func();
  std::cout << a.a << ", " << b << ", " << c << std::endl;
  return 0;
}

void test() {
  unsigned height = 8;
  unsigned width = 8;
  unsigned n_in_row = 5;
  std::vector<std::vector<int>> data(height, std::vector<int>(width, 0));
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 2);
  auto *board = new Board(height, width, n_in_row);

  for (int x = 0; x < 3; ++x) {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        data[i][j] = distribution(generator);
      }
    }
    board->print(data);
    auto winner = board->check(data);
    std::cout << winner << std::endl;
  }
}

int play() {
  auto game = new Game(8, 8, 5);

  auto c_puct = 5.0;
  auto n_simulation = 3000;

  auto t0 = Clock::now();
  auto player = new MCTSSelfPlayer(c_puct, n_simulation, true);

  auto data = game->start_self_play(player, 1.0);
  auto t1 = Clock::now();
  auto run_time = elapsed_seconds(t1, t0);
  std::cout << "data length: " << data.samples_size() << std::endl;
  std::cout << "run_time: " << run_time << std::endl;
  player->mcts->print_time();

  std::string output_path = "./data/samples";
  data.SerializeToString(&output_path);
  std::cout << "job done" << std::endl;

  return 0;
}

int main() {
  // test();
  play();
  return 0;
}
