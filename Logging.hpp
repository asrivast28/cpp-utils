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
#include <boost/log/expressions/keyword_fwd.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>


namespace logging = boost::log;

logging::trivial::severity_level
log_severity(
  const std::string& level
)
{
  logging::trivial::severity_level sl;
  if (!logging::trivial::from_string(level.c_str(), level.length(), sl)) {
    sl = logging::trivial::info;
  }
  return sl;
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

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", int)
using logger_type = logging::sources::severity_channel_logger<logging::trivial::severity_level, int>;

/**
 * @brief Class for providing a singleton instance of the logger.
 */
class Logger {
public:
  /**
   * @brief Default constructor.
   */
  Logger()
    : m_logger()
  {
  }

  /**
   * @brief Deleted copy constructor.
   */
  Logger(const Logger&) = delete;

  /**
   * @brief Deleted assignment operator.
   */
  void
  operator=(const Logger&) = delete;

  /**
   * @brief Function for setting the underlying logger.
   */
  void
  set(const logger_type& logger)
  {
    m_logger = logger;
  }

  /**
   * @brief Operator for getting the underlying logger.
   */
  logger_type&
  operator*()
  {
    return m_logger;
  }

  /**
   * @brief Default destructor.
   */
  ~Logger()
  {
  }

public:
  /**
   * @brief Function for getting the singleton instance.
   */
  static
  Logger&
  get()
  {
    static Logger instance;
    return instance;
  }

private:
  logger_type m_logger;
}; // class Logger


// Initialize logging
#define INIT_LOGGING(file, tag, level) \
  if (file == "") { \
    logging::add_console_log(std::cerr, logging::keywords::format = "[%Severity%] %Channel%: %Message%"); \
  } \
  else { \
    logging::add_file_log(logging::keywords::file_name = file, logging::keywords::filter = (channel == tag), logging::keywords::format = "[%Severity%] %Message%"); \
  } \
  Logger::get().set(logger_type(logging::keywords::channel = tag)); \
  logging::core::get()->set_filter(logging::trivial::severity >= log_severity(level))

// Logging macros
#define LOG_MESSAGE(level, ...) \
  BOOST_LOG_SEV(*(Logger::get()), logging::trivial::level) << log_message(__VA_ARGS__)
#define LOG_MESSAGE_IF(exp, level, ...) \
  if (exp) LOG_MESSAGE(level, __VA_ARGS__)

#else
#define INIT_LOGGING(file, tag, level)
#define LOG_MESSAGE(level, ...)
#define LOG_MESSAGE_IF(exp, level, ...)
#endif

#endif // LOGGING_HPP_
