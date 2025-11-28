#include "mainWindow.h"

namespace s21 {

// GameFieldWidget implementation
GameFieldWidget::GameFieldWidget(QWidget* parent)
    : QWidget(parent),
      has_overlay_(false),
      game_type_(GameType::TETRIS),
      controller_(nullptr) {
  setFixedSize(GAME_FIELD_WIDTH, GAME_FIELD_HEIGHT);
  setStyleSheet("background-color: black; border: 2px solid #333;");

  // Инициализация game_info_ нулями
  game_info_ = {};
}

void GameFieldWidget::setGameType(GameType type) { game_type_ = type; }

void GameFieldWidget::setController(GameController* controller) {
  controller_ = controller;
}

void GameFieldWidget::setGameInfo(const GameInfo_t& info) {
  game_info_ = info;
  update();
}

void GameFieldWidget::setOverlayMessage(const QString& message) {
  overlay_message_ = message;
  has_overlay_ = true;
  update();
}

void GameFieldWidget::clearOverlayMessage() {
  has_overlay_ = false;
  update();
}

void GameFieldWidget::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  drawField(painter);

  if (has_overlay_) {
    drawOverlay(painter);
  }
}

void GameFieldWidget::drawField(QPainter& painter) {
  // Отрисовка игрового поля
  if (!game_info_.field) return;

  // Создаем временную копию поля для отображения с текущей фигурой
  int** display_field = new int*[FIELD_HEIGHT];
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    display_field[i] = new int[FIELD_WIDTH];
    for (int j = 0; j < FIELD_WIDTH; ++j) {
      display_field[i][j] = game_info_.field[i][j];
    }
  }

  // Накладываем текущую фигуру (если игра активна и Tetris)
  if (game_type_ == GameType::TETRIS && controller_) {
    GameState_t state = controller_->State();
    if (state == STATE_MOVING || state == STATE_SHIFTING) {
      Tetromino_t current = controller_->CurrentTetromino();

      for (int y = 0; y < TETROMINO_SIZE; y++) {
        for (int x = 0; x < TETROMINO_SIZE; x++) {
          if (current.shape[y][x]) {
            int field_x = current.x + x;
            int field_y = current.y + y;

            if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
                field_x < FIELD_WIDTH) {
              display_field[field_y][field_x] = current.type + 1;
            }
          }
        }
      }
    }
  }

  // Отрисовка поля
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      QRect cellRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);

      int cellValue = display_field[y][x];
      QColor cellColor = getCellColor(cellValue);

      painter.fillRect(cellRect, cellColor);
    }
  }

  // Освобождаем память
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    delete[] display_field[i];
  }
  delete[] display_field;
}

void GameFieldWidget::drawOverlay(QPainter& painter) {
  // Полупрозрачный фон для оверлея
  QRect widget_rect = this->rect();
  painter.fillRect(widget_rect, QColor(0, 0, 0, 180));

  // Настройка шрифта для сообщения
  QFont font = painter.font();
  font.setPointSize(24);
  font.setBold(true);
  painter.setFont(font);
  painter.setPen(Qt::white);

  // Отрисовка текста по центру
  painter.drawText(widget_rect, Qt::AlignCenter, overlay_message_);
}

QColor GameFieldWidget::getCellColor(int value) {
  if (value == 0) {
    return QColor(10, 10, 10);  // Почти черный для пустых клеток
  }

  // Разные цвета для Snake и Tetris
  if (game_type_ == GameType::SNAKE) {
    switch (value) {
      case 1:
        return QColor(0, 255, 0);  // Green for snake body
      case 2:
        return QColor(255, 0, 0);  // Red for apple
      case 3:
        return QColor(0, 255, 255);  // Cyan for snake head
      default:
        return QColor(255, 255, 255);  // White для остальных
    }
  }

  // Цвета для Tetris
  switch (value) {
    case 1:
      return QColor(0, 255, 255);  // Cyan (I)
    case 2:
      return QColor(255, 255, 0);  // Yellow (O)
    case 3:
      return QColor(128, 0, 128);  // Purple (T)
    case 4:
      return QColor(0, 255, 0);  // Green (S)
    case 5:
      return QColor(255, 0, 0);  // Red (Z)
    case 6:
      return QColor(0, 0, 255);  // Blue (J)
    case 7:
      return QColor(255, 165, 0);  // Orange (L)
    default:
      return QColor(255, 255, 255);  // White для остальных
  }
}

