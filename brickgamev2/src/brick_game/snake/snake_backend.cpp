#include "snake_backend.h"

// Инициализируем FSM со змейкой при первом вызове
static bool fsm_initialized = false;

static void ensure_fsm_initialized(void) {
  if (!fsm_initialized) {
    fsm_init(GAME_SNAKE, get_snake_callbacks());
    fsm_initialized = true;
  }
}

// Функции инициализации (совместимость)
void snake_init_game(void) { ensure_fsm_initialized(); }

void snake_cleanup_game(void) {
  if (fsm_initialized) {
    fsm_cleanup();
    fsm_initialized = false;
  }
}

void snake_reset_game(void) {
  ensure_fsm_initialized();
  if (fsm_state.game_callbacks && fsm_state.game_callbacks->reset_game) {
    fsm_state.game_callbacks->reset_game();
  }
}

// Функции управления игрой (совместимость)
void snake_start_game(void) {
  ensure_fsm_initialized();
  fsm_set_state(STATE_MOVING);
}

void snake_pause_game(void) {
  ensure_fsm_initialized();
  GameState_t current_state = fsm_get_state();
  if (current_state != STATE_START && current_state != STATE_GAME_OVER) {
    if (current_state == STATE_PAUSE) {
      fsm_set_state(STATE_MOVING);
    } else {
      fsm_set_state(STATE_PAUSE);
    }
  }
}

void snake_terminate_game(void) {
  ensure_fsm_initialized();
  fsm_set_state(STATE_GAME_OVER);
}

// Дополнительные функции для совместимости
GameState_t snake_get_current_game_state(void) {
  return snake_get_current_state();
}
