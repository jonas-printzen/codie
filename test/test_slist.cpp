#include "utest.hpp"

#include <codie/slist.hpp>
using namespace codie;

using RealList = slist<float>;

TCASE(SList,Basic) {
  RealList reals;

  TEXPECT_EQ( 0U, reals.size() );

  reals.push_back(42.f);
  TEXPECT_EQ( 1U, reals.size() );
  TEXPECT_EQ( 42.f, reals.front() );
  TEXPECT_EQ( 42.f, reals.back() );
 
  reals.push_front( 3.14f );
  TEXPECT_EQ( 2U, reals.size() );
  TEXPECT_EQ( 3.14f, reals.front() );
  TEXPECT_EQ( 42.f, reals.back() );

  float sum=0.f;
  for( auto &f : reals ) {
    sum += f;
  }
  TEXPECT_EQ( 45.14f, sum );

  reals.pop_back();
  TEXPECT_EQ( 1U, reals.size() );
  TEXPECT_EQ( 3.14f, reals.front() );
  TEXPECT_EQ( 3.14f, reals.back() );

  const RealList &rlist = reals;

  sum = 0.f;
  for( const float &f : rlist ) {
    sum += f;
  }
  TEXPECT_EQ( 3.14f, sum );


  auto pr = reals.pull_front();
  TEXPECT_EQ( 0U, reals.size() );
  TEXPECT_EQ( 3.14f, pr->value );
}

struct Vec { 
  double x,y; 

  bool operator == ( const Vec& ) const = default;
};

using VecList = slist<Vec>;

TCASE(SList,Moving) {
  VecList vecs;

  TEXPECT_EQ( 0U, vecs.size() );

  VecList::iptr_t pv;
  TEXPECT_EQ( false, bool(pv) );

  pv.reset( new VecList::item_t(0.f,0.f) );

  TEXPECT_EQ( true, bool(pv) );

  vecs.insert_front( move(pv)  );
  TEXPECT_EQ( false, bool(pv) );
  TEXPECT_EQ( 1U, vecs.size() );
 
  pv.reset( new VecList::item_t(1.f,2.f) );
  vecs.insert_front( move(pv)  );
  TEXPECT_EQ( false, bool(pv) );
  TEXPECT_EQ( 2U, vecs.size() );

  pv.reset( new VecList::item_t(1920.f,1080.f) );

  vecs.insert_after( vecs.begin(), move(pv) );
  TEXPECT_EQ( 3U, vecs.size() );
  TEXPECT_EQ( 1., vecs.front().x );
  TEXPECT_EQ( 0., vecs.back().x );
  vecs.erase_after( vecs.begin() );
  TEXPECT_EQ( 2U, vecs.size() );
  TEXPECT_EQ( 1., vecs.front().x );
  TEXPECT_EQ( 0., vecs.back().x );

  pv = vecs.pull_back();
  Vec tmp{0.f,0.f};
  TEXPECT_EQ( tmp, pv->value );
  TEXPECT_EQ( 1U, vecs.size() );
  
}