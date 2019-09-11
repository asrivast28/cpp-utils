/**
 * @file Logging.hpp
 * @brief Definition of logging macros.
 * @author Ankit Srivastava <asrivast@gatech.edu>
 */
#ifndef LOGGING_HPP_
#define LOGGING_HPP_

// Use logging only when NDEBUG flag is not defined
#ifndef NDEBUG

#include <string>

#define BOOST_LOG_DYN_LINK 1

#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


std::string
format_helper(
  boost::format& f
)
{
  return boost::str(f);
}

template <typename T, typename... Args>
std::string
format_helper(
  boost::format& f,
  T&& t,
  Args&&... args
)
{
  return format_helper(f % std::forward<T>(t), std::forward<Args>(args)...);
}

template <typename... Args>
std::string
debug_log(
  const std::string& fmt,
  Args&&... args
)
{
  boost::format f(fmt);
  return format_helper(f, std::forward<Args>(args)...);
}

// Initialize logging
#define INIT_LOGGING(level) boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::level);
#define DEBUG_LOG(level, fmt, ...) BOOST_LOG_TRIVIAL(level) <<  debug_log(fmt, __VA_ARGS__);
#define DEBUG_LOG_IF(exp, level, fmt, ...) if (exp) DEBUG_LOG(level, fmt, __VA_ARGS__);

#else
#define INIT_LOGGING(level)
#define DEBUG_LOG(level, fmt, ...)
#define DEBUG_LOG_IF(exp, level, fmt, ...)
#endif

#endif // LOGGING_HPP_
