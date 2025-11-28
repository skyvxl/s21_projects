#pragma once

#include <fstream>
#include <mutex>
#include <string>

namespace s21 {

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
 public:
  static Logger& getInstance();

  void setLogFile(const std::string& filename);
  void setLogLevel(LogLevel level);
  void setConsoleOutput(bool enabled);

  void log(LogLevel level, const std::string& message);
  void debug(const std::string& message);
  void info(const std::string& message);
  void warning(const std::string& message);
  void error(const std::string& message);

  // Запрет копирования и перемещения (Singleton)
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

 private:
  Logger();
  ~Logger();

  std::string levelToString(LogLevel level) const;
  std::string getCurrentTimestamp() const;
  bool shouldLog(LogLevel level) const;

  std::ofstream logFile_;
  LogLevel minLogLevel_;
  bool consoleOutput_;
  std::mutex mutex_;
};

}  // namespace s21
