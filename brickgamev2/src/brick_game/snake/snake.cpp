#include "snake.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <vector>

namespace s21 {

std::filesystem::path ResolveHighScorePath() {
  static std::filesystem::path cached_path;
  if (!cached_path.empty()) return cached_path;

  const std::filesystem::path file_name("snake_high_score.txt");
  std::vector<std::filesystem::path> candidates = {
      std::filesystem::current_path() / file_name,
      std::filesystem::current_path().parent_path() / file_name,
      std::filesystem::current_path().parent_path().parent_path() / file_name};

  if (const char* home_dir = std::getenv("HOME")) {
    std::filesystem::path config =
        std::filesystem::path(home_dir) / ".local" / "share" / "brickgame";
    candidates.push_back(config / file_name);
  }

  for (const auto& candidate : candidates) {
    if (std::filesystem::exists(candidate)) {
      cached_path = candidate;
      return cached_path;
    }
  }

  // Если файл не найден, используем первый путь и гарантируем существование
  cached_path = candidates.front();
  std::filesystem::create_directories(cached_path.parent_path());
  return cached_path;
}

Snake::Snake() : direction(3) {
  body.push_back({FIELD_WIDTH / 2, FIELD_HEIGHT / 2});
  body.push_back({FIELD_WIDTH / 2 - 1, FIELD_HEIGHT / 2});
  body.push_back({FIELD_WIDTH / 2 - 2, FIELD_HEIGHT / 2});
  body.push_back({FIELD_WIDTH / 2 - 3, FIELD_HEIGHT / 2});
}

void Snake::move() {
  Point newHead = body.front();
  switch (direction) {
    case 0:
      newHead.y--;
      break;  // up
    case 1:
      newHead.y++;
      break;  // down
    case 2:
      newHead.x--;
      break;  // left
    case 3:
      newHead.x++;
      break;  // right
  }
  body.push_front(newHead);
  body.pop_back();
}

void Snake::changeDirection(int dx, int dy) {
  int new_dir;
  if (dx == -1 && dy == 0)
    new_dir = 2;
  else if (dx == 1 && dy == 0)
    new_dir = 3;
  else if (dx == 0 && dy == -1)
    new_dir = 0;
  else if (dx == 0 && dy == 1)
    new_dir = 1;
  else
    return;

  // Prevent reverse
  if ((direction == 0 && new_dir == 1) || (direction == 1 && new_dir == 0) ||
      (direction == 2 && new_dir == 3) || (direction == 3 && new_dir == 2)) {
    return;
  }
  direction = new_dir;
}

void Snake::grow() {
  if (body.size() < kSnakeMaxLength) {
    body.push_back(body.back());
  }
}

// Реализация класса Apple
Apple::Apple() : x(0), y(0) {}

void Apple::spawn(int width, int height, const std::vector<Point>& occupied) {
  int max_attempts = width * height * 2;
  for (int attempt = 0; attempt < max_attempts; ++attempt) {
    int nx = std::rand() % width;
    int ny = std::rand() % height;
    Point p{nx, ny};
    if (std::find(occupied.begin(), occupied.end(), p) == occupied.end()) {
      x = nx;
      y = ny;
      return;
    }
  }
  // Fallback
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Point p{x, y};
      if (std::find(occupied.begin(), occupied.end(), p) == occupied.end()) {
        this->x = x;
        this->y = y;
        return;
      }
    }
  }
}

// Реализация класса Grid
Grid::Grid(int width, int height)
    : cells(height, std::vector<int>(width, 0)),
      width_(width),
      height_(height) {}

void Grid::clear() {
  for (auto& row : cells) {
    std::fill(row.begin(), row.end(), 0);
  }
}

int Grid::getCell(int x, int y) const {
  if (x >= 0 && x < width_ && y >= 0 && y < height_) return cells[y][x];
  return 0;
}

void Grid::setCell(int x, int y, int value) {
  if (x >= 0 && x < width_ && y >= 0 && y < height_) cells[y][x] = value;
}

// Реализация класса SnakeModel
SnakeModel::SnakeModel()
    : grid_(FIELD_WIDTH, FIELD_HEIGHT),
      state_(STATE_START),
      score_(0),
      high_score_(0),
      level_(1),
      speed_(1),
      running_(false),
      paused_(false),
      game_over_(false),
      move_interval_(500),
      accelerated_(false) {
  game_info_.field = allocate_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  game_info_.next = allocate_matrix(NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);
  clear_matrix(game_info_.field, FIELD_HEIGHT, FIELD_WIDTH);
  clear_matrix(game_info_.next, NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);
  std::srand(static_cast<unsigned>(std::time(nullptr)));
  loadHighScore();
  reset();
}

SnakeModel::~SnakeModel() {
  free_matrix(game_info_.field, FIELD_HEIGHT);
  free_matrix(game_info_.next, NEXT_FIELD_SIZE);
}

