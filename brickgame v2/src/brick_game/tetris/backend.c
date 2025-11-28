#include "backend.h"

/// Слой совместимости для существующего API

// Инициализируем FSM с тетрисом при первом вызове
static bool fsm_initialized = false;

static void ensure_fsm_initialized(void) {
  if (!fsm_initialized) {
    fsm_init(GAME_TETRIS, get_tetris_callbacks());
    fsm_initialized = true;
  }
}

// Функции инициализации (совместимость)
void init_game(void) { ensure_fsm_initialized(); }

void cleanup_game(void) {
  if (fsm_initialized) {
    fsm_cleanup();
    fsm_initialized = false;
  }
}

void reset_game(void) {
  ensure_fsm_initialized();
  if (fsm_state.game_callbacks && fsm_state.game_callbacks->reset_game) {
    fsm_state.game_callbacks->reset_game();
  }
}

// Функции управления игрой (совместимость)
void start_game(void) {
  ensure_fsm_initialized();
  fsm_set_state(STATE_SPAWN);
}

void pause_game(void) {
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

void terminate_game(void) {
  ensure_fsm_initialized();
  fsm_set_state(STATE_GAME_OVER);
}
