#ifndef CODIE_RANDOM_HDR
#define CODIE_RANDOM_HDR

#include <codie/common.hpp>

#include <random>

namespace codie {

/** @addtogroup RND 
 *
 * @{
 */

/** @brief Random-generator, fast, small state,
 *
 *  This generator is a decent tradeoff for speed and
 *  spectral/repetitiveness.
 */
template <typename...ARGS>
using linear_congruential = std::linear_congruential_engine<ARGS...>;

/** @brief 32-bit fast random-generator
 *
 *
 */
using minstd_rand=std::minstd_rand;

/** @brief Random-generator, slow, big, non-repeating
 *
 *  This generator focus on better randomness, longest
 *  non-repeating sequence and better spectral characteristics
 *
 *  This is the base of mt19937 and mt19937_64
 */
template <typename...ARGS>
using mersenne_twister=std::mersenne_twister_engine<ARGS...>;

/** @brief A common 32-bit random generator
 *
 *  This generator is a 32-bit mersenne_twister_engine with
 *  proven initialization paramenters.
 *
 *  Constructor takes a unsigned seed
 */
using mt19937_32=std::mt19937;

/** @brief A common 64-bit random generator
 *
 *  This generator is a 64-bit mersenne_twister_engine with
 *  proven initialization paramenters.
 */
using mt19937_64=std::mt19937_64;

/** @brief The uniform real distribution, float/double
 *
 *  This is the straight forward random distribution. In theory
 *  it is always the same probability throughout the range.
 *
 *  You specify <float/double> as type and min,max value.
 */
template <typename...ARGS>
using uniform_real_dist=std::uniform_real_distribution<ARGS...>;

/** @brief The uniform real distribution, integer
 *
 *  Again, same probability throughout the range,
 *  here with integers.
 *
 *  You specify <int-type> and min,max value.
 */
template <typename...ARGS>
using uniform_int_dist=std::uniform_int_distribution<ARGS...>;

/** @brief Normal distribution
 *
 *  This distribution has a ***mean*** and a ***standard deviation***.
 *  Random values will cluster around the mean and spread according
 *  to the deviation.
 *
 * @note In theory there is no min/max. But this can be enforced in code.<br>
 *       Da rule is:
 *         - 99.7% is within mean +/- 3stdv
 *         - 95% +/-2stdv
 *         - 68 +/- 1stdv
 *
 *  Your specify <float|double>, mean and stdv
 */
template <typename...ARGS>
using normal_dist=std::normal_distribution<ARGS...>;

/** @brief Binary/Set/classification distribution
 *
 *  You specify <int-type>, set-size and probability.
 */
template <typename...ARGS>
using binomial_dist=std::binomial_distribution<ARGS...>;

/** @brief (re)occurence TBD */
template <typename...ARGS>
using poisson_dist=std::poisson_distribution<ARGS...>;

/**
 * @}
 */


} // codie

#endif // CODIE_RANDOM_HDR
