#include "logger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

namespace s21 {

Logger::Logger() : minLogLevel_(LogLevel::INFO), consoleOutput_(true) {}

Logger::~Logger() {
  if (logFile_.is_open()) {
    logFile_.close();
  }
}

Logger& Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::setLogFile(const std::string& filename) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (logFile_.is_open()) {
    logFile_.close();
  }

  logFile_.open(filename, std::ios::app);
  if (!logFile_.is_open()) {
    std::cerr << "Failed to open log file: " << filename << std::endl;
  }
}

void Logger::setLogLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(mutex_);
  minLogLevel_ = level;
}

void Logger::setConsoleOutput(bool enabled) {
  std::lock_guard<std::mutex> lock(mutex_);
  consoleOutput_ = enabled;
}

void Logger::log(LogLevel level, const std::string& message) {
  if (!shouldLog(level)) {
    return;
  }

  std::lock_guard<std::mutex> lock(mutex_);

  std::ostringstream logEntry;
  logEntry << "[" << getCurrentTimestamp() << "] " << "["
           << levelToString(level) << "] " << message;

  const std::string entry = logEntry.str();

  if (consoleOutput_) {
    if (level == LogLevel::ERROR || level == LogLevel::WARNING) {
      std::cerr << entry << std::endl;
    } else {
      std::cout << entry << std::endl;
    }
  }

  if (logFile_.is_open()) {
    logFile_ << entry << std::endl;
    logFile_.flush();
  }
}

void Logger::debug(const std::string& message) {
  log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) { log(LogLevel::INFO, message); }

void Logger::warning(const std::string& message) {
  log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
  log(LogLevel::ERROR, message);
}

std::string Logger::levelToString(LogLevel level) const {
  switch (level) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    default:
      return "UNKNOWN";
  }
}

std::string Logger::getCurrentTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;

  std::ostringstream oss;
  oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
  oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

  return oss.str();
}

bool Logger::shouldLog(LogLevel level) const {
  return static_cast<int>(level) >= static_cast<int>(minLogLevel_);
}

}  // namespace s21
