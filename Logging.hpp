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

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", logging::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", int)
using logger_type = logging::sources::severity_channel_logger<logging::trivial::severity_level, int>;

/**
 * @brief Class which provides the logging functionality.
 */
class Logger {
private:
  /**
   * @brief Static function for getting the singleton instance
   *        of this class.
   */
  static
  Logger&
  get()
  {
    static Logger instance;
    return instance;
  }

  /**
   * @brief Static function for getting the severity level from
   *        its string representation.
   */
  static
  logging::trivial::severity_level
  severity(
    const std::string& level
  )
  {
    logging::trivial::severity_level sl;
    if (!logging::trivial::from_string(level.c_str(), level.length(), sl)) {
      sl = logging::trivial::info;
    }
    return sl;
  }

  static
  std::string
  format_helper(
    boost::format& f
  )
  {
    return boost::str(f);
  }

  template <typename T, typename... Args>
  static
  std::string
  format_helper(
    boost::format& f,
    T&& t,
    Args&&... args
  )
  {
    return Logger::format_helper(f % std::forward<T>(t), std::forward<Args>(args)...);
  }

public:
  /**
   * @brief Static function for initializing logging.
   *
   * @param file The name of the file to be used for logging.
   *             Console is used if the name is empty.
   * @param tag The tag to be used for logging to channels.
   *            Useful for logging from multiple processes.
   * @param level Minimum severity level of messages to be logged.
   */
  static
  void
  init(
    const std::string& file,
    const int tag,
    const std::string& level
  )
  {
    if (file == "") {
      logging::add_console_log(
        std::cerr,
        logging::keywords::format = "[%Severity%] %Channel%: %Message%"
      );
    }
    else {
      logging::add_file_log(
        logging::keywords::file_name = file,
        logging::keywords::filter = (channel == tag),
        logging::keywords::format = "[%Severity%] %Message%"
      );
    }
    Logger::get().set(logger_type(logging::keywords::channel = tag));
    logging::core::get()->set_filter(logging::trivial::severity >= severity(level));
  }

  /**
   * @brief Static function for re-enabling loggging,
   *        if it was disabled.
   */
  static
  void
  enable()
  {
    if (!logging::core::get()->get_logging_enabled()) {
      logging::core::get()->set_logging_enabled(true);
    }
  }

  /**
   * @brief Static function for disabling loggging,
   *        if it was enabled.
   */
  static
  void
  disable()
  {
    if (logging::core::get()->get_logging_enabled()) {
      logging::core::get()->flush();
      logging::core::get()->set_logging_enabled(false);
    }
  }

  /**
   * @brief Static function for logging a message.
   *
   * @tparam Args Type of the variable formatter arguments.
   * @param sl Severity level of the message.
   * @param fmt Format string of the message.
   * @param args Formatting args for the message.
   */
  template <typename... Args>
  static
  void
  message(
    const logging::trivial::severity_level sl,
    const std::string& fmt,
    Args&&... args
  )
  {
    boost::format f(fmt);
    BOOST_LOG_SEV(*(Logger::get()), sl) << Logger::format_helper(f, std::forward<Args>(args)...);
  }

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
   * @brief Default destructor.
   */
  ~Logger()
  {
  }

private:
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

private:
  logger_type m_logger;
}; // class Logger

// Macros for logging
#define INIT_LOGGING(file, tag, level) Logger::init(file, tag, level)
#define START_LOGGING() Logger::enable()
#define PAUSE_LOGGING() Logger::disable()
#define LOG_MESSAGE(level, ...) Logger::message(logging::trivial::level, __VA_ARGS__)
#define LOG_MESSAGE_IF(exp, level, ...) if (exp) Logger::message(logging::trivial::level, __VA_ARGS__)

#else
#define INIT_LOGGING(file, tag, level)
#define START_LOGGING()
#define PAUSE_LOGGING()
#define LOG_MESSAGE(level, ...)
#define LOG_MESSAGE_IF(exp, level, ...)
#endif // LOGGING

#endif // LOGGING_HPP_
