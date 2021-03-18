/**
 * @file Random.hpp
 * @brief Declaration and implementation of random functions.
 * @author Ankit Srivastava <asrivast@gatech.edu>
 */
#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <algorithm>
#include <random>


/**
 * @brief Picks an index based on the given weights.
 *        Used for safely handling infinite weights,
 *        since std::discrete_distribution expects finite.
 *
 * @tparam IntType Type of integer for picking the index.
 * @tparam InputIt Type of input iterator with weights.
 * @tparam GenType Type of the pseudo random number generator.
 * @param first Iterator to the first weight.
 * @param last Iterator to the last weight.
 * @param generator Instance of the PRNG.
 */
template <typename IntType, typename InputIt, typename GenType>
IntType
discrete_distribution_pick(
  const InputIt first,
  const InputIt last,
  GenType& generator
)
{
  IntType picked = std::distance(first, last);
  static auto findInf = [] (const double w) { return std::isinf(w); };
  auto it = std::find_if(first, last, findInf);
  if (it == last) {
    auto distrib = std::discrete_distribution<IntType>(first, last);
    picked = distrib(generator);
  }
  else {
    generator.discard(1);
    picked = std::distance(first, it);
  }
  return picked;
}

#endif // RANDOM_HPP_
