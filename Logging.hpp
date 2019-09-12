/**
 * @file Logging.hpp
 * @brief Definition of logging macros.
 * @author Ankit Srivastava <asrivast@gatech.edu>
 */
#ifndef LOGGING_HPP_
#define LOGGING_HPP_

// Use logging only when LOGGING flag is defined
#ifdef LOGGING

#include <string>

#define BOOST_LOG_DYN_LINK 1

#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


boost::log::trivial::severity_level
log_severity(
  const std::string& level
)
{
  if (level == "trace") {
    return boost::log::trivial::trace;
  }
  else if (level == "debug") {
    return boost::log::trivial::debug;
  }
  else if (level == "info") {
    return boost::log::trivial::info;
  }
  else if (level == "warning") {
    return boost::log::trivial::warning;
  }
  else if (level == "error") {
    return boost::log::trivial::error;
  }
  else if (level == "fatal") {
    return boost::log::trivial::fatal;
  }
  else {
    return boost::log::trivial::info;
  }
}

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
log_message(
  const std::string& fmt,
  Args&&... args
)
{
  boost::format f(fmt);
  return format_helper(f, std::forward<Args>(args)...);
}

// Initialize logging
#define INIT_LOGGING(level) boost::log::core::get()->set_filter(boost::log::trivial::severity >= log_severity(level));

// Logging macros
#define LOG_MESSAGE(level, ...) BOOST_LOG_TRIVIAL(level) <<  log_message(__VA_ARGS__);
#define LOG_MESSAGE_IF(exp, level, ...) if (exp) LOG_MESSAGE(level, __VA_ARGS__);

#else
#define INIT_LOGGING(level)
#define LOG_MESSAGE(level, ...)
#define LOG_MESSAGE_IF(exp, level, ...)
#endif

#endif // LOGGING_HPP_
