#ifndef __SNAKE_BACKEND_H__
#define __SNAKE_BACKEND_H__

// Единый интерфейс для подключения к FSM и Snake
#include "../fsm/fsm.h"
#include "snake.h"  // IWYU pragma: keep

// Функции инициализации (совместимость)
void snake_init_game(void);
void snake_cleanup_game(void);
void snake_reset_game(void);

// Функции управления игрой (совместимость)
void snake_start_game(void);
void snake_pause_game(void);
void snake_terminate_game(void);

// Дополнительные функции для совместимости
GameState_t snake_get_current_game_state(void);

#endif  // __SNAKE_BACKEND_H__
