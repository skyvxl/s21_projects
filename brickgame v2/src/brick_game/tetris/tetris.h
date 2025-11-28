#ifndef __TETRIS_H__
#define __TETRIS_H__

#include "../fsm/fsm.h"
#include "tetromino_shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Константы тетриса
#define TETROMINO_SIZE 4
#define TETROMINO_TYPES 7

// Структура тетромино
typedef struct {
  int shape[TETROMINO_SIZE][TETROMINO_SIZE];
  int x, y;  // Позиция на поле
  TetrominoType_t type;
  int rotation;  // Текущий поворот (0-3)
} Tetromino_t;

// Структура состояния игры Tetris
typedef struct {
  GameInfo_t game_info;
  GameState_t state;
  Tetromino_t current_tetromino;
  Tetromino_t next_tetromino;
  struct timeval last_fall_time;
  int lines_cleared;
  bool game_over;
  bool need_new_tetromino;
  int fall_interval;  // Интервал падения в миллисекундах
} TetrisGameData_t;

// Глобальная переменная состояния игры Tetris
extern TetrisGameData_t tetris_game_data;

// Основные функции для интеграции с FSM
void tetris_init_game(void);
void tetris_cleanup_game(void);
void tetris_reset_game(void);
void tetris_handle_input(UserAction_t action, bool hold);
GameInfo_t tetris_get_game_info(void);
GameState_t tetris_get_current_state(void);
void tetris_set_current_state(GameState_t state);

// Обработчики состояний FSM для тетриса
void tetris_handle_start_state(void);
void tetris_handle_spawn_state(void);
void tetris_handle_moving_state(void);
void tetris_handle_shifting_state(void);
void tetris_handle_attaching_state(void);
void tetris_handle_game_over_state(void);
void tetris_handle_pause_state(void);

// Функции для управления тетромино
void generate_tetromino(Tetromino_t* tetromino, TetrominoType_t type);
void get_random_tetromino(Tetromino_t* tetromino);
bool rotate_tetromino(Tetromino_t* tetromino);
bool move_tetromino(Tetromino_t* tetromino, int dx, int dy);
void place_tetromino_on_field(void);
void get_tetromino_shape(TetrominoType_t type, int rotation,
                         int shape[TETROMINO_SIZE][TETROMINO_SIZE]);
int get_next_rotation(int current_rotation);

// Функции для проверки коллизий и позиций
bool check_collision(const Tetromino_t* tetromino);

// Функции для управления игровым полем
void check_and_clear_lines(void);
int clear_full_lines(void);
bool is_line_full(int line);
void drop_lines_above(int cleared_line);
void calculate_score(int lines_cleared);
void update_level_and_speed(void);
bool is_game_over(void);

// Функции для управления временем и таймерами
bool should_fall(void);
void reset_fall_timer(void);
long get_elapsed_time(const struct timeval* last_time);

// Функции для работы с файлами
void save_high_score(void);
void load_high_score(void);

// Дополнительные функции
void update_next_field(void);
void drop_tetromino_down(void);
int get_fall_speed(int level);

// Функция для получения callbacks структуры для тетриса
GameCallbacks_t* get_tetris_callbacks(void);

// Функции для совместимости (могут быть вызваны из C++)
GameState_t getCurrentGameState(void);
Tetromino_t getCurrentTetromino(void);

#ifdef __cplusplus
}
#endif

#endif  // __TETRIS_H__
