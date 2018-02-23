#include <iostream>
#include <vector>
#include <random>

#include "../include/board.h"
#include "../include/data.h"
#include "../include/mcts_self_player.h"
#include "../include/game.h"

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

  std::string output_path = "../data/samples.pb";

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Write the new samples back to disk
  std::fstream output(output_path, std::ios::out | std::ios::trunc | std::ios::binary);
  if (!output) {
    std::cout << output_path << ": File not found. Creating a new file" << std::endl;
  }
  else if (!data.SerializeToOstream(&output)) {
    std::cerr << "Failed to write data" << std::endl;
    return -1;
  }
  output.close(); // must close

  mcts::Samples tmp;
  // Read the existing samples
  std::fstream input(output_path, std::ios::in | std::ios::binary);
  if (!input) {
    std::cout << output_path << ": File not found. Creating a new file" << std::endl;
  } else if (!tmp.ParseFromIstream(&input)) {
    std::cerr << "Failed to parse samples" << std::endl;
    return -1;
  }
  input.close(); // must close
  std::cout << "tmp data length: " << tmp.samples_size() << std::endl;

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}

int main() {
  // test();
  play();
  return 0;
}
