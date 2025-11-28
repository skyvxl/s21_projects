#ifndef __BACKEND_H__
#define __BACKEND_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "tetromino_shapes.h"

// Константы игры
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define TETROMINO_SIZE 4
#define TETROMINO_TYPES 7
#define NEXT_FIELD_SIZE 4

// Пользовательские действия
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

// Состояния конечного автомата
typedef enum {
  STATE_START,  // Ожидание начала игры
  STATE_SPAWN,  // Создание новой фигуры
  STATE_MOVING,  // Основное состояние - движение фигуры
  STATE_SHIFTING,   // Опускание фигуры вниз
  STATE_ATTACHING,  // Присоединение фигуры к полю
  STATE_GAME_OVER,  // Конец игры
  STATE_PAUSE       // Пауза
} GameState_t;

// Структура тетромино
typedef struct {
  int shape[TETROMINO_SIZE][TETROMINO_SIZE];
  int x, y;  // Позиция на поле
  TetrominoType_t type;
  int rotation;  // Текущий поворот (0-3)
} Tetromino_t;

// Информация о текущем состоянии игры (обязательный API)
typedef struct {
  int **field;     // Игровое поле 20x10
  int **next;      // Следующая фигура 4x4
  int score;       // Текущий счет
  int high_score;  // Рекорд
  int level;       // Уровень
  int speed;       // Скорость
  int pause;       // Флаг паузы
} GameInfo_t;

// Структура состояния игры
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
} GameData_t;

// Глобальная переменная состояния игры
extern GameData_t game_data;

// Массив форм тетромино [тип][поворот][строка][столбец]
// Определен в tetromino_shapes.c
extern const int tetromino_shapes[TETROMINO_TYPES][4][TETROMINO_SIZE]
                                 [TETROMINO_SIZE];

// Функции для управления состоянием игры
void userInput(UserAction_t action, bool hold);
GameInfo_t updateCurrentState(void);

// Функции для управления состоянием игры
void init_game(void);
void cleanup_game(void);
void reset_game(void);
void start_game(void);
void pause_game(void);
void terminate_game(void);

// Функции для инициализации и очистки игры
int **allocate_matrix(int rows, int cols);
void free_matrix(int **matrix, int rows);
void clear_matrix(int **matrix, int rows, int cols);
void copy_matrix(int **src, int **dest, int rows, int cols);

// Функции для управления тетромино
void generate_tetromino(Tetromino_t *tetromino, TetrominoType_t type);
void get_random_tetromino(Tetromino_t *tetromino);
bool rotate_tetromino(Tetromino_t *tetromino);
bool move_tetromino(Tetromino_t *tetromino, int dx, int dy);
void place_tetromino_on_field(void);
void get_tetromino_shape(TetrominoType_t type, int rotation,
                         int shape[TETROMINO_SIZE][TETROMINO_SIZE]);
int get_next_rotation(int current_rotation);
int get_prev_rotation(int current_rotation);

// Функции для проверки коллизий и позиций
bool check_collision(const Tetromino_t *tetromino);
bool check_position(const Tetromino_t *tetromino, int new_x, int new_y);
bool can_rotate(const Tetromino_t *tetromino);

// Функции для управления игровым полем
void check_and_clear_lines(void);
int clear_full_lines(void);
bool is_line_full(int line);
void drop_lines_above(int cleared_line);
void calculate_score(int lines_cleared);
void update_level_and_speed(void);
bool is_game_over(void);

// Функции для управления состоянием игры
void update_fsm(void);
void handle_start_state(void);
void handle_spawn_state(void);
void handle_moving_state(void);
void handle_shifting_state(void);
void handle_attaching_state(void);
void handle_game_over_state(void);
void handle_pause_state(void);

// Функции для управления временем и таймерами
bool should_fall(void);
void reset_fall_timer(void);
long get_elapsed_time(const struct timeval *last_time);

// Функции для управления временем и таймерами
void save_high_score(void);
void load_high_score(void);

// Функции для управления предпросмотром следующей фигуры
void update_next_field(void);
void drop_tetromino_down(void);
int get_fall_speed(int level);

#endif  // __BACKEND_H__
