#include "utest.hpp"
#include <codie/item.hpp>
using namespace codie;

using ADouble = AItem<UID("double"),double>;
using ADummy = AItem<UID("dummy")>;

TCASE(Item,Basic) {
  TEXPECT_EQ( sizeof(uid_t), sizeof(Item) );
  TEXPECT_EQ( sizeof(uid_t)+sizeof(int), sizeof(AItem<UID("int"),int>) );

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
