/**
 * @file Timer.hpp
 * @brief Declaration and implementation of timer functions.
 * @author Ankit Srivastava <asrivast@gatech.edu>
 */
#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <chrono>

class Timer {
public:
  /**
   * @brief  Types for specifying the duration type of elapsed time.
   */
  using MilliSeconds = std::chrono::duration<float, std::chrono::milliseconds::period>;
  using Seconds = std::chrono::duration<float, std::chrono::seconds::period>;
  using Minutes = std::chrono::duration<float, std::chrono::minutes::period>;
  using Hours = std::chrono::duration<float, std::chrono::hours::period>;

public:
  /**
   * @brief  Default constructor starts the timer.
   */
  Timer()
    : m_startTime(),
      m_duration(),
      m_running()
  {
    start();
  }

  /**
   * @brief  Function for starting/resetting the timer.
   */
  void
  start()
  {
    m_startTime = std::chrono::steady_clock::now();
    m_running = true;
  }

  /**
   * @brief  Function for pausing the timer.
   */
  void
  pause()
  {
    if (m_running) {
      m_duration += (std::chrono::steady_clock::now() - m_startTime);
      m_running = false;
    }
  }

  /**
   * @brief  Function for getting the elapsed time.
   *
   * @tparam  DurationType  Type of the elapsed duration.
   *
   * @return  Returns the total elapsed duration for which the timer was running.
   */
  template <typename DurationType = MilliSeconds>
  float 
  elapsed() const
  {
    std::chrono::steady_clock::duration d = m_duration;
    if (m_running) {
      d += (std::chrono::steady_clock::now() - m_startTime);
    }
    auto e = std::chrono::duration_cast<DurationType>(d);
    return static_cast<float>(e.count());
  }

  /**
   * @brief  Default destructor.
   */
  ~Timer()
  {
  }

private:
  std::chrono::steady_clock::time_point m_startTime;
  std::chrono::steady_clock::duration m_duration;
  bool m_running;
}; // class Timer

#endif // TIMER_HPP_