// NextFieldWidget implementation
NextFieldWidget::NextFieldWidget(QWidget* parent) : QWidget(parent) {
  setFixedSize(NEXT_FIELD_DISPLAY_SIZE, NEXT_FIELD_DISPLAY_SIZE);
  setStyleSheet("background-color: black; border: 1px solid #333;");

  // Инициализация game_info_ нулями
  game_info_ = {};
}

void NextFieldWidget::setGameInfo(const GameInfo_t& info) {
  game_info_ = info;
  update();
}

void NextFieldWidget::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // Отрисовка Next поля
  if (!game_info_.next) return;

  for (int y = 0; y < NEXT_FIELD_SIZE; ++y) {
    for (int x = 0; x < NEXT_FIELD_SIZE; ++x) {
      QRect cellRect(x * 22, y * 22, 22, 22);

      int cellValue = game_info_.next[y][x];
      QColor cellColor = getCellColor(cellValue);

      painter.fillRect(cellRect, cellColor);
    }
  }
}

QColor NextFieldWidget::getCellColor(int value) {
  if (value == 0) {
    return QColor(10, 10, 10);  // Почти черный для пустых клеток
  }

  // Цвета для различных типов тетромино
  switch (value) {
    case 1:
      return QColor(0, 255, 255);  // Cyan (I)
    case 2:
      return QColor(255, 255, 0);  // Yellow (O)
    case 3:
      return QColor(128, 0, 128);  // Purple (T)
    case 4:
      return QColor(0, 255, 0);  // Green (S)
    case 5:
      return QColor(255, 0, 0);  // Red (Z)
    case 6:
      return QColor(0, 0, 255);  // Blue (J)
    case 7:
      return QColor(255, 165, 0);  // Orange (L)
    default:
      return QColor(255, 255, 255);  // White для остальных
  }
}

// InfoPanelWidget implementation
InfoPanelWidget::InfoPanelWidget(QWidget* parent) : QWidget(parent) {
  setFixedWidth(INFO_PANEL_WIDTH);
  setStyleSheet(
      "background-color: black; color: white; border: 2px solid #333;");
  setupLayout();
}

void InfoPanelWidget::setupLayout() {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setSpacing(6);
  layout->setContentsMargins(12, 12, 12, 12);

  // Заголовок INFO
  QLabel* title = new QLabel("INFO");
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet(
      "font-size: 18px; font-weight: bold; color: white; "
      "border: 2px solid #555; padding: 8px; margin-bottom: 8px; "
      "background-color: #333;");
  layout->addWidget(title);

  // Next section
  next_label_ = new QLabel("Next:");
  next_label_->setStyleSheet(
      "font-size: 14px; font-weight: bold; color: white; "
      "padding: 4px; margin-bottom: 4px;");
  layout->addWidget(next_label_);

  // Место для отображения следующей фигуры
  next_preview_ = new NextFieldWidget();
  layout->addWidget(next_preview_);

  layout->addSpacing(8);

  // Score information
  score_label_ = new QLabel("Score: 0");
  score_label_->setStyleSheet(
      "font-size: 14px; color: white; border: 2px solid #555; padding: 6px; "
      "background-color: #222;");
  layout->addWidget(score_label_);

  high_score_label_ = new QLabel("High: 0");
  high_score_label_->setStyleSheet(
      "font-size: 14px; color: white; border: 2px solid #555; padding: 6px; "
      "background-color: #222;");
  layout->addWidget(high_score_label_);

  level_label_ = new QLabel("Level: 1");
  level_label_->setStyleSheet(
      "font-size: 14px; color: white; border: 2px solid #555; padding: 6px; "
      "background-color: #222;");
  layout->addWidget(level_label_);

  speed_label_ = new QLabel("Speed: 1000");
  speed_label_->setStyleSheet(
      "font-size: 14px; color: white; border: 2px solid #555; padding: 6px; "
      "background-color: #222;");
  layout->addWidget(speed_label_);

  layout->addSpacing(12);

  // Controls section
  QLabel* controls_title = new QLabel("Controls:");
  controls_title->setStyleSheet(
      "font-size: 14px; font-weight: bold; color: white; "
      "border: 2px solid #555; padding: 6px; background-color: #333;");
  layout->addWidget(controls_title);

  QStringList controls = {"↑↓←→ Move", "Space Action", "P Pause", "Q Quit"};

  for (const QString& control : controls) {
    QLabel* control_label = new QLabel(control);
    control_label->setStyleSheet(
        "font-size: 12px; color: white; border: 1px solid #444; padding: 4px; "
        "background-color: #111;");
    layout->addWidget(control_label);
  }

  layout->addStretch();
}

