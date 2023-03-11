
/** @mainpage My codi:ng library
 *
 * This is my own toolkit for speeding up my tooling and design testing.
 * It has no other use than to match my code-snippets and basic ideas
 * about how to quickly write C++ 20 code in a form readable to me.
 *
 * @see Modules
 */


/** @defgroup COMMON Common types
 *
 * @brief DRY starts with commonalities
 *
 * The common types are used throughout the code to provide
 * abstraction-friendly base complementing standard libraries.
 * By explicitly setting some types that are common I find it 
 * easier to prototype API's that are concise and useful.
 *
 * This is still just an embryo, I intend to include constructs
 * that I find particularly usefull as I go along.
 *
 * When prototyping and testing out stuff, I do it my way!
 */

/** @defgroup RANGE Ranges - Python style
 *
 * @brief Just some helper for iterating in 1 and 2 dimensions
 *
 * When ranged for-loops was introduced in C++ 11 I immediately 
 * wanted to write ierations similar to python. This is what I
 * came up with and haven't found anything in standard libraries
 * that is as good. (personal pref)
 *
 */

/** @defgroup MODEL Model - complements
 *
 *  @brief Tools complementing C++ aggregation classes
 *
 *  Sometimes the standard containers in C++ are less than optimal.
 *  Here some complementing constructs I often use...
 *
 */

/** @defgroup TIMERS Some helpers for timing
 *
 *  @brief Shorthands for easy coding of timestamps et'al...
 *
 *  Timestamps in µs, and software timers (***TBC***)
 *
 */

/** @defgroup RND Random generators
 *
 *  @brief Random generation
 *
 *  Creating a random number is split in ***two parts***. The first is a
 *  random-engine, which generates unsigned integer values (16/32/64).
 *  With this you can then select a distribution, which determines
 *  the probability of values between ***min*** and ***max***.
 *
 *  These are my shorthand selections...
 *
 */
