#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QApplication>
#include <QColor>
#include <QDialog>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QRect>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "gameController.h"

// UI константы
#define CELL_SIZE 25
#define GAME_FIELD_WIDTH (FIELD_WIDTH * CELL_SIZE)
#define GAME_FIELD_HEIGHT (FIELD_HEIGHT * CELL_SIZE)
#define INFO_PANEL_WIDTH 250
#define NEXT_FIELD_DISPLAY_SIZE (NEXT_FIELD_SIZE * 22)
#define WINDOW_MARGIN 20

namespace s21 {

// Кастомный виджет для отрисовки игрового поля
class GameFieldWidget : public QWidget {
  Q_OBJECT

 public:
  explicit GameFieldWidget(QWidget* parent = nullptr);
  void setGameInfo(const GameInfo_t& info);
  void setOverlayMessage(const QString& message);
  void clearOverlayMessage();
  void setGameType(GameType type);
  void setController(GameController* controller);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  GameInfo_t game_info_;
  QString overlay_message_;
  bool has_overlay_;
  GameType game_type_;
  GameController* controller_;

  void drawField(QPainter& painter);
  void drawNext(QPainter& painter);
  void drawOverlay(QPainter& painter);
  QColor getCellColor(int value);
};

// Кастомный виджет для отображения Next поля
class NextFieldWidget : public QWidget {
  Q_OBJECT

 public:
  explicit NextFieldWidget(QWidget* parent = nullptr);
  void setGameInfo(const GameInfo_t& info);

 protected:
  void paintEvent(QPaintEvent* event) override;

 private:
  GameInfo_t game_info_;
  QColor getCellColor(int value);
};

// Кастомный виджет для информационной панели
class InfoPanelWidget : public QWidget {
  Q_OBJECT

 public:
  explicit InfoPanelWidget(QWidget* parent = nullptr);
  void updateInfo(const GameInfo_t& info);

 private:
  QLabel* next_label_;
  NextFieldWidget* next_preview_;
  QLabel* score_label_;
  QLabel* high_score_label_;
  QLabel* level_label_;
  QLabel* speed_label_;
  QLabel* controls_label_;

  void setupLayout();
};

// Диалог выбора игры
class GameSelectionDialog : public QDialog {
  Q_OBJECT

 public:
  explicit GameSelectionDialog(QWidget* parent = nullptr);
  GameType getSelectedGame() const;
  bool exitRequested() const;

 private slots:
  void selectTetris();
  void selectSnake();
  void selectQuit();

 private:
  GameType selected_game_;
  bool exit_requested_;
};

// Главное окно
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void keyReleaseEvent(QKeyEvent* event) override;
  void closeEvent(QCloseEvent* event) override;

 private slots:
  void updateGame();

 private:
  GameFieldWidget* game_field_;
  InfoPanelWidget* info_panel_;
  QTimer* game_timer_;
  GameType current_game_;
  bool quit_confirmed_;
  bool space_pressed_;
  bool game_active_;
  GameController controller_;

  void setupUI();
  void initializeGame(GameType type);
  bool startFromSelection();
  bool promptGameSelection(GameType& selected_game);
  void stopCurrentGame();
  void handleQuitToMenu();
  void handleUserAction(UserAction_t action, bool hold = false);
  void updateDisplay();
  void showGameOverMessage();
  void showPauseMessage();
  void showStartMessage();
  bool confirmQuit();
};

}  // namespace s21

#endif  // __MAINWINDOW_H__
