#include "utest.hpp"

#include <codie/common.hpp>
using namespace codie;

// Found no variant that works everywhere
template <const char (&TAG)[]>
struct Hepp {
  static constexpr const hash32_t ID=UID(TAG);
};

TCASE(Cxx,TemplateString) {
  // Not yet!
  // Hepp<"Hepp"> hepp;
}