/*
 * SWG:ANH Combat Upgrade Sandbox
 *
 * @copyright   Copyright (c) 2010 The SWG:ANH Team
 * @author      Eric Barr <apathy@swganh.org>
**/

#ifndef SRC_SHARED_LOGGER_H_
#define SRC_SHARED_LOGGER_H_

#include <sstream>
#include <string>

namespace sandbox {
namespace shared {

class Logger {
 public:
  enum LogLevel {
    ERR = 0,
    WARNING,
    INFO,
    DEBUG,
    DEBUG1,
    DEBUG2,
    DEBUG3,
    DEBUG4
  };

 public:
  Logger();
  ~Logger();

  std::ostringstream& log(LogLevel level);
  std::string getLevelString(LogLevel level);

 public:
  static void setReportingLevel(LogLevel level = INFO);
  static LogLevel getReportingLevel();

 private:
  Logger(const Logger&);
  Logger& operator=(const Logger&);

  std::string timestamp();

  LogLevel instance_level_;
  static LogLevel reporting_level_;

  std::ostringstream output_stream_;
};

}  // namespace sandbox
}  // namespace shared

#endif  // SRC_SHARED_LOGGER_H_
