#ifndef S21_BRICKGAME_GAME_CONTROLLER_H_
#define S21_BRICKGAME_GAME_CONTROLLER_H_

#include "../../brick_game/fsm/fsm.h"
#include "../../brick_game/tetris/tetris.h"

namespace s21 {

enum class GameType { TETRIS, SNAKE };

class GameController {
 public:
  GameController();
  ~GameController();

  void Initialize(GameType type);
  void HandleAction(UserAction_t action, bool hold);
  GameInfo_t Update();
  GameInfo_t CurrentInfo() const;
  GameState_t State() const;
  Tetromino_t CurrentTetromino() const;
  GameType CurrentGame() const { return current_type_; }
  void Cleanup();

 private:
  GameType current_type_;
  bool initialized_;
};

}  // namespace s21

#endif  // S21_BRICKGAME_GAME_CONTROLLER_H_