void SnakeModel::reset() {
  score_ = 0;
  level_ = 1;
  speed_ = 1;
  move_interval_ = 500;
  running_ = false;
  paused_ = false;
  game_over_ = false;
  accelerated_ = false;
  state_ = STATE_START;

  grid_.clear();
  clear_matrix(game_info_.field, FIELD_HEIGHT, FIELD_WIDTH);
  clear_matrix(game_info_.next, NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);

  snake_ = Snake();
  std::vector<Point> occupied;
  for (const auto& p : snake_.body) occupied.push_back(p);
  apple_.spawn(grid_.width(), grid_.height(), occupied);

  resetMoveTimer();
}

void SnakeModel::start() {
  if (state_ == STATE_GAME_OVER) {
    reset();
  }
  state_ = STATE_MOVING;
  running_ = true;
  paused_ = false;
  game_over_ = false;
}

void SnakeModel::togglePause() {
  if (state_ == STATE_MOVING) {
    state_ = STATE_PAUSE;
    paused_ = true;
  } else if (state_ == STATE_PAUSE) {
    state_ = STATE_MOVING;
    paused_ = false;
  }
}

void SnakeModel::move() {
  if (!running_ || paused_) return;

  if (!shouldMove()) return;
  snake_.move();

  // Check apple
  if (snake_.body.front().x == apple_.x && snake_.body.front().y == apple_.y) {
    snake_.grow();
    score_ += 10;
    level_ = std::min(10, 1 + score_ / 20);
    speed_ = level_;
    move_interval_ = 500 - (level_ - 1) * 40;
    if (move_interval_ < 100) move_interval_ = 100;

    if (score_ > high_score_) {
      high_score_ = score_;
      saveHighScore();
    }

    // Check for victory condition at 200 points
    if (score_ >= 200) {
      game_over_ = true;
      state_ = STATE_GAME_OVER;
      saveHighScore();
      return;
    }

    std::vector<Point> occupied;
    for (const auto& p : snake_.body) occupied.push_back(p);
    apple_.spawn(grid_.width(), grid_.height(), occupied);
  }

  handleCollisions();
  resetMoveTimer();
}

void SnakeModel::changeDirection(int dx, int dy) {
  snake_.changeDirection(dx, dy);
}

void SnakeModel::handleCollisions() {
  const Point& head = snake_.body.front();

  // Wall
  if (head.x < 0 || head.x >= grid_.width() || head.y < 0 ||
      head.y >= grid_.height()) {
    game_over_ = true;
    state_ = STATE_GAME_OVER;
    saveHighScore();
    return;
  }

  // Self
  for (size_t i = 1; i < snake_.body.size(); ++i) {
    if (head.x == snake_.body[i].x && head.y == snake_.body[i].y) {
      game_over_ = true;
      state_ = STATE_GAME_OVER;
      saveHighScore();
      return;
    }
  }
}

void SnakeModel::loadHighScore() {
  std::ifstream file(ResolveHighScorePath());
  if (file.is_open()) {
    file >> high_score_;
    file.close();
  } else {
    high_score_ = 0;
  }
}

void SnakeModel::saveHighScore() {
  const auto path = ResolveHighScorePath();
  std::filesystem::create_directories(path.parent_path());
  std::ofstream file(path);
  if (!file.is_open()) return;
  file << high_score_;
  file.close();
}

bool SnakeModel::shouldMove() const {
  int interval = accelerated_ ? move_interval_ / 2 : move_interval_;
  long elapsed = getElapsedTime(&last_move_time_);
  long required = interval * 1000;
  return elapsed >= required;  // microseconds
}

void SnakeModel::resetMoveTimer() { gettimeofday(&last_move_time_, nullptr); }

long SnakeModel::getElapsedTime(const struct timeval* last_time) const {
  struct timeval current_time;
  gettimeofday(&current_time, nullptr);
  return (current_time.tv_sec - last_time->tv_sec) * 1000000 +
         (current_time.tv_usec - last_time->tv_usec);
}

void SnakeModel::setAccelerated(bool acc) { accelerated_ = acc; }

GameInfo_t SnakeModel::getGameInfo() {
  clear_matrix(game_info_.field, FIELD_HEIGHT, FIELD_WIDTH);

  // Draw snake body
  for (size_t i = 1; i < snake_.body.size(); ++i) {
    const Point& p = snake_.body[i];
    if (p.x >= 0 && p.x < FIELD_WIDTH && p.y >= 0 && p.y < FIELD_HEIGHT) {
      game_info_.field[p.y][p.x] = 1;  // body
    }
  }

  // Head
  const Point& head = snake_.body.front();
  if (head.x >= 0 && head.x < FIELD_WIDTH && head.y >= 0 &&
      head.y < FIELD_HEIGHT) {
    game_info_.field[head.y][head.x] = 3;  // head
  }

  // Apple
  if (apple_.x >= 0 && apple_.x < FIELD_WIDTH && apple_.y >= 0 &&
      apple_.y < FIELD_HEIGHT) {
    game_info_.field[apple_.y][apple_.x] = 2;  // apple
  }

  game_info_.score = score_;
  game_info_.high_score = high_score_;
  game_info_.level = level_;
  game_info_.speed = speed_;
  game_info_.pause = paused_ ? 1 : 0;

  return game_info_;
}

