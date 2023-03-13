#include "codie/common.hpp"
#include "utest.hpp"

#include <codie/mfile.hpp>
#include <cstddef>
#include <istream>

using namespace codie;

TCASE(MFile,Basic) {
  MFile lorem("test/data/lorem.txt");

  size_t count = 0;
  for( char c : lorem.as_text() ) std::ignore=c,count++;
  TEXPECT_EQ( 6884, count );
  TEXPECT_EQ( count, lorem.size() );
}

TCASE(MFile, IStream) {
  MFile lorem("test/data/lorem.txt");

  auto buf = lorem.as_buf();
  std::istream in(&buf);

  size_t count=0;

  string txt;
  while( !in.eof() ) {
    in >> txt;
    if(!txt.empty()) count++;
  }

  TEXPECT_EQ( 1001, count );
}
