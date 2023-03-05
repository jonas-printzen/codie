#include "utest.hpp"
#include <codie/item.hpp>
using namespace codie;

#include <memory>
using std::unique_ptr;

using ADouble = AItem<UID("double"),double>;
using ADummy = AItem<UID("dummy")>;

TCASE(Item,Basic) {
  TEXPECT_EQ( 4, sizeof(int) );
  TEXPECT_EQ( 4, sizeof(int32_t) );
  
  TEXPECT_EQ( 2*sizeof(uint32_t), sizeof(Item) );
  TEXPECT_EQ( 2*sizeof(uint32_t)+sizeof(int), sizeof(AItem<UID("int"),int>) );

  ADummy dummy;
  TEXPECT_EQ( Item::NONE, dummy.meta().pool );
  
  TEXPECT_EQ( UID("double"),ADouble::ID);
  TEXPECT_EQ( UID("dummy"), ADummy::ID );

  TEXPECT_NE( UID("double"),ADummy::ID);
  TEXPECT_NE( UID("dummy"), ADouble::ID );
}

struct Size2D { 
  int w,h; 
};

using ASize2D = AItem<UID("Size2D"), Size2D>;

TCASE(Item,Is) {
  ADouble di = 1.0;
  ASize2D sz2d = Size2D{5,5};

  Item &i1 = sz2d;
  Item &i2 = di;

  TEXPECT_EQ( 1.0, *di );
  // Conditional, when type is not known
  {
    for( auto &d : ADouble::Is(i2) ) d = 3.14;   // Should update
    for( auto &d : ADouble::Is(i1) ) d = 1.41;   // Should NOT update
  }
  TEXPECT_EQ( 3.14, *di );

  TEXPECT_EQ( 5, sz2d->w );
  TEXPECT_EQ( 5, sz2d->h );
  // Conditional, when type is not known
  {
    for( auto &sz : ASize2D::Is(i1) ) sz.w = 10;  // Should update
    for( auto &sz : ASize2D::Is(i2) ) sz.w = 20;  // Should NOT update
  }
  TEXPECT_EQ( 10, sz2d->w );
  TEXPECT_EQ(  5, sz2d->h );

  // A more direct approach (when type is known)
  *sz2d = {10,10};
  TEXPECT_EQ( 10, sz2d->w );
  TEXPECT_EQ( 10, sz2d->h );
}

// template <typename T>
// struct ItemDeleter {
//   inline void operator () ( T*p ) {
//     switch( p->meta().pool ) {
//     case Item::NEW: delete p; break;
//     case Item::HEAP: ::free(p ); break;
//     default:;
//     }
//   }
// };

class Tracker {
protected:
  // To dis-allow Tracker direct, must use [A/P]Tracker...
  template<hash32_t,typename> friend class codie::AItem;
  template<hash32_t,typename> friend class codie::PItem;
  Tracker() {
    constructs++;
  }

public:
  ~Tracker() { destructs++; }

  static size_t constructs;
  static size_t destructs;
};

size_t Tracker::constructs = 0;
size_t Tracker::destructs = 0;

using ATracker = AItem<UID("ATracker"), Tracker>;
using PTracker = PItem<UID("PTracker"), Tracker>;

TCASE( Item, Pool ) {
  TEXPECT_EQ( 0, Tracker::constructs );
  TEXPECT_EQ( 0, Tracker::destructs );

  {
    ATracker t1;
    TEXPECT_EQ( Item::NONE, t1.meta().pool );
    TEXPECT_EQ( 1, Tracker::constructs );
    TEXPECT_EQ( 0, Tracker::destructs );
  }
  TEXPECT_EQ( 1, Tracker::constructs );
  TEXPECT_EQ( 1, Tracker::destructs );

  {
    PTracker p1;
    TEXPECT_EQ( false, bool(p1) );
    TEXPECT_EQ( 1, Tracker::constructs );
    TEXPECT_EQ( 1, Tracker::destructs );
    p1 = PTracker::create();
    TEXPECT_EQ( true, bool(p1) );
    TEXPECT_EQ( Item::NEW, p1->meta().pool );

    TEXPECT_EQ( 2, Tracker::constructs );
    TEXPECT_EQ( 1, Tracker::destructs );
  }
  TEXPECT_EQ( 2, Tracker::constructs );
  TEXPECT_EQ( 2, Tracker::destructs );

}