#include <QApplication>

#include "./utils/logger.h"
#include "./views/mainWindow.h"

int main(int argc, char* argv[]) {
  // Инициализация логгера
  s21::Logger& logger = s21::Logger::getInstance();
  logger.setLogFile("3dviewer.log");
  logger.setLogLevel(s21::LogLevel::DEBUG);
  logger.setConsoleOutput(true);

  logger.info("═══════════════════════════════════════════════════════");
  logger.info("3D Viewer v2.0 - Запуск приложения");
  logger.info("═══════════════════════════════════════════════════════");

  QApplication app(argc, argv);
  logger.info("Qt Application инициализирован");

  s21::MainWindow mainWindow;
  logger.info("MainWindow создан и готов к отображению");

  int exitCode = app.exec();

  logger.info("Приложение завершено с кодом: " + std::to_string(exitCode));
  logger.info("═══════════════════════════════════════════════════════");

  return exitCode;
}