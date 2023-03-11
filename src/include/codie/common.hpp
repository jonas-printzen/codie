#ifndef CODIE_COMMON_HDR
#define CODIE_COMMON_HDR

#include <stdint.h>
#include <string.h>

#include <type_traits>
#include <stdexcept>
#include <tuple>
#include <span>
#include <vector>
#include <memory>
#include <initializer_list>

#define CODIE_SPEC_STR "libcodie 0.1"

#define CODIE_EXPECT(EXPR,MSG) if(!(EXPR)) { throw RuntimeError(MSG); }

namespace codie {

/** @addtogroup COMMON
 *
 * @{
 */

using std::tuple;
using std::tie;
using std::span;
using std::vector;
using std::array;
using std::initializer_list;
using std::is_same_v;
using std::same_as;


using Exception=std::exception;
using KeyError = std::out_of_range;
// using NotImpl=std::bad_optional_access;
using RuntimeError = std::runtime_error;

using cstr_p = const char *;
using std::string;
using cstrings = span<cstr_p,std::dynamic_extent>;
using vcstrings = vector<cstr_p>;

//==== Vulkan related shorter names =========
using size2d_t = tuple<uint32_t,uint32_t>;
using size3d_t = tuple<uint32_t,uint32_t,uint32_t>;

/** @brief The typed item owned reference-type */
template <typename T, class D=std::default_delete<T>>
using aptr_t=std::unique_ptr<T,D>;

/** @brief Array-sizeof */
inline constexpr size_t asizeof( const auto &array ) {
  return sizeof(array)/sizeof(array[0]);
}

using hash32_t = ::uint32_t;  ///< 32-bit hash value
using hash64_t = ::uint64_t;  ///< 64-bit hash value

/** @brief Compiletime hash */
template <typename H>
inline constexpr H XHASH(const char *txt) {
    H hash = sizeof(H) == 8 ? 0xcbf29ce484222325 : 0x811c9dc5;
    const H prime = sizeof(H) == 8 ? 0x00000100000001b3 : 0x01000193;

    while (*txt) {
        hash ^= static_cast<H>(*txt);
        hash *= prime;
        ++txt;
    }

    return hash;
}

/** 32-bit string hasher
 *
 *  This inline function can be used at both compile-time to create
 *  ***switch*** - able constants and in runtime to generate hashes.
 */
inline constexpr hash32_t UID( auto &&...args ) {
  return XHASH<hash32_t>(std::forward<decltype(args)>(args)...);
}

/** 64-bit string hasher
 *
 *  This inline function can be used at both compile-time to create
 *  ***switch*** - able constants and in runtime to generate hashes.
 */
inline constexpr hash64_t UUID( auto &&...args ) {
  return XHASH<hash64_t>(std::forward<decltype(args)>(args)...);
}

/** @brief Hash on library 'name version' */
extern const hash64_t g_codie_spec;

/**
 * @}
 */

} // namespace codie

#endif  // CODIE_COMMON_HDR