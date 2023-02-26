#include "utest.hpp"

#include <codie/common.hpp>
using namespace codie;

TCASE(Codie,Basic) {
  // Generic assumptions
  TEXPECT_EQ( 4, sizeof(int)  );
  TEXPECT_EQ( 8, sizeof(long)  );
  TEXPECT_EQ( 4, sizeof(float)  );
  TEXPECT_EQ( 8, sizeof(double)  );
}


const char* hashables[] = {
  "net.pzen.codie:filesvc.read",
  "net.pzen.codie:filesvc.readl",
  "net.pzen.codie:filesvc.raw",
  "net.pzen.codie:filesvc.text",
  "TiddeliPom"
};

TCASE(Codie,Hash) {
  // This is a key use-case
  TEXPECT_EQ( g_codie_spec, UUID(CODIE_SPEC_STR) );

  // Hashes in general
  hash32_t h1 = UID(hashables[3]);

  {
    int found_idx = -1;
    for( unsigned n=0; n<(sizeof(hashables)/sizeof(*hashables)); ++n ) {
      if( UID(hashables[n]) == h1 ) {
        found_idx = n;
      }
    }

    TEXPECT( found_idx >= 0 );
    TEXPECT_EQ( UID(hashables[found_idx]), h1 );
    TEXPECT_EQ( 3, found_idx );
  }

  {
    hash64_t h2 = UUID(hashables[2]);

    int found_idx = -1;
    for( unsigned n=0; n<(sizeof(hashables)/sizeof(*hashables)); ++n ) {
      if( UUID(hashables[n]) == h2 ) {
        found_idx = n;
      }
    }

    TEXPECT( found_idx >= 0 );
    TEXPECT_EQ( UUID(hashables[found_idx]), h2 );
    TEXPECT_EQ( 2, found_idx );
  }

  {
    // The switch is a key use-case!!!
    int tiddelipom=-1;
    for( size_t i=0; i<asizeof(hashables); ++i ) {
      switch( UID(hashables[i]) ) {
      case UID("TiddeliPom"): tiddelipom  = i; break;
      default: /* Nope */;
      }
    }
    TEXPECT_EQ( 4, tiddelipom );
  }

}

struct TestVec {
  long x,y,z;
};

TCASE(Codie,ASizeOf) {
  int ints[2] = {0};
  float floats[5]={0.f};
  TestVec vecs[3]={0};

  TEXPECT_EQ( 2, asizeof(ints) );
  TEXPECT_EQ( 5, asizeof(floats) );
  TEXPECT_EQ( 3, asizeof(vecs) );
}


TCASE(Codie,IsSame) {
  TEXPECT_EQ( true, (is_same_v<int,int>) );
  TEXPECT_EQ( false, (is_same_v<int,short>) );
  TEXPECT_EQ( false,(is_same_v<int,float>) );
  TEXPECT_EQ( false,(is_same_v<double,float>) );
}

TCASE(Codie,Memory) {
  aptr_t<int> pi;
  TEXPECT_EQ(false,bool(pi));

  pi.reset(new int(0));

  TEXPECT_EQ(true,bool(pi));
  TEXPECT_EQ(0,*pi);
  *pi = 2;

  TEXPECT_EQ(2,*pi);
}

struct bullet {
  static int n_active;

  using ptr_t = aptr_t<bullet>;

  inline virtual ~bullet() {
    --n_active;
  }

  inline static ptr_t create() {
    return ptr_t(new bullet());
  }

protected:
  bullet() {
    ++n_active;
  }
};

int bullet::n_active = 0;

TCASE(Codie,aptr_t) {
  TEXPECT_EQ( 0, bullet::n_active );
  bullet::ptr_t br;
  TEXPECT_EQ( 0, bullet::n_active );
  TEXPECT( !br );

  br = bullet::create();
  TEXPECT( br );
  TEXPECT_EQ( 1, bullet::n_active );
  br.reset();
  TEXPECT( !br );
  TEXPECT_EQ( 0, bullet::n_active );
}


TCASE(Codie,Span) {
  int s_ints[] = {1,5,17,42};
  vector<int> s_vec_ints{ 2,6,18,43, 72 };
  array<string,3> s_arr_txts{"Killroy","was","here!"};

  TEXPECT_EQ(4, codie::asizeof(s_ints) );
  TEXPECT_EQ( 5U, s_vec_ints.size() );
  TEXPECT_EQ( 3, s_arr_txts.size() );
  

  {
    auto ints = std::span{s_ints};
    TEXPECT_EQ( 4, ints.size() );

    TEXPECT_EQ( 1, ints[0] );
    TEXPECT_EQ( 42, ints[3] );
  }

  {
    auto ints = std::span{s_vec_ints};
    TEXPECT_EQ( 5, ints.size() );

    TEXPECT_EQ( 2, ints[0] );
    TEXPECT_EQ( 72, ints[4] );
  }

  {
    auto txts = std::span{s_arr_txts};
    TEXPECT_EQ( 3, txts.size() );

    TEXPECT_EQ( string("was"), txts[1] );
    TEXPECT_EQ( string("Killroy"), txts.front() );
    TEXPECT_EQ( string("here!"), txts.back() );
  }

}

struct bullet_ng : bullet {
  uint64_t _random = 42;
  using ptr_t = aptr_t<bullet_ng>;

  inline static ptr_t create() {
    return ptr_t(new bullet_ng());
  }
};

// ????
TCASE(Codie,UpCast) {
  TEXPECT_EQ( 0, bullet_ng::n_active );
  bullet_ng::ptr_t br;
  TEXPECT_EQ( 0, bullet_ng::n_active );
  TEXPECT( !br );

  br = bullet_ng::create();
  TEXPECT( br );
  TEXPECT_EQ(42,br->_random);

  bullet::ptr_t base(br.release());
  // ???!!!
}

using ret_t = tuple<int,float>;

static ret_t GetIt() {
  return {42,0.5f};
}

TCASE(Codie, Tuple) {
  auto [i,f] = GetIt();

  TEXPECT_EQ( 42, i );
  TEXPECT_EQ( 0.5f, f );
}

TCASE(Codie, Tie) {
  int i=0;
  float f = 0.f;

  tie(i,f) = GetIt();

  TEXPECT_EQ( 42, i );
  TEXPECT_EQ( 0.5f, f );
}
