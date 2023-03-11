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

  vecs.insert_front( std::move(pv)  );
  TEXPECT_EQ( false, bool(pv) );
  TEXPECT_EQ( 1U, vecs.size() );
 
  pv.reset( new VecList::item_t(1.f,2.f) );
  vecs.insert_front( std::move(pv)  );
  TEXPECT_EQ( false, bool(pv) );
  TEXPECT_EQ( 2U, vecs.size() );

  pv.reset( new VecList::item_t(1920.f,1080.f) );

  vecs.insert_after( vecs.begin(), std::move(pv) );
  TEXPECT_EQ( 3U, vecs.size() );
  TEXPECT_EQ( 1., vecs.front().x );
  TEXPECT_EQ( 0., vecs.back().x );
  vecs.erase_after( vecs.begin() );
  TEXPECT_EQ( 2U, vecs.size() );
  TEXPECT_EQ( 1., vecs.front().x );
  TEXPECT_EQ( 0., vecs.back().x );

  TEXPECT_EQ( false, bool(pv) );
  pv = vecs.pull_back();
  TEXPECT_EQ( true, bool(pv) );
  Vec tmp{0.f,0.f};
  TEXPECT_EQ( tmp, pv->value );
  TEXPECT_EQ( 1U, vecs.size() );

  vecs.clear();
  TEXPECT_EQ( 0U, vecs.size() );
  
  vecs.insert_after( vecs.before_begin(), std::move(pv) );
  TEXPECT_EQ( 1U, vecs.size() );
  TEXPECT_EQ( false, bool(pv) );

  auto &vec = vecs.emplace_after( vecs.before_begin(), Vec{42.f,42.f} );
  TEXPECT_EQ( 42.f, vec.x );
  vecs.pull_after( vecs.before_begin() );

  Vec tmp2{28,28};

  pv.reset( new VecList::item_t(tmp2) );
  TEXPECT_EQ( true, bool(pv) );

  vecs.insert_after( vecs.before_end(), std::move(pv) );
  TEXPECT_EQ( 2U, vecs.size() );

  const VecList &cvecs = vecs;
  TEXPECT_EQ( tmp, cvecs.front() );
  TEXPECT_EQ( tmp2, cvecs.back() );
  TEXPECT_EQ( tmp, *++cvecs.before_begin() );
  TEXPECT_EQ( tmp2, *cvecs.before_end() );
  // Tricy to test, but for completenes...
  const VecList::iterator &cit = vecs.before_end();
  const Vec &cvec = *cit;
  TEXPECT_EQ( tmp2, cvec );
 
}