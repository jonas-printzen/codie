#include "utest.hpp"

#include <codie/ranges.hpp>
using namespace codie;

TCASE(Range,Basic) {
  Range range(1U,2U);

  TEXPECT_EQ( 1U, *range.begin() );
  TEXPECT_EQ( 2U, *range.end() );

  size_t count=0U;

  // Can't have step 0!
  TEXPECT_THROW( Range(0U,1U,0U), RuntimeError );

  for( auto i : Range(0U,10U) ) {
    std::ignore = i;
    count++;
  }

  TEXPECT_EQ( 10U, count );

  count = 0; // Empty range!
  for( auto i : Range(0U,0U) ) {
    std::ignore = i;
    count++;
  }

  TEXPECT_EQ( 0U, count );
}

TCASE(Range,Signed) {
  Range<int> range(-1,2);

  TEXPECT_EQ( -1, *range.begin() );
  TEXPECT_EQ( 2, *range.end() );

  size_t count=0U;

  // Can't have step 0!
  TEXPECT_THROW( Range(0U,1U,0U), RuntimeError );

  for( auto i : Range(0,10) ) {
    std::ignore = i;
    count++;
  }
  TEXPECT_EQ( 10U, count );

  count = 0; // Empty range!
  for( auto i : Range<int16_t>(-1,1) ) {
    std::ignore = i;
    count++;
  }
  TEXPECT_EQ( 2U, count );

  count = 0; // Empty range!
  for( auto i : Range(-1,-1) ) {
    std::ignore = i;
    count++;
  }
  TEXPECT_EQ( 0U, count );
}

using U64Range2 = Range2<>;

TCASE(Range2,Basic) {
  U64Range2 range({0U,0U},{1U,1U});

  {
    auto [x,y] = *range.begin();
    
    TEXPECT_EQ( 0U, x );
    TEXPECT_EQ( 0U, y );
  }

  {
    auto [x,y] = *range.end();
    TEXPECT_EQ( 1U, x );
    TEXPECT_EQ( 1U, y );
  }

  auto it = range.begin();

  TEXPECT_NE( it, range.end() );

  ++it;
  TEXPECT_EQ( it, range.end() );
  ++it;
  TEXPECT_EQ( it, range.end() );

  size_t count = 0U;

  for( auto ab : U64Range2({0,0},{5,5}) ) {
    std::ignore = ab;
    ++count;
  }

  TEXPECT_EQ( 25, count );

  count = 0;
  for( auto [a,b] : U64Range2({0,0},{10,1}) ) {
    std::ignore = a;
    std::ignore = b;
    ++count;
  }
  TEXPECT_EQ( 10, count );

  // Can't have step 0!
  TEXPECT_THROW( U64Range2({0,0},{10,1},{2,0}), RuntimeError );
  TEXPECT_THROW( U64Range2({0,0},{10,1},{0,2}), RuntimeError );

  count = 0;
  for( auto ab : U64Range2({0,0},{10,1},{2,2}) ) {
    std::ignore = ab;
    ++count;
  }
 
  TEXPECT_EQ( 5, count );

  count = 0;  // Legal empty range
  for( auto ab : U64Range2({0,0},{0,0},{2,2}) ) {
    std::ignore = ab;
    ++count;
  }
  TEXPECT_EQ( 0, count );

  count = 0;  // Legal empty range!
  for( auto ab : U64Range2({0,0},{0,1},{2,2}) ) {
    std::ignore = ab;
    ++count;
  }
  TEXPECT_EQ( 0, count );

  count = 0;  // Legal empty range!
  for( auto ab : U64Range2({5,5},{6,5},{2,2}) ) {
    std::ignore = ab;
    ++count;
  }
  TEXPECT_EQ( 0, count );
}
