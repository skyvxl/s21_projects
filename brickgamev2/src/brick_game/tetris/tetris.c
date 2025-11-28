#include "tetris.h"

#include <string.h>

// Глобальная переменная состояния игры Tetris
TetrisGameData_t tetris_game_data = {0};

// Основные функции для интеграции с FSM
void tetris_init_game(void) {
  tetris_game_data.game_info.field = allocate_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  tetris_game_data.game_info.next =
      allocate_matrix(NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);

  clear_matrix(tetris_game_data.game_info.field, FIELD_HEIGHT, FIELD_WIDTH);
  clear_matrix(tetris_game_data.game_info.next, NEXT_FIELD_SIZE,
               NEXT_FIELD_SIZE);

  tetris_game_data.game_info.score = 0;
  tetris_game_data.game_info.level = 1;
  tetris_game_data.game_info.speed = 1000;
  tetris_game_data.game_info.pause = 0;

  tetris_game_data.state = STATE_START;
  tetris_game_data.game_over = false;
  tetris_game_data.need_new_tetromino = true;
  tetris_game_data.lines_cleared = 0;
  tetris_game_data.fall_interval = get_fall_speed(1);

  load_high_score();

  srand(time(NULL));
  get_random_tetromino(&tetris_game_data.next_tetromino);
  reset_fall_timer();
}

void tetris_cleanup_game(void) {
  free_matrix(tetris_game_data.game_info.field, FIELD_HEIGHT);
  free_matrix(tetris_game_data.game_info.next, NEXT_FIELD_SIZE);
}

void tetris_reset_game(void) {
  int saved_high_score = tetris_game_data.game_info.high_score;
  tetris_cleanup_game();
  tetris_init_game();
  // Восстанавливаем high score после инициализации
  tetris_game_data.game_info.high_score = saved_high_score;
}

void tetris_handle_input(UserAction_t action, bool hold) {
  (void)hold;  // не используется

  switch (action) {
    case Start:
      if (tetris_game_data.state == STATE_START) {
        tetris_game_data.state = STATE_SPAWN;
      }
      break;
    case Action:
      // Space работает как для запуска игры, так и для поворота
      if (tetris_game_data.state == STATE_START) {
        tetris_game_data.state = STATE_SPAWN;
      } else if ((tetris_game_data.state == STATE_MOVING ||
                  tetris_game_data.state == STATE_SHIFTING) &&
                 !tetris_game_data.game_info.pause) {
        rotate_tetromino(&tetris_game_data.current_tetromino);
      } else if (tetris_game_data.state == STATE_GAME_OVER) {
        tetris_reset_game();  // Перезапуск игры при game over
      }
      break;
    case Pause:
      if (tetris_game_data.state != STATE_START &&
          tetris_game_data.state != STATE_GAME_OVER) {
        if (tetris_game_data.state == STATE_PAUSE) {
          tetris_game_data.state = STATE_MOVING;
          tetris_game_data.game_info.pause = 0;
        } else {
          tetris_game_data.state = STATE_PAUSE;
          tetris_game_data.game_info.pause = 1;
        }
      }
      break;
    case Terminate:
      tetris_game_data.state = STATE_GAME_OVER;
      save_high_score();
      break;
    case Left:
      if ((tetris_game_data.state == STATE_MOVING ||
           tetris_game_data.state == STATE_SHIFTING) &&
          !tetris_game_data.game_info.pause) {
        move_tetromino(&tetris_game_data.current_tetromino, -1, 0);
      }
      break;
    case Right:
      if ((tetris_game_data.state == STATE_MOVING ||
           tetris_game_data.state == STATE_SHIFTING) &&
          !tetris_game_data.game_info.pause) {
        move_tetromino(&tetris_game_data.current_tetromino, 1, 0);
      }
      break;
    case Down:
      if ((tetris_game_data.state == STATE_MOVING ||
           tetris_game_data.state == STATE_SHIFTING) &&
          !tetris_game_data.game_info.pause) {
        drop_tetromino_down();  // Ускоренное падение до конца
      }
      break;
    case Up:
      if ((tetris_game_data.state == STATE_MOVING ||
           tetris_game_data.state == STATE_SHIFTING) &&
          !tetris_game_data.game_info.pause) {
        rotate_tetromino(&tetris_game_data.current_tetromino);
      }
      break;
    default:
      break;
  }
}

GameInfo_t tetris_get_game_info(void) {
  update_next_field();
  return tetris_game_data.game_info;
}

GameState_t tetris_get_current_state(void) { return tetris_game_data.state; }

void tetris_set_current_state(GameState_t state) {
  tetris_game_data.state = state;
}

// Обработчики состояний FSM для тетриса
void tetris_handle_start_state(void) {
  // Ожидание ввода
}

