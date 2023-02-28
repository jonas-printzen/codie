#include "utest.hpp"

#include <codie/chronos.hpp>

#include <unistd.h>

#include <iostream>

using namespace codie;

static unsigned SLEEP_GRACE = 200U;

TCASE(Chronos,Assumptions) {
  // Precision of timestamps from std::chrono
  auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
  auto ts = std::chrono::duration_cast<std::chrono::microseconds>(now).count();
  TEXPECT_EQ( sizeof(uint64_t), sizeof(ts) );
  // Sleeping for more than
  TEXPECT_EQ( sizeof(ts), sizeof(timestamp_t) );
  TEXPECT_EQ( sizeof(ts), sizeof(timediff_t) );
  // Check with known usleep
  auto t0 = Time::Now();
  ::usleep(455U);
  auto t1 = Time::Now();

  auto tdiff = t1-t0;

  // Unavoidable that sleep starts a few microseconds late...
  TEXPECT( tdiff >= 455U );
  TEXPECT( tdiff <= (460U + SLEEP_GRACE) );
}

TCASE(Chronos,Basic) {
  const timediff_t sleeptime = 1001;

  auto t0 = Time::Now();
  Time::SleepFor(sleeptime);   // Best effort => minimal 1ms
  auto t1 = Time::Now();

  TEXPECT_NE(t0,t1);

  auto tdiff = t1-t0;
  TEXPECT_EQ(true, tdiff >= sleeptime );
}
