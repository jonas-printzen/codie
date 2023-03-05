#ifndef PZEN_UTEST_HDR
#define PZEN_UTEST_HDR

#include <stdint.h>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

#define _ITOS_(N) #N
#define _ITOS(N) _ITOS_(N)
#define __SLINE__ _ITOS(__LINE__)

struct test_map {
  using fn = void(*)(void);
  using ptr = test_map*;
  static ptr p_head, p_tail;
  test_map(fn,const char *);

  void operator() ();

  fn m_test;
  const char *m_name;
  ptr p_next;
};

struct Unexpected {
  Unexpected( const std::string &t, const char *pFile, int line, std::string xtra=""  )
   : msg(t), other(xtra), _file_(pFile), _line_(line) {}

  std::string msg;
  std::string other;

  const char *_file_ = nullptr;
  int         _line_;
};


#define TCASE( GRP, N ) \
void TCase_##GRP##N(void); \
static test_map TLink_##GRP##N(TCase_##GRP##N, #GRP "_" #N);\
void TCase_##GRP##N(void)

#define TEXPECT(IS)\
  do { \
    if( !(true == bool(IS)) )\
     throw Unexpected(#IS, __FILE__, __LINE__ ); \
  } while(false)

#define TEXPECT_EQ(EXPECT, IS)\
  do { \
    if( !((IS) == (EXPECT)) )\
      throw Unexpected(#EXPECT "==" #IS, __FILE__, __LINE__ );\
  } while(false)

#define TEXPECT_NE(EXPECT, IS)\
  do {\
    if( !((IS) != (EXPECT)) )\
      throw Unexpected(#EXPECT "!=" #IS, __FILE__, __LINE__);\
  } while(false)

#define TEXPECT_THROW(EXPR, EXCEPT)\
  do {\
    try{\
      (EXPR);\
      throw Unexpected(#EXPR " didn't throw!",__FILE__, __LINE__ );\
    } catch(const EXCEPT &e) {\
      break;\
    } catch(std::exception&e){\
      throw Unexpected(#EXPR " threw other!",__FILE__, __LINE__,e.what());\
    }\
  }while(false)

#define TEXPECT_NOTHROW(EXPR)\
  do {\
    try{\
      (EXPR);\
    } catch(...) {\
      throw Unexpected(#EXPR " threw!", __FILE__ , __LINE__ );\
    }\
  } while(false)

#endif // PZEN_UTEST_HDR
