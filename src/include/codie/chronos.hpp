#ifndef PZEN_CHRONOS_HDR
#define PZEN_CHRONOS_HDR

#include <codie/common.hpp>

// Warning, may change
#include <chrono>

namespace codie {

using timestamp_t = uint64_t;   ///< Timestamp in µsec
using timediff_t  = int64_t;    ///< Time-difference in µsec

class Time {
public:

  inline static timestamp_t Now(void) {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
  }

  static void SleepFor( timediff_t );
};



} // ns pzen

#endif // PZEN_CHRONOS_HDR