void InfoPanelWidget::updateInfo(const GameInfo_t& info) {
  score_label_->setText(QString("Score: %1").arg(info.score));
  high_score_label_->setText(QString("High: %1").arg(info.high_score));
  level_label_->setText(QString("Level: %1").arg(info.level));
  speed_label_->setText(QString("Speed: %1").arg(info.speed));

  // Обновляем Next поле
  next_preview_->setGameInfo(info);
}

// GameSelectionDialog implementation
GameSelectionDialog::GameSelectionDialog(QWidget* parent)
    : QDialog(parent),
      selected_game_(GameType::TETRIS),
      exit_requested_(false) {
  setWindowTitle("Select Game");
  setModal(true);
  setFixedSize(300, 200);

  QVBoxLayout* layout = new QVBoxLayout(this);

  QLabel* title = new QLabel("Choose a game:");
  title->setAlignment(Qt::AlignCenter);
  title->setStyleSheet("font-size: 16px; font-weight: bold; margin: 20px;");
  layout->addWidget(title);

  QPushButton* tetris_btn = new QPushButton("Tetris");
  tetris_btn->setStyleSheet("QPushButton { font-size: 14px; padding: 10px; }");
  connect(tetris_btn, &QPushButton::clicked, this,
          &GameSelectionDialog::selectTetris);
  layout->addWidget(tetris_btn);

  QPushButton* snake_btn = new QPushButton("Snake");
  snake_btn->setStyleSheet("QPushButton { font-size: 14px; padding: 10px; }");
  connect(snake_btn, &QPushButton::clicked, this,
          &GameSelectionDialog::selectSnake);
  layout->addWidget(snake_btn);

  QPushButton* quit_btn = new QPushButton("Quit");
  quit_btn->setStyleSheet("QPushButton { font-size: 14px; padding: 10px; }");
  connect(quit_btn, &QPushButton::clicked, this,
          &GameSelectionDialog::selectQuit);
  layout->addWidget(quit_btn);
}

GameType GameSelectionDialog::getSelectedGame() const { return selected_game_; }

bool GameSelectionDialog::exitRequested() const { return exit_requested_; }

void GameSelectionDialog::selectTetris() {
  selected_game_ = GameType::TETRIS;
  exit_requested_ = false;
  accept();
}

void GameSelectionDialog::selectSnake() {
  selected_game_ = GameType::SNAKE;
  exit_requested_ = false;
  accept();
}

void GameSelectionDialog::selectQuit() {
  exit_requested_ = true;
  accept();
}

// MainWindow implementation
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      current_game_(GameType::TETRIS),
      quit_confirmed_(false),
      space_pressed_(false),
      game_active_(false) {
  setupUI();
  if (!startFromSelection()) {
    QTimer::singleShot(0, this, &MainWindow::close);
  }
}

void MainWindow::setupUI() {
  setWindowTitle("BrickGame v2.0 by Sandersm");
  setFixedSize(GAME_FIELD_WIDTH + INFO_PANEL_WIDTH + WINDOW_MARGIN * 3,
               GAME_FIELD_HEIGHT + WINDOW_MARGIN * 2);

  // Центральный виджет с горизонтальным layout
  QWidget* central_widget = new QWidget();
  setCentralWidget(central_widget);

  QHBoxLayout* main_layout = new QHBoxLayout(central_widget);
  main_layout->setSpacing(WINDOW_MARGIN);
  main_layout->setContentsMargins(WINDOW_MARGIN, WINDOW_MARGIN, WINDOW_MARGIN,
                                  WINDOW_MARGIN);

  // Игровое поле
  game_field_ = new GameFieldWidget();
  game_field_->setController(&controller_);
  main_layout->addWidget(game_field_);

  // Информационная панель
  info_panel_ = new InfoPanelWidget();
  main_layout->addWidget(info_panel_);

  // Таймер для обновления игры
  game_timer_ = new QTimer(this);
  connect(game_timer_, &QTimer::timeout, this, &MainWindow::updateGame);

  // Устанавливаем фокус для получения событий клавиатуры
  setFocusPolicy(Qt::StrongFocus);

  // Стилизация главного окна
  setStyleSheet("QMainWindow { background-color: black; }");
}

bool MainWindow::startFromSelection() {
  GameType selected;
  if (!promptGameSelection(selected)) {
    return false;
  }
  initializeGame(selected);
  return true;
}

bool MainWindow::promptGameSelection(GameType& selected_game) {
  while (true) {
    GameSelectionDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted) {
      continue;
    }

    if (dialog.exitRequested()) {
      if (confirmQuit()) {
        quit_confirmed_ = true;
        return false;
      }
      continue;
    }

    selected_game = dialog.getSelectedGame();
    return true;
  }
}

