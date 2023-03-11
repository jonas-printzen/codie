#ifndef SPOCK_SINGLE_HDR
#define SPOCK_SINGLE_HDR

#include <codie/common.hpp>

#include <signal.h>

namespace codie {

/** @addtogroup MODEL
 *
 * @{
 */

/** @brief Can't make copyable class singleton, doah! */
struct NoCopy {
  NoCopy( const NoCopy& ) = delete;
  void operator = ( const NoCopy& ) = delete;
  NoCopy() {}
};

/** @brief Singleton factory
 *
 *  This class wraps the functionality for creating a singleton
 *  safely. This implementation forces lifetime binding of the
 *  singleton and will throw if program behaves badly.
 *
 *
 * @code
 * class MyApp : NoCopy {
 *   // Explicit declaration of the allowed lifetime scope
 *   using one = Single<MyApp>;
 *   friend int main(int,char**);
 * public:
 *   // The public API goes here!  
 *   void DoSomething();
 * };
 *
 * int main() {
 *   // Prepare stuff before singleton creation
 *   int ret = 0;
 *   {
 *      MyApp::one _app_instance_; // Creates an instance ..
 *      MyApp &app = MyApp::one::inst();
 *      ret = app.Main();
 *   }  // Instance destroyed
 *   // Cleanup stuff after singleton destruction.
 *   return ret;
 * }
 * @endcode
 */
template <class C>
class Single {
  friend int main(int,char**);
public:

  /** @brief Throwing constructor 
   *
   * @throw Will not accept multiple instances
   */
  Single( auto&&...args ) {
    static_assert( !(std::is_copy_constructible_v<C> || std::is_copy_assignable_v<C>),
     "Can't make copyables singleton!" );
    
    if( pInst ) { throw RuntimeError("Second Single<C> for same class C!"); }
    pInst.reset( new C( std::forward<decltype(args)>(args)... ) );
  }

  ~Single() {
    pInst.reset();
  }

  /** @brief Check if the single is initialized */
  static inline constexpr bool initialized() { return bool(pInst); }

  static inline C& inst() {
    if( pInst )[[likely]] return *pInst;
    else throw RuntimeError("Accessing Single::inst() before creation / after destruction!");
  }

private:
  static aptr_t<C> pInst;
};

template <class C>
aptr_t<C> Single<C>::pInst;

/**
 * @}
 */


} // namespace spock

#endif  // SPOCK_SINGLE_HDR