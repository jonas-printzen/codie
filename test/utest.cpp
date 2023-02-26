#include "utest.hpp"

#include <iostream>
#include <chrono>
#include <string.h>
using namespace std;

test_map::ptr test_map::p_head = nullptr;
test_map::ptr test_map::p_tail = nullptr;

inline std::ostream& operator << ( std::ostream&out, const Unexpected &ux) {
  out << ux.msg;
  if( !ux.other.empty() ) out << "[" << ux.other << "]";
  out  << " in " << ux._file_ << "(" << ux._line_ << ")";
  return out;
}

uint64_t Now() {
  auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
}

test_map::test_map(fn tfn,const char *name)
  : m_test(tfn), m_name(name) {
  p_next = nullptr;
  if( p_tail ) p_tail->p_next = this;
  p_tail = this;
  if( nullptr == p_head ) p_head = p_tail;
}

void test_map::operator() () {
  (*m_test)();
}

int main() {
  int n_tests=0;
  int n_tests_ok=0;
  // Estimage sleep-delay
  uint64_t t0=Now();
  uint64_t t1=Now();
  auto TDELAY = (t1-t0)/2;

  t0 = Now();
  for( auto t = test_map::p_head; nullptr != t; t = t->p_next ) {
    ++n_tests;
    try {
      cout << " " << t->m_name << " ...";
      (*t)();
      ++n_tests_ok;
      cout << " OK!\n";
      cout.flush();
    } catch ( Unexpected ux) {
      cout << " Failed @ " << ux << "!\n";
    } catch(std::exception &e) {
      cout << " Failed, " << e.what() <<  "(" << ::strerror(errno) << ")!\n";
    } catch(...) {
      cout << " Failed, unknown exception!\n";
    }
  }

  t1=Now();

  cout << "-----------------------\n";
  cout << "Time spent: " << (t1-t0-TDELAY) << " microseconds.\n";
  if( n_tests_ok == n_tests ) {
    cout << "SUCCESS!  ";
  } else {
    cout << "FAILURE!  ";
  }
  cout << n_tests_ok << " of " << n_tests << " tests OK!\n";
}
