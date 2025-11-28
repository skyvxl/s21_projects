#include "fsm.h"

#include <string.h>

// Глобальная переменная состояния FSM
FSMState_t fsm_state = {.current_state = STATE_START,
                        .current_game_type = GAME_TETRIS,
                        .game_callbacks = NULL,
                        .last_update_time = {0, 0},
                        .initialized = false};

// Основные функции API (соответствуют спецификации)
void userInput(UserAction_t action, bool hold) {
  if (!fsm_state.initialized || !fsm_state.game_callbacks) {
    return;
  }

  // Делегируем обработку ввода конкретной игре
  if (fsm_state.game_callbacks->handle_input) {
    fsm_state.game_callbacks->handle_input(action, hold);
  }
}

GameInfo_t updateCurrentState(void) {
  if (!fsm_state.initialized || !fsm_state.game_callbacks) {
    GameInfo_t empty_info = {0};
    return empty_info;
  }

  // Обновляем FSM
  fsm_update();

  // Получаем текущее состояние игры
  if (fsm_state.game_callbacks->get_game_info) {
    return fsm_state.game_callbacks->get_game_info();
  }

  GameInfo_t empty_info = {0};
  return empty_info;
}

GameInfo_t getCurrentGameInfo(void) {
  if (!fsm_state.initialized || !fsm_state.game_callbacks) {
    GameInfo_t empty_info = {0};
    return empty_info;
  }

  // Получаем текущее состояние игры без обновления
  if (fsm_state.game_callbacks->get_game_info) {
    return fsm_state.game_callbacks->get_game_info();
  }

  GameInfo_t empty_info = {0};
  return empty_info;
}

// Функции управления FSM
void fsm_init(GameType_t game_type, GameCallbacks_t* callbacks) {
  if (!callbacks) return;

  fsm_state.current_game_type = game_type;
  fsm_state.game_callbacks = callbacks;
  fsm_state.current_state = STATE_START;
  fsm_state.initialized = true;

  fsm_reset_timer();

  // Инициализируем игру
  if (callbacks->init_game) {
    callbacks->init_game();
  }
}

void fsm_cleanup(void) {
  if (fsm_state.initialized && fsm_state.game_callbacks) {
    if (fsm_state.game_callbacks->cleanup_game) {
      fsm_state.game_callbacks->cleanup_game();
    }
  }

  fsm_state.initialized = false;
  fsm_state.game_callbacks = NULL;
}

void fsm_set_state(GameState_t new_state) {
  fsm_state.current_state = new_state;

  // Синхронизируем состояние с игрой
  if (fsm_state.game_callbacks && fsm_state.game_callbacks->set_current_state) {
    fsm_state.game_callbacks->set_current_state(new_state);
  }
}

GameState_t fsm_get_state(void) {
  // Синхронизируем состояние с игрой
  if (fsm_state.game_callbacks && fsm_state.game_callbacks->get_current_state) {
    fsm_state.current_state = fsm_state.game_callbacks->get_current_state();
  }

  return fsm_state.current_state;
}

void fsm_update(void) {
  if (!fsm_state.initialized || !fsm_state.game_callbacks) {
    return;
  }

  // Получаем текущее состояние от игры
  GameState_t current_state = fsm_get_state();

  // Выполняем логику конечного автомата
  switch (current_state) {
    case STATE_START:
      if (fsm_state.game_callbacks->handle_start_state) {
        fsm_state.game_callbacks->handle_start_state();
      }
      break;
    case STATE_SPAWN:
      if (fsm_state.game_callbacks->handle_spawn_state) {
        fsm_state.game_callbacks->handle_spawn_state();
      }
      break;
    case STATE_MOVING:
      if (fsm_state.game_callbacks->handle_moving_state) {
        fsm_state.game_callbacks->handle_moving_state();
      }
      break;
    case STATE_SHIFTING:
      if (fsm_state.game_callbacks->handle_shifting_state) {
        fsm_state.game_callbacks->handle_shifting_state();
      }
      break;
    case STATE_ATTACHING:
      if (fsm_state.game_callbacks->handle_attaching_state) {
        fsm_state.game_callbacks->handle_attaching_state();
      }
      break;
    case STATE_GAME_OVER:
      if (fsm_state.game_callbacks->handle_game_over_state) {
        fsm_state.game_callbacks->handle_game_over_state();
      }
      break;
    case STATE_PAUSE:
      if (fsm_state.game_callbacks->handle_pause_state) {
        fsm_state.game_callbacks->handle_pause_state();
      }
      break;
  }
}

// Функции для переходов между состояниями
void fsm_transition_to_start(void) { fsm_set_state(STATE_START); }

void fsm_transition_to_spawn(void) { fsm_set_state(STATE_SPAWN); }

void fsm_transition_to_moving(void) { fsm_set_state(STATE_MOVING); }

void fsm_transition_to_shifting(void) { fsm_set_state(STATE_SHIFTING); }

void fsm_transition_to_attaching(void) { fsm_set_state(STATE_ATTACHING); }

void fsm_transition_to_game_over(void) { fsm_set_state(STATE_GAME_OVER); }

void fsm_transition_to_pause(void) { fsm_set_state(STATE_PAUSE); }

// Утилиты для работы с временем
void fsm_reset_timer(void) { gettimeofday(&fsm_state.last_update_time, NULL); }

long fsm_get_elapsed_time(void) {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  return (current_time.tv_sec - fsm_state.last_update_time.tv_sec) * 1000000 +
         (current_time.tv_usec - fsm_state.last_update_time.tv_usec);
}

// Утилиты для работы с матрицами (общие для всех игр)
int** allocate_matrix(int rows, int cols) {
  int** matrix = (int**)calloc(rows, sizeof(int*));
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      matrix[i] = (int*)calloc(cols, sizeof(int));
      if (!matrix[i]) {
        // Освобождаем уже выделенную память при ошибке
        for (int j = 0; j < i; j++) {
          free(matrix[j]);
        }
        free(matrix);
        return NULL;
      }
    }
  }
  return matrix;
}

void free_matrix(int** matrix, int rows) {
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

void clear_matrix(int** matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    memset(matrix[i], 0, cols * sizeof(int));
  }
}
