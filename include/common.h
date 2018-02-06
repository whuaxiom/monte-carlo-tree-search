//
// Created by Axiom on 2018/1/30.
//

#ifndef MCTS_COMMON_H
#define MCTS_COMMON_H

// some utility functions
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <regex>
// concurrency
#include <future>
#include <thread>
#include <mutex>
// smart pointers
#include <memory>
// stream
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <streambuf>
// containers
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <limits>
#include <iomanip>

#include <ctime>
#include <chrono>
#include <random>
#include <functional>
#include <tuple>

#include <cassert>
#include <array>

using Time = std::chrono::time_point<std::chrono::system_clock>;
using Clock = std::chrono::system_clock;

using std::string;
using std::vector;
using std::deque;

using real_t = double;
using tensor = std::vector<std::vector<real_t>>;

template <class T, size_t ROW, size_t COL>
using Matrix = std::array<std::array<T, COL>, ROW>;
// using Matrix = T[ROW][COL];

template <typename V>
inline string dbstr(const std::vector<V>& data, int m = 5) {
  std::stringstream ss;
  ss << "[" << data.size() << "]: ";
  if (data.size() < 2 * m) {
    for (int i = 0; i < data.size(); ++i) ss << data[i] << " ";
  } else {
    for (int i = 0; i < m; ++i) ss << data[i] << " ";
    ss << "... ";
    for (int i = (int)data.size() - m; i < data.size(); ++i) ss << data[i] << " ";
  }
  return ss.str();
}

template <typename V>
inline string fullstr(const std::vector<V>& data) {
  std::stringstream ss;
  ss << "[" << data.size() << "]: ";
  for (int i = 0; i < data.size(); ++i) ss << data[i] << " ";
  return ss.str();
}

// for debug use
template <typename V>
std::ostream& operator<<(std::ostream& os, const std::vector<V>& obj) {
  os << dbstr(&obj[0], (int)obj.size());
  return os;
}

inline double elapsed_seconds(Time end, Time start) {
  std::chrono::duration<double> elapsed_seconds = end - start;
  return elapsed_seconds.count();
}

// The clock() function returns the amount of CPU time used by your process since it started,
// not the absolute time according to a real-time clock.
inline double duration_seconds(std::clock_t end, std::clock_t start) {
  return (end - start) / (double)(CLOCKS_PER_SEC);
}

#endif //MCTS_COMMON_H