void tetris_handle_spawn_state(void) {
  tetris_game_data.current_tetromino = tetris_game_data.next_tetromino;
  tetris_game_data.current_tetromino.x = FIELD_WIDTH / 2 - 2;
  tetris_game_data.current_tetromino.y = 0;

  get_random_tetromino(&tetris_game_data.next_tetromino);

  if (check_collision(&tetris_game_data.current_tetromino)) {
    tetris_game_data.state = STATE_GAME_OVER;
  } else {
    tetris_game_data.state = STATE_MOVING;
    reset_fall_timer();
  }
}

void tetris_handle_moving_state(void) {
  if (should_fall()) {
    tetris_game_data.state = STATE_SHIFTING;
  }
}

void tetris_handle_shifting_state(void) {
  if (!move_tetromino(&tetris_game_data.current_tetromino, 0, 1)) {
    tetris_game_data.state = STATE_ATTACHING;
  } else {
    tetris_game_data.state = STATE_MOVING;
    reset_fall_timer();
  }
}

void tetris_handle_attaching_state(void) {
  place_tetromino_on_field();
  check_and_clear_lines();

  if (is_game_over()) {
    tetris_game_data.state = STATE_GAME_OVER;
  } else {
    tetris_game_data.state = STATE_SPAWN;
  }
}

void tetris_handle_game_over_state(void) {
  // Конец игры - ожидание завершения
}

void tetris_handle_pause_state(void) {
  // Остановлено - ждем продолжения
}

// Функции для управления тетромино
void generate_tetromino(Tetromino_t* tetromino, TetrominoType_t type) {
  tetromino->type = type;
  tetromino->rotation = 0;
  tetromino->x = 0;
  tetromino->y = 0;

  get_tetromino_shape(type, 0, tetromino->shape);
}

void get_random_tetromino(Tetromino_t* tetromino) {
  TetrominoType_t type = (TetrominoType_t)(rand() % TETROMINO_TYPES);
  generate_tetromino(tetromino, type);
}

bool rotate_tetromino(Tetromino_t* tetromino) {
  int old_rotation = tetromino->rotation;
  tetromino->rotation = get_next_rotation(tetromino->rotation);

  get_tetromino_shape(tetromino->type, tetromino->rotation, tetromino->shape);

  if (check_collision(tetromino)) {
    tetromino->rotation = old_rotation;
    get_tetromino_shape(tetromino->type, tetromino->rotation, tetromino->shape);
    return false;
  }

  return true;
}

bool move_tetromino(Tetromino_t* tetromino, int dx, int dy) {
  int old_x = tetromino->x;
  int old_y = tetromino->y;

  tetromino->x += dx;
  tetromino->y += dy;

  if (check_collision(tetromino)) {
    tetromino->x = old_x;
    tetromino->y = old_y;
    return false;
  }

  return true;
}

void place_tetromino_on_field(void) {
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (tetris_game_data.current_tetromino.shape[y][x]) {
        int field_x = tetris_game_data.current_tetromino.x + x;
        int field_y = tetris_game_data.current_tetromino.y + y;

        if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
            field_x < FIELD_WIDTH) {
          tetris_game_data.game_info.field[field_y][field_x] =
              tetris_game_data.current_tetromino.type + 1;
        }
      }
    }
  }
}

void get_tetromino_shape(TetrominoType_t type, int rotation,
                         int shape[TETROMINO_SIZE][TETROMINO_SIZE]) {
  for (int i = 0; i < TETROMINO_SIZE; i++) {
    for (int j = 0; j < TETROMINO_SIZE; j++) {
      shape[i][j] = tetromino_shapes[type][rotation][i][j];
    }
  }
}

int get_next_rotation(int current_rotation) {
  return (current_rotation + 1) % 4;
}

bool check_collision(const Tetromino_t* tetromino) {
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (tetromino->shape[y][x]) {
        int field_x = tetromino->x + x;
        int field_y = tetromino->y + y;

        // Проверка выхода за границы поля
        if (field_x < 0 || field_x >= FIELD_WIDTH || field_y >= FIELD_HEIGHT) {
          return true;
        }

        // Проверка столкновения с уже размещенными блоками
        if (field_y >= 0 &&
            tetris_game_data.game_info.field[field_y][field_x] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}

// Функции для управления игровым полем
void check_and_clear_lines(void) {
  int lines_cleared = clear_full_lines();
  if (lines_cleared > 0) {
    calculate_score(lines_cleared);
    update_level_and_speed();
  }
}

int clear_full_lines(void) {
  int lines_cleared = 0;

  for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
    if (is_line_full(y)) {
      drop_lines_above(y);
      lines_cleared++;
      y++;  // Проверяем ту же строку
    }
  }

  return lines_cleared;
}

bool is_line_full(int line) {
  for (int x = 0; x < FIELD_WIDTH; x++) {
    if (tetris_game_data.game_info.field[line][x] == 0) {
      return false;
    }
  }
  return true;
}

void drop_lines_above(int cleared_line) {
  for (int y = cleared_line; y > 0; y--) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      tetris_game_data.game_info.field[y][x] =
          tetris_game_data.game_info.field[y - 1][x];
    }
  }

  for (int x = 0; x < FIELD_WIDTH; x++) {
    tetris_game_data.game_info.field[0][x] = 0;
  }
}

