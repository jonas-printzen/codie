#include "utest.hpp"

#include <codie/islist.hpp>
using namespace codie;

//TODO: Test StateHook variant

class Gadget : public islink<Gadget> {
public:
  using List = islist<Gadget>;

  Gadget( cstr_p tag ) : m_tag(tag) {}

  cstr_p Tag() const { return m_tag; }

  inline void Tick(void) { ++m_ticks; }

  inline uint64_t Ticks(void) const { return m_ticks; }

private:
  cstr_p    m_tag;
  uint64_t  m_ticks=0;
};

TCASE(ISList,TestMocs) {
  Gadget::List gadgets;

  Gadget g1("First");
  TEXPECT_EQ( string("First"), g1.Tag() );
}

template <typename C>
inline unsigned count( const C&bag ) {
  unsigned n = 0;
  for( auto &any : bag ) {
    std::ignore = any;
    ++n;
  }
  return n;
}


TCASE(ISList,NonConst) {
  Gadget::List gadgets;

  Gadget g1("First");
  Gadget g2("Second");

  TEXPECT_EQ( 0, g1.Ticks() );
  TEXPECT_EQ( 0, g2.Ticks() );

  TEXPECT_EQ( 0U, count(gadgets) );
  gadgets.link_front(g1);
  TEXPECT_EQ( 1U, count(gadgets) );
  gadgets.link_back(g2);
  TEXPECT_EQ( 2U, count(gadgets) );

  TEXPECT_EQ( 0,g1.Ticks() );
  TEXPECT_EQ( 0,g2.Ticks() );

  for( auto &any : gadgets ) {
    any.Tick();
  }

  TEXPECT_EQ( 1,g1.Ticks() );
  TEXPECT_EQ( 1,g2.Ticks() );

  gadgets.unlink_front();
  TEXPECT_EQ( 1U, count(gadgets) );
}


TCASE(ISList, Basic) {
  Gadget::List gadgets;

  TEXPECT_EQ( 0U, gadgets.size() );

  Gadget g1("adam"), g2("bertil");

  gadgets.link_back(g1);

  TEXPECT_EQ( 1U, gadgets.size() );
  TEXPECT_EQ( true, gadgets.contains( g1 ) );
  TEXPECT_EQ( false, gadgets.contains( g2 ) );
  // UB! TEXPECT_THROW( gadgets.unlink_after(gadgets.front()), RuntimeError );

  gadgets.unlink_after( gadgets.before_front() );
  TEXPECT_EQ( 0U, gadgets.size() );
  TEXPECT_EQ( false, gadgets.contains( g1 ) );
  TEXPECT_EQ( false, gadgets.contains( g2 ) );

  // UB! TEXPECT_THROW( gadgets.unlink_after(gadgets.before_front()), RuntimeError );

  TEXPECT_EQ( 0U, gadgets.size() );

  gadgets.link_back(g1);
  gadgets.link_back(g2);
  TEXPECT_EQ( 2U, gadgets.size() );

  Gadget::List::iterator gi = gadgets.begin();
  TEXPECT( gi );
  TEXPECT_EQ( string("adam"), gi->Tag() );
  ++gi;
  TEXPECT( gi );
  TEXPECT_EQ( string("bertil"), gi->Tag() );
  ++gi;
  TEXPECT( !(gi) );

  Gadget g3("cecar");

  gadgets.link_front( g3 );

  TEXPECT_EQ( 3U, gadgets.size() );

  // Let's make sure we assume correct
  TEXPECT_EQ( &g3, &gadgets.front() );
  TEXPECT_EQ( &g2, &gadgets.back() );

  gadgets.unlink_after(g1);
  TEXPECT_EQ( &g3, &gadgets.front() );
  TEXPECT_EQ( &g1, &gadgets.back() );
  TEXPECT_EQ( 2U, gadgets.size() );

  gadgets.link_after( g1, g2 );
  TEXPECT_EQ( &g3, &gadgets.front() );
  TEXPECT_EQ( &g2, &gadgets.back() );
  TEXPECT_EQ( 3U, gadgets.size() );

  gi = gadgets.begin();

  TEXPECT( gi );
  TEXPECT_EQ( string("cecar"), gi->Tag() );
  ++gi;
  TEXPECT( gi );
  TEXPECT_EQ( string("adam"), gi->Tag() );
  ++gi;
  TEXPECT( gi );
  TEXPECT_EQ( string("bertil"), gi->Tag() );
  ++gi;
  TEXPECT( !(gi) );

  //=== Removing ===
  // Find the middle
  TEXPECT_EQ( 3U, gadgets.size() );
  gadgets.unlink_front();
  TEXPECT_EQ( 2U, gadgets.size() );

  gi = gadgets.begin();
  TEXPECT_EQ( string("adam"), gi->Tag() );
  ++gi;
  TEXPECT_EQ( string("bertil"), gi->Tag() );
}

TCASE(ISList, Destruct) {
  Gadget g1("Adam"), g2("Bertil"), g3("Cecar");

  TEXPECT_EQ( nullptr, (g1.next()) );
  TEXPECT_EQ( nullptr, (g2.next()) );
  TEXPECT_EQ( nullptr, (g3.next()) );

  {
    Gadget::List gadgets;
    TEXPECT_EQ( 0U, gadgets.size() );

    gadgets.link_back(g1);
    gadgets.link_back(g2);
    gadgets.link_back(g3);
    TEXPECT_EQ( 3U, gadgets.size() );
    // Only the two first has next...
    TEXPECT_NE( nullptr, g1.next() );
    TEXPECT_NE( nullptr, g2.next() );
    TEXPECT_EQ( nullptr, g3.next() );

    // Remove the one in the middle
    TEXPECT_EQ(true, gadgets.find_unlink(g2));
    TEXPECT_EQ( 2U, gadgets.size() );
    // Only the two first has next...
    TEXPECT_NE( nullptr, g1.next() );
    TEXPECT_EQ( nullptr, g2.next() );
    TEXPECT_EQ( nullptr, g3.next() );
    TEXPECT_EQ( false, gadgets.find_unlink(g2) );
    TEXPECT_NE( nullptr, g1.next() );
    TEXPECT_EQ( nullptr, g2.next() );
    TEXPECT_EQ( nullptr, g3.next() );

    TEXPECT_EQ( true, gadgets.find_unlink(g1) );
    TEXPECT_EQ( 1U, gadgets.size() );
    TEXPECT_EQ( nullptr, g1.next() );
    TEXPECT_EQ( nullptr, g2.next() );
    TEXPECT_EQ( nullptr, g3.next() );
  }

  // Now Gadget::List has unlinked it's members
  TEXPECT_EQ( nullptr, g1.next() );
  TEXPECT_EQ( nullptr, g2.next() );
  TEXPECT_EQ( nullptr, g3.next() );
}