void MainWindow::initializeGame(GameType type) {
  current_game_ = type;
  controller_.Initialize(type);
  space_pressed_ = false;
  game_active_ = true;

  GameInfo_t info = controller_.CurrentInfo();
  game_field_->setGameType(type);
  game_field_->setGameInfo(info);
  info_panel_->updateInfo(info);

  showStartMessage();
  game_timer_->start(100);
}

void MainWindow::stopCurrentGame() {
  if (game_timer_) {
    game_timer_->stop();
  }

  controller_.Cleanup();
  space_pressed_ = false;
  game_active_ = false;

  GameInfo_t empty = {};
  game_field_->setGameInfo(empty);
  info_panel_->updateInfo(empty);
  game_field_->clearOverlayMessage();
}

void MainWindow::handleQuitToMenu() {
  stopCurrentGame();

  if (!startFromSelection()) {
    close();
  }
}

void MainWindow::updateGame() {
  if (!game_active_) {
    return;
  }

  // Получаем текущее состояние игры
  GameInfo_t info = controller_.Update();

  // Обновляем отображение
  game_field_->setGameInfo(info);
  info_panel_->updateInfo(info);

  // Проверяем состояние игры для отображения сообщений
  updateDisplay();
}

void MainWindow::updateDisplay() {
  if (!game_active_) {
    return;
  }

  GameState_t current_state = controller_.State();

  switch (current_state) {
    case STATE_START:
      showStartMessage();
      break;
    case STATE_GAME_OVER:
      showGameOverMessage();
      break;
    case STATE_PAUSE:
      showPauseMessage();
      break;
    default:
      game_field_->clearOverlayMessage();
      break;
  }
}
void MainWindow::showStartMessage() {
  QString game_name = (current_game_ == GameType::TETRIS) ? "TETRIS" : "SNAKE";
  game_field_->setOverlayMessage(
      QString("%1\n\nPress SPACE to start").arg(game_name));
}

void MainWindow::showGameOverMessage() {
  GameInfo_t info = controller_.CurrentInfo();

  // Check for victory condition in Snake
  if (current_game_ == GameType::SNAKE && info.score >= 200) {
    game_field_->setOverlayMessage(
        "VICTORY!\n\nYou reached 200 points!\n\nPress SPACE to restart\nPress "
        "Q to quit");
  } else {
    game_field_->setOverlayMessage(
        "GAME OVER\n\nPress SPACE to restart\nPress Q to quit");
  }
}

void MainWindow::showPauseMessage() {
  game_field_->setOverlayMessage("PAUSED\n\nPress P to continue");
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
  if (!game_active_) {
    QMainWindow::keyPressEvent(event);
    return;
  }

  UserAction_t action = INVALID_ACTION;

  switch (event->key()) {
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Space:
      action = Action;
      space_pressed_ = true;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    case Qt::Key_Q:
      handleQuitToMenu();
      return;
    default:
      QMainWindow::keyPressEvent(event);
      return;
  }

  if (action != INVALID_ACTION) {
    bool hold = (action == Action && space_pressed_);
    handleUserAction(action, hold);
  }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event) {
  if (!game_active_) {
    QMainWindow::keyReleaseEvent(event);
    return;
  }

  switch (event->key()) {
    case Qt::Key_Space:
      space_pressed_ = false;
      controller_.HandleAction(Action, false);
      break;
    default:
      QMainWindow::keyReleaseEvent(event);
      break;
  }
}

void MainWindow::handleUserAction(UserAction_t action, bool hold) {
  if (!game_active_) {
    return;
  }

  // Отправляем действие в контроллер
  controller_.HandleAction(action, hold);

  // Для действий движения и поворота - мгновенно обновляем дисплей
  if (action == Left || action == Right || action == Up || action == Action) {
    // Получаем обновленное состояние сразу
    GameInfo_t info = controller_.Update();
    game_field_->setGameInfo(info);
    info_panel_->updateInfo(info);
  } else {
    // Для остальных действий - обычное обновление
    updateGame();
  }
}

bool MainWindow::confirmQuit() {
  QMessageBox::StandardButton reply = QMessageBox::question(
      this, "Quit Game", "Are you sure you want to quit?",
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

  return reply == QMessageBox::Yes;
}

void MainWindow::closeEvent(QCloseEvent* event) {
  if (quit_confirmed_ || confirmQuit()) {
    quit_confirmed_ = true;
    stopCurrentGame();
    event->accept();
  } else {
    event->ignore();
  }
}

}  // namespace s21