void calculate_score(int lines_cleared) {
  int const points[] = {0, 100, 300, 700, 1500};
  if (lines_cleared >= 1 && lines_cleared <= 4) {
    tetris_game_data.game_info.score +=
        points[lines_cleared] * tetris_game_data.game_info.level;
  }

  if (tetris_game_data.game_info.score >
      tetris_game_data.game_info.high_score) {
    tetris_game_data.game_info.high_score = tetris_game_data.game_info.score;
    save_high_score();  // Сохраняем новый рекорд сразу
  }
}

void update_level_and_speed(void) {
  int new_level = tetris_game_data.game_info.score / 600 + 1;
  if (new_level > 10) new_level = 10;

  if (new_level != tetris_game_data.game_info.level) {
    tetris_game_data.game_info.level = new_level;
    tetris_game_data.fall_interval = get_fall_speed(new_level);
    tetris_game_data.game_info.speed = new_level;
  }
}

bool is_game_over(void) {
  for (int x = 0; x < FIELD_WIDTH; x++) {
    if (tetris_game_data.game_info.field[0][x] != 0) {
      return true;
    }
  }
  return false;
}

// Функции для управления временем и таймерами
bool should_fall(void) {
  return get_elapsed_time(&tetris_game_data.last_fall_time) >=
         tetris_game_data.fall_interval * 1000;
}

void reset_fall_timer(void) {
  gettimeofday(&tetris_game_data.last_fall_time, NULL);
}

long get_elapsed_time(const struct timeval* last_time) {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  return (current_time.tv_sec - last_time->tv_sec) * 1000000 +
         (current_time.tv_usec - last_time->tv_usec);
}

void save_high_score(void) {
  FILE* file = fopen("../high_score.txt", "w");
  if (file) {
    fprintf(file, "%d\n", tetris_game_data.game_info.high_score);
    fflush(file);  // Принудительно записываем данные
    fclose(file);
  }
}

void load_high_score(void) {
  FILE* file = fopen("../high_score.txt", "r");
  if (file) {
    int score = 0;
    if (fscanf(file, "%d", &score) == 1 && score >= 0) {
      tetris_game_data.game_info.high_score = score;
    } else {
      tetris_game_data.game_info.high_score = 0;
    }
    fclose(file);
  } else {
    tetris_game_data.game_info.high_score = 0;
  }
}

void update_next_field(void) {
  // Очищаем поле next
  clear_matrix(tetris_game_data.game_info.next, NEXT_FIELD_SIZE,
               NEXT_FIELD_SIZE);

  // Отображаем следующую фигуру в поле next
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (tetris_game_data.next_tetromino.shape[y][x]) {
        if (y < NEXT_FIELD_SIZE && x < NEXT_FIELD_SIZE) {
          tetris_game_data.game_info.next[y][x] =
              tetris_game_data.next_tetromino.type + 1;
        }
      }
    }
  }
}

void drop_tetromino_down(void) {
  // Ускоренное падение - перемещаем фигуру до конца вниз
  while (move_tetromino(&tetris_game_data.current_tetromino, 0, 1)) {
    // Продолжаем движение вниз пока возможно
  }
  // После падения сразу переходим к присоединению
  tetris_game_data.state = STATE_ATTACHING;
}

int get_fall_speed(int level) {
  int const speeds[] = {1000, 900, 800, 700, 600, 500, 400, 300, 200, 100, 50};
  if (level > 10) level = 10;
  return speeds[level - 1];
}

// Функция для получения callbacks структуры для тетриса
GameCallbacks_t* get_tetris_callbacks(void) {
  static GameCallbacks_t tetris_callbacks = {
      .init_game = tetris_init_game,
      .cleanup_game = tetris_cleanup_game,
      .reset_game = tetris_reset_game,
      .handle_input = tetris_handle_input,
      .handle_start_state = tetris_handle_start_state,
      .handle_spawn_state = tetris_handle_spawn_state,
      .handle_moving_state = tetris_handle_moving_state,
      .handle_shifting_state = tetris_handle_shifting_state,
      .handle_attaching_state = tetris_handle_attaching_state,
      .handle_game_over_state = tetris_handle_game_over_state,
      .handle_pause_state = tetris_handle_pause_state,
      .get_game_info = tetris_get_game_info,
      .get_current_state = tetris_get_current_state,
      .set_current_state = tetris_set_current_state};

  return &tetris_callbacks;
}

// Функции для совместимости (могут быть вызваны из C++)
GameState_t getCurrentGameState(void) { return tetris_get_current_state(); }

Tetromino_t getCurrentTetromino(void) {
  return tetris_game_data.current_tetromino;
}
