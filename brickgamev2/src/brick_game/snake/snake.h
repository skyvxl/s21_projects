#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <sys/time.h>

#include <deque>
#include <vector>

#include "../fsm/fsm.h"

namespace s21 {

inline constexpr int kSnakeMaxLength = 200;

struct Point {
  int x, y;
  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
};

class Snake {
 public:
  std::deque<Point> body;
  int direction;

  Snake();
  void move();
  void changeDirection(int dx, int dy);
  void grow();
};

class Apple {
 public:
  int x, y;

  Apple();
  void spawn(int width, int height, const std::vector<Point>& occupied);
};

class Grid {
 public:
  std::vector<std::vector<int>> cells;

  Grid(int width, int height);
  void clear();
  int getCell(int x, int y) const;
  void setCell(int x, int y, int value);
  int width() const { return width_; }
  int height() const { return height_; }

 private:
  int width_;
  int height_;
};

class SnakeModel;

class SnakeModelTestAccessor {
 public:
  static void ForceReadyForMove(SnakeModel& model);
  static void SetApplePosition(SnakeModel& model, const Point& apple_pos);
  static void SetSnakeBody(SnakeModel& model, const std::deque<Point>& body);
  static void SetState(SnakeModel& model, GameState_t state);
  static void SetRunning(SnakeModel& model, bool running);
  static void SetPaused(SnakeModel& model, bool paused);
  static void SetDirection(SnakeModel& model, int direction);
  static bool IsAccelerated(const SnakeModel& model);
};

class SnakeModel {
 public:
  SnakeModel();
  ~SnakeModel();

  void reset();
  void start();
  void togglePause();
  void move();
  void changeDirection(int dx, int dy);

  bool isRunning() const { return running_; }
  bool isPaused() const { return paused_; }
  bool isGameOver() const { return game_over_; }

  const Grid& getGrid() const { return grid_; }
  int getScore() const { return score_; }
  int getHighScore() const { return high_score_; }
  int getLevel() const { return level_; }
  int getSpeed() const { return speed_; }

  const Snake& getSnake() const { return snake_; }
  const Apple& getApple() const { return apple_; }

  GameInfo_t getGameInfo();
  GameState_t getCurrentState() const { return state_; }
  void setCurrentState(GameState_t state) { state_ = state; }
  void setAccelerated(bool acc);
  bool canMoveNow() const { return shouldMove(); }
  void resetMoveTimer();

 private:
  Grid grid_;
  Snake snake_;
  Apple apple_;
  GameState_t state_;
  GameInfo_t game_info_;
  int score_;
  int high_score_;
  int level_;
  int speed_;
  bool running_;
  bool paused_;
  bool game_over_;
  struct timeval last_move_time_;
  int move_interval_;
  bool accelerated_;

  void handleCollisions();
  void loadHighScore();
  void saveHighScore();
  bool shouldMove() const;
  long getElapsedTime(const struct timeval* last_time) const;

  friend class SnakeModelTestAccessor;
};

extern SnakeModel* snake_model;

}  // namespace s21

// Основные функции для интеграции с FSM (обертки для C API)
extern "C" {
void snake_init_game(void);
void snake_cleanup_game(void);
void snake_reset_game(void);
void snake_handle_input(UserAction_t action, bool hold);
GameInfo_t snake_get_game_info(void);
GameState_t snake_get_current_state(void);
void snake_set_current_state(GameState_t state);

// Обработчики состояний FSM для змейки
void snake_handle_start_state(void);
void snake_handle_moving_state(void);
void snake_handle_pause_state(void);
void snake_handle_game_over_state(void);

// Функция для получения callbacks структуры для змейки
GameCallbacks_t* get_snake_callbacks(void);

// Функции для совместимости
GameState_t getSnakeCurrentGameState(void);
}

#endif  // __SNAKE_H__
