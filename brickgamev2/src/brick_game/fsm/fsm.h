#ifndef __FSM_H__
#define __FSM_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

// Константы игрового поля (общие для всех игр)
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define NEXT_FIELD_SIZE 4

// Пользовательские действия (общие для всех игр)
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  INVALID_ACTION = -1
} UserAction_t;

// Состояния конечного автомата (общие для всех игр)
typedef enum {
  STATE_START,      // Ожидание начала игры
  STATE_SPAWN,      // Создание нового игрового элемента
  STATE_MOVING,     // Основное состояние - движение/ожидание ввода
  STATE_SHIFTING,   // Автоматическое движение/изменение
  STATE_ATTACHING,  // Завершение хода/присоединение
  STATE_GAME_OVER,  // Конец игры
  STATE_PAUSE       // Пауза
} GameState_t;

// Информация о текущем состоянии игры (общая структура для всех игр)
typedef struct {
  int** field;     // Игровое поле 20x10
  int** next;      // Предпросмотр следующего элемента 4x4
  int score;       // Текущий счет
  int high_score;  // Рекорд
  int level;       // Уровень
  int speed;       // Скорость
  int pause;       // Флаг паузы
} GameInfo_t;

// Тип игры
typedef enum { GAME_TETRIS, GAME_SNAKE } GameType_t;

// Структура callback-функций для конкретной игры
typedef struct {
  void (*init_game)(void);
  void (*cleanup_game)(void);
  void (*reset_game)(void);
  void (*handle_input)(UserAction_t action, bool hold);
  void (*handle_start_state)(void);
  void (*handle_spawn_state)(void);
  void (*handle_moving_state)(void);
  void (*handle_shifting_state)(void);
  void (*handle_attaching_state)(void);
  void (*handle_game_over_state)(void);
  void (*handle_pause_state)(void);
  GameInfo_t (*get_game_info)(void);
  GameState_t (*get_current_state)(void);
  void (*set_current_state)(GameState_t state);
} GameCallbacks_t;

// Основная структура состояния FSM
typedef struct {
  GameState_t current_state;
  GameType_t current_game_type;
  GameCallbacks_t* game_callbacks;
  struct timeval last_update_time;
  bool initialized;
} FSMState_t;

// Основные функции FSM API (соответствуют спецификации)
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState(void);
GameInfo_t getCurrentGameInfo(void);

// Функции управления FSM
void fsm_init(GameType_t game_type, GameCallbacks_t* callbacks);
void fsm_cleanup(void);
void fsm_set_state(GameState_t new_state);
GameState_t fsm_get_state(void);
void fsm_update(void);

// Функции для переходов между состояниями
void fsm_transition_to_start(void);
void fsm_transition_to_spawn(void);
void fsm_transition_to_moving(void);
void fsm_transition_to_shifting(void);
void fsm_transition_to_attaching(void);
void fsm_transition_to_game_over(void);
void fsm_transition_to_pause(void);

// Утилиты для работы с временем
void fsm_reset_timer(void);
long fsm_get_elapsed_time(void);

// Утилиты для работы с матрицами (общие для всех игр)
int** allocate_matrix(int rows, int cols);
void free_matrix(int** matrix, int rows);
void clear_matrix(int** matrix, int rows, int cols);

// Глобальная переменная состояния FSM
extern FSMState_t fsm_state;

#ifdef __cplusplus
}
#endif

#endif  // __FSM_H__