SnakeModel* snake_model = nullptr;

void SnakeModelTestAccessor::ForceReadyForMove(SnakeModel& model) {
  model.last_move_time_.tv_sec -= 1;
}

void SnakeModelTestAccessor::SetApplePosition(SnakeModel& model,
                                              const Point& apple_pos) {
  model.apple_.x = apple_pos.x;
  model.apple_.y = apple_pos.y;
}

void SnakeModelTestAccessor::SetSnakeBody(SnakeModel& model,
                                          const std::deque<Point>& body) {
  model.snake_.body = body;
}

void SnakeModelTestAccessor::SetState(SnakeModel& model, GameState_t state) {
  model.state_ = state;
}

void SnakeModelTestAccessor::SetRunning(SnakeModel& model, bool running) {
  model.running_ = running;
}

void SnakeModelTestAccessor::SetPaused(SnakeModel& model, bool paused) {
  model.paused_ = paused;
}

void SnakeModelTestAccessor::SetDirection(SnakeModel& model, int direction) {
  model.snake_.direction = direction;
}

bool SnakeModelTestAccessor::IsAccelerated(const SnakeModel& model) {
  return model.accelerated_;
}

}  // namespace s21

// Обертки для FSM
extern "C" {

void snake_init_game(void) {
  if (s21::snake_model) delete s21::snake_model;
  s21::snake_model = new s21::SnakeModel();
}

void snake_cleanup_game(void) {
  if (s21::snake_model) {
    delete s21::snake_model;
    s21::snake_model = nullptr;
  }
}

void snake_reset_game(void) {
  if (s21::snake_model) s21::snake_model->reset();
}

void snake_handle_input(UserAction_t action, bool hold) {
  if (!s21::snake_model) return;
  (void)hold;

  switch (action) {
    case Start:
      if (s21::snake_model->getCurrentState() == STATE_START) {
        s21::snake_model->start();
      }
      break;
    case Pause:
      s21::snake_model->togglePause();
      break;
    case Terminate:
      s21::snake_model->setCurrentState(STATE_GAME_OVER);
      break;
    case Left:
      if (s21::snake_model->getCurrentState() == STATE_MOVING) {
        s21::snake_model->changeDirection(-1, 0);
      }
      break;
    case Right:
      if (s21::snake_model->getCurrentState() == STATE_MOVING) {
        s21::snake_model->changeDirection(1, 0);
      }
      break;
    case Up:
      if (s21::snake_model->getCurrentState() == STATE_MOVING) {
        s21::snake_model->changeDirection(0, -1);
      }
      break;
    case Down:
      if (s21::snake_model->getCurrentState() == STATE_MOVING) {
        s21::snake_model->changeDirection(0, 1);
      }
      break;
    case Action:
      if (s21::snake_model->getCurrentState() == STATE_START) {
        s21::snake_model->start();
      } else if (s21::snake_model->getCurrentState() == STATE_GAME_OVER) {
        s21::snake_model->reset();
        s21::snake_model->start();
      } else {
        if (hold) {
          s21::snake_model->setAccelerated(true);
        } else {
          s21::snake_model->setAccelerated(false);
        }
      }
      break;
    default:
      break;
  }
}

GameInfo_t snake_get_game_info(void) {
  if (s21::snake_model) return s21::snake_model->getGameInfo();
  GameInfo_t empty = {};
  return empty;
}

GameState_t snake_get_current_state(void) {
  if (s21::snake_model) return s21::snake_model->getCurrentState();
  return STATE_START;
}

void snake_set_current_state(GameState_t state) {
  if (s21::snake_model) s21::snake_model->setCurrentState(state);
}

void snake_handle_start_state(void) {}

void snake_handle_moving_state(void) {
  if (s21::snake_model) {
    s21::snake_model->move();
  }
}

void snake_handle_pause_state(void) {}

void snake_handle_game_over_state(void) {}

GameCallbacks_t* get_snake_callbacks(void) {
  static GameCallbacks_t callbacks = {snake_init_game,
                                      snake_cleanup_game,
                                      snake_reset_game,
                                      snake_handle_input,
                                      snake_handle_start_state,
                                      nullptr,  // spawn
                                      snake_handle_moving_state,
                                      nullptr,  // shifting
                                      nullptr,  // attaching
                                      snake_handle_game_over_state,
                                      snake_handle_pause_state,
                                      snake_get_game_info,
                                      snake_get_current_state,
                                      snake_set_current_state};
  return &callbacks;
}

GameState_t getSnakeCurrentGameState(void) { return snake_get_current_state(); }

}  // extern "C"
