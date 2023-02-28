
#include <codie/chronos.hpp>
#include <thread> // Argh...

namespace codie {

void Time::SleepFor( timediff_t duration ) {
  // usleep(duration);
  std::this_thread::sleep_for (std::chrono::microseconds(duration));
}


} // namespace codie
