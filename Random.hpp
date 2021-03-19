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
 * @brief Safe version of std::discrete_distribution
 *        which handles infinite weights.
 *
 * @tparam IntType Type of integer for picking the index.
 */
template <typename IntType = int>
class discrete_distribution_safe {
public:
 /**
  * @brief Constructor for the distribution.
  *
  * @tparam InputIt Type of input iterator with weights.
  * @param first Iterator to the first weight.
  * @param last Iterator to the last weight.
  */
  template <typename InputIt>
  discrete_distribution_safe(
    const InputIt first,
    const InputIt last
  ) : m_distrib(first, last),
      m_infiniteIndex(std::numeric_limits<IntType>::max()),
      m_infiniteWeight(false)
  {
    static auto findInf = [] (const double w) { return std::isinf(w); };
    auto it = std::find_if(first, last, findInf);
    if (it != last) {
      m_infiniteIndex = std::distance(first, it);
      m_infiniteWeight = true;
    }
  }

  /**
   * @brief Returns a random index based on the given weights if all
   *        the weights are finite. Otherwise, returns the index of
   *        the first infinite weight. Always moves PRNG state forward.
   *
   * @tparam GenType Type of the pseudo random number generator.
   * @param generator Instance of the PRNG.
   */
  template <typename Generator>
  IntType
  operator()(Generator& generator)
  {
    if (!m_infiniteWeight) {
      return m_distrib(generator);
    }
    else {
      generator.discard(1);
      return m_infiniteIndex;
    }
  }

private:
  std::discrete_distribution<IntType> m_distrib;
  IntType m_infiniteIndex;
  bool m_infiniteWeight;
}; // class discrete_distribution_safe

#endif // RANDOM_HPP_
