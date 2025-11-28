#include "gameController.h"

#include "../../brick_game/snake/snake.h"
#include "../../brick_game/tetris/tetris.h"

namespace s21 {

GameController::GameController()
    : current_type_(GameType::TETRIS), initialized_(false) {}

GameController::~GameController() { Cleanup(); }

void GameController::Initialize(GameType type) {
  if (initialized_) {
    Cleanup();
  }

  current_type_ = type;

  GameCallbacks_t* callbacks = nullptr;
  GameType_t game_type = GAME_TETRIS;

  switch (current_type_) {
    case GameType::TETRIS:
      callbacks = get_tetris_callbacks();
      game_type = GAME_TETRIS;
      break;
    case GameType::SNAKE:
      callbacks = get_snake_callbacks();
      game_type = GAME_SNAKE;
      break;
  }

  if (!callbacks) {
    initialized_ = false;
    return;
  }

  fsm_init(game_type, callbacks);
  initialized_ = true;
}

void GameController::HandleAction(UserAction_t action, bool hold) {
  if (!initialized_) return;
  userInput(action, hold);
}

GameInfo_t GameController::Update() {
  if (!initialized_) {
    GameInfo_t empty = {};
    return empty;
  }
  return updateCurrentState();
}

GameInfo_t GameController::CurrentInfo() const {
  if (!initialized_) {
    GameInfo_t empty = {};
    return empty;
  }
  return getCurrentGameInfo();
}

GameState_t GameController::State() const {
  if (!initialized_) {
    return STATE_START;
  }
  return fsm_get_state();
}

Tetromino_t GameController::CurrentTetromino() const {
  if (current_type_ == GameType::TETRIS && initialized_) {
    return getCurrentTetromino();
  }
  return {};
}

void GameController::Cleanup() {
  if (!initialized_) return;
  fsm_cleanup();
  initialized_ = false;
}

}  // namespace s21
