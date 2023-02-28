#include "codie/common.hpp"
#include "utest.hpp"

#include <codie/single.hpp>
using namespace codie;

void TCase_SingleBasic(void); 

class Dummy : NoCopy {
public:

  const string tag;

  static int incarnation;
private:
  using one=Single<Dummy>;
  friend one;
  friend void TCase_SingleBasic(void);

  Dummy( string atag ) : tag(atag) { incarnation++; }
};


int Dummy::incarnation=0;

TCASE(Single,Basic) {
  TEXPECT_EQ( false, Dummy::one::initialized() );

  TEXPECT_THROW( Dummy::one::inst(), RuntimeError );

  // No instance created yet!
  TEXPECT_EQ( 0, Dummy::incarnation );

  {
    // The friend declaration in Dummy allows access to singleton
    Dummy::one _one_inst_only_("First!");
    TEXPECT_EQ( true, Dummy::one::initialized() );
    auto &dummy = Dummy::one::inst();
    TEXPECT_EQ( 1, dummy.incarnation );
    TEXPECT_EQ( string("First!"), dummy.tag );

    try {
      Dummy::one _second_simultaneous_throws_("Nah!"); // Should throw!
      TEXPECT( false ); // Should not get here!
    } catch (const RuntimeError &e ) {
      // All well!
    }
  }
  // Should be dead now!
  Dummy::one another_one("hepp");
  auto &dummy = Dummy::one::inst();
  TEXPECT_EQ( 2, dummy.incarnation );
  TEXPECT_EQ( string("hepp"), dummy.tag );
}