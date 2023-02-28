#include "utest.hpp"
#include <codie/rand.hpp>
using namespace codie;

TCASE(Rand,Uniform) {
  int ints[10]= {0};

  {
    mt19937_32 rgen(42);
    uniform_int_dist<int> udist(-5,5);
    for( size_t i=0; i<asizeof(ints); ++i ) {
      ints[i] = udist(rgen);
    }
  }

  for( size_t i=0; i<asizeof(ints); ++i ) {
    TEXPECT( (ints[i] >= -5) && (ints[i] <= 5) );
  }

  float floats[12] = {0.f};
  {
    mt19937_32 rgen(17);
    uniform_real_dist<float> udist(-1.f,1.f);
    for( size_t i=0; i<asizeof(floats); ++i ) {
      floats[i] = udist(rgen);
    }
  }

  for( size_t i=0; i<asizeof(floats); ++i ) {
    TEXPECT( (floats[i] >= -1.f) && (floats[i] <= 1.f) );
  }
}

TCASE(Rand,Norm) {
  float floats[12] = {0.f};
  {
    minstd_rand rgen(17);
    normal_dist<float> ndist(0.f,1.f);
    for( size_t i=0; i<asizeof(floats); ++i ) {
      floats[i] = ndist(rgen);
    }
  }
  // We rely on given seed ...
  for( size_t i=0; i<asizeof(floats); ++i ) {
    TEXPECT( (floats[i] >= -3.f) && (floats[i] <= 3.f) );
  }
}
