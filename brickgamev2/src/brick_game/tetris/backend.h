#ifndef __BACKEND_H__
#define __BACKEND_H__

// Единый интерфейс для подключения к FSM и Tetris
#include "../fsm/fsm.h"
#include "tetris.h"

// Функции инициализации (совместимость)
void init_game(void);
void cleanup_game(void);
void reset_game(void);

// Функции управления игрой (совместимость)
void start_game(void);
void pause_game(void);
void terminate_game(void);

// Дополнительные функции для совместимости с C++
GameState_t getCurrentGameState(void);
Tetromino_t getCurrentTetromino(void);

#endif  // __BACKEND_H__
