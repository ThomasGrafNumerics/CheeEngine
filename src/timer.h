#pragma once
#include <chrono>
#include <climits>

class Timer
{
  long long thinking_time_ms = 0;
  std::chrono::high_resolution_clock::time_point start_point;

  // use 1/20 of the remaining time
  static constexpr long long thinking_time_ratio = 20;

public:
  void StartTimer (long long remaining_time_ms, long long new_increment_time_ms);
  bool IsTimeOut ();

  static constexpr long long default_thinking_time_ms = INT_MAX;
  static constexpr long long default_increment_time_ms = 0;
};
