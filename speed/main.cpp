
#include <codie/common.hpp>
#include <codie/chronos.hpp>
#include <codie/item.hpp>
#include <codie/rand.hpp>
using namespace codie;

#include <iostream>
using namespace std;


struct Size2D {
  unsigned w, h;
};

struct Rect2D {
  unsigned x,y,w,h;
};


using ASize = AItem<UID("Size2D"), Size2D>;
using ARect = AItem<UID("Rect2D"), Rect2D>;
using AReal = AItem<UID("float"), float>;
using AInt = AItem<UID("int"), int>;
using ASetup = AItem<UID("Setup")>;
using ATeardown = AItem<UID("Teardwon")>;
using ATimeout = AItem<UID("timeout")>;

ASize s_size=Size2D{1,2};       uint64_t s_size_count=0;
ARect s_rect=Rect2D{0,0,10,10}; uint64_t s_rect_count=0;
AReal s_real=3.14;              uint64_t s_real_count=0;
AInt  s_int = 42;               uint64_t s_int_count=0;
ASetup s_setup;                 uint64_t s_setup_count=0;
ATeardown s_teardown;           uint64_t s_teardown_count=0;
ATimeout s_timeout;             uint64_t s_timeout_count=0;

static void ItemDispatch() {
  cout << "==== Item-dispatch ====\n"
       << "  start..."; cout.flush();

  Item* items[] {
    &s_size, &s_rect, &s_real, &s_int, &s_setup, &s_teardown, &s_timeout
  };

  const uint64_t n_micros = 500000;

  timestamp_t t0 = Time::Now();
  timestamp_t timeout = t0 + n_micros; // ca: 1s
  uint64_t i=0;
  while( Time::Now() < timeout ) {
    Item &item = *items[i%asizeof(items)];
    switch( item.uid ) {
    case ASize::ID: s_size_count++; break;
    case ARect::ID: s_rect_count++; break;
    case AReal::ID: s_real_count++; break;
    case AInt::ID: s_int_count++; break;
    case ASetup::ID: s_setup_count++; break;
    case ATeardown::ID: s_teardown_count++; break;
    case ATimeout::ID: s_timeout_count++; break;
    }
  }
  timestamp_t t1 = Time::Now();
  uint64_t sum = s_size_count + s_rect_count + s_real_count + s_int_count + s_setup_count + s_teardown_count + s_timeout_count;

  cout << "done!\n"
       << "  Dispatched " << (double(sum)/double(t1-t0)) << " events per µs!" << endl;
}



int main( int, char** ) {
  ItemDispatch();
  return 0;
}