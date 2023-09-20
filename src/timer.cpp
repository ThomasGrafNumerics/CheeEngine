#include "timer.h"
#include <algorithm>

using namespace std;
using namespace std::chrono;

void
Timer::StartTimer (long long remaining_time_ms, long long increment_time_ms)
{
  thinking_time_ms = std::max (remaining_time_ms / thinking_time_ratio, increment_time_ms);
  start_point = high_resolution_clock::now ();
}
bool
Timer::IsTimeOut ()
{
  long long passed_time_ms = duration_cast<milliseconds> (high_resolution_clock::now () - start_point).count ();

  return passed_time_ms > thinking_time_ms;
}
