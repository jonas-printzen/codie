#include "utest.hpp"
#include <codie/item.hpp>
using namespace codie;

#include <memory>
using std::unique_ptr;

using ADouble = AItem<UUID("double"),double>;
using ADummy = AItem<UUID("dummy")>;

TCASE(Item,Basic) {
  TEXPECT_EQ( 4, sizeof(int) );
  TEXPECT_EQ( 4, sizeof(int32_t) );
  TEXPECT_EQ( 8, sizeof(Item::hash_t) );
  TEXPECT_EQ( sizeof(Item::hash_t), sizeof(Item) );

  ADummy dummy;
  
  TEXPECT_EQ( UUID("double"),ADouble::ID);
  TEXPECT_EQ( UUID("dummy"), ADummy::ID );
  TEXPECT_NE( UUID("double"),ADummy::ID);
  TEXPECT_NE( UUID("dummy"), ADouble::ID );
}

struct Size2D { 
  int w,h; 
};

using ASize2D = AItem<UUID("Size2D"), Size2D>;

TCASE(Item,Is) {
  ADouble di = 1.0;
  ASize2D sz2d = Size2D{5,5};

  Item &i1 = sz2d;
  Item &i2 = di;

  TEXPECT_EQ( true, ADouble::Is( di ) );
  TEXPECT_EQ( false, ADouble::Is( sz2d ) );

  TEXPECT_EQ( 1.0, *di );
  // Conditional, when type is not known
  {
    for( auto &d : ADouble::If(i2) ) d = 3.14;   // Should update
    for( auto &d : ADouble::If(i1) ) d = 1.41;   // Should NOT update
  }
  TEXPECT_EQ( 3.14, *di );

  TEXPECT_EQ( 5, sz2d->w );
  TEXPECT_EQ( 5, sz2d->h );
  // Conditional, when type is not known
  {
    for( auto &sz : ASize2D::If(i1) ) sz.w = 10;  // Should update
    for( auto &sz : ASize2D::If(i2) ) sz.w = 20;  // Should NOT update
  }
  TEXPECT_EQ( 10, sz2d->w );
  TEXPECT_EQ(  5, sz2d->h );

  // A more direct approach (when type is known)
  *sz2d = {10,10};
  TEXPECT_EQ( 10, sz2d->w );
  TEXPECT_EQ( 10, sz2d->h );
}
