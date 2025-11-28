#include "backend.h"

GameData_t game_data;

void init_game(void) {
  game_data.game_info.field = allocate_matrix(FIELD_HEIGHT, FIELD_WIDTH);
  game_data.game_info.next = allocate_matrix(NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);

  clear_matrix(game_data.game_info.field, FIELD_HEIGHT, FIELD_WIDTH);
  clear_matrix(game_data.game_info.next, NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);

  game_data.game_info.score = 0;
  game_data.game_info.level = 1;
  game_data.game_info.speed = 1000;
  game_data.game_info.pause = 0;

  game_data.state = STATE_START;
  game_data.game_over = false;
  game_data.need_new_tetromino = true;
  game_data.lines_cleared = 0;
  game_data.fall_interval = get_fall_speed(1);

  load_high_score();

  srand(time(NULL));
  get_random_tetromino(&game_data.next_tetromino);
  reset_fall_timer();
}

void cleanup_game(void) {
  free_matrix(game_data.game_info.field, FIELD_HEIGHT);
  free_matrix(game_data.game_info.next, NEXT_FIELD_SIZE);
}

void reset_game(void) {
  int saved_high_score = game_data.game_info.high_score;
  cleanup_game();
  init_game();
  // Восстанавливаем high score после инициализации
  game_data.game_info.high_score = saved_high_score;
}

void start_game(void) {
  if (game_data.state == STATE_START) {
    game_data.state = STATE_SPAWN;
  }
}

void pause_game(void) {
  if (game_data.state != STATE_START && game_data.state != STATE_GAME_OVER) {
    if (game_data.state == STATE_PAUSE) {
      game_data.state = STATE_MOVING;
      game_data.game_info.pause = 0;
    } else {
      game_data.state = STATE_PAUSE;
      game_data.game_info.pause = 1;
    }
  }
}

void terminate_game(void) {
  game_data.state = STATE_GAME_OVER;
  save_high_score();
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;  // не используется

  switch (action) {
    case Start:
      if (game_data.state == STATE_START) {
        start_game();
      }
      break;
    case Pause:
      pause_game();
      break;
    case Terminate:
      terminate_game();
      break;
    case Left:
      if (game_data.state == STATE_MOVING && !game_data.game_info.pause) {
        move_tetromino(&game_data.current_tetromino, -1, 0);
      }
      break;
    case Right:
      if (game_data.state == STATE_MOVING && !game_data.game_info.pause) {
        move_tetromino(&game_data.current_tetromino, 1, 0);
      }
      break;
    case Down:
      if (game_data.state == STATE_MOVING && !game_data.game_info.pause) {
        drop_tetromino_down();  // Ускоренное падение до конца
      }
      break;
    case Action:
    case Up:
      if (game_data.state == STATE_MOVING && !game_data.game_info.pause) {
        rotate_tetromino(&game_data.current_tetromino);
      }
      break;
    default:
      break;
  }
}

void update_fsm(void) {
  switch (game_data.state) {
    case STATE_START:
      handle_start_state();
      break;
    case STATE_SPAWN:
      handle_spawn_state();
      break;
    case STATE_MOVING:
      handle_moving_state();
      break;
    case STATE_SHIFTING:
      handle_shifting_state();
      break;
    case STATE_ATTACHING:
      handle_attaching_state();
      break;
    case STATE_GAME_OVER:
      handle_game_over_state();
      break;
    case STATE_PAUSE:
      handle_pause_state();
      break;
  }
}

void handle_start_state(void) {
  // Ожидание ввода
}

void handle_spawn_state(void) {
  game_data.current_tetromino = game_data.next_tetromino;
  game_data.current_tetromino.x = FIELD_WIDTH / 2 - 2;
  game_data.current_tetromino.y = 0;

  get_random_tetromino(&game_data.next_tetromino);

  if (check_collision(&game_data.current_tetromino)) {
    game_data.state = STATE_GAME_OVER;
  } else {
    game_data.state = STATE_MOVING;
    reset_fall_timer();
  }
}

void handle_moving_state(void) {
  if (should_fall()) {
    game_data.state = STATE_SHIFTING;
  }
}

void handle_shifting_state(void) {
  if (!move_tetromino(&game_data.current_tetromino, 0, 1)) {
    game_data.state = STATE_ATTACHING;
  } else {
    game_data.state = STATE_MOVING;
    reset_fall_timer();
  }
}

void handle_attaching_state(void) {
  place_tetromino_on_field();
  check_and_clear_lines();

  if (is_game_over()) {
    game_data.state = STATE_GAME_OVER;
  } else {
    game_data.state = STATE_SPAWN;
  }
}

void handle_game_over_state(void) {
  // Конец игры - ожидание завершения
}

void handle_pause_state(void) {
  // Остановлено - ждем продолжения
}

int **allocate_matrix(int rows, int cols) {
  int **matrix = calloc(rows, sizeof(int *));
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      matrix[i] = calloc(cols, sizeof(int));
      if (!matrix[i]) {
        free_matrix(matrix, i);
        return NULL;
      }
    }
  }
  return matrix;
}

void free_matrix(int **matrix, int rows) {
  if (matrix) {
    for (int i = 0; i < rows; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }
}

void clear_matrix(int **matrix, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    memset(matrix[i], 0, cols * sizeof(int));
  }
}

void generate_tetromino(Tetromino_t *tetromino, TetrominoType_t type) {
  tetromino->type = type;
  tetromino->rotation = 0;
  tetromino->x = 0;
  tetromino->y = 0;

  get_tetromino_shape(type, 0, tetromino->shape);
}

void get_random_tetromino(Tetromino_t *tetromino) {
  TetrominoType_t type = rand() % TETROMINO_TYPES;
  generate_tetromino(tetromino, type);
}

bool rotate_tetromino(Tetromino_t *tetromino) {
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

bool move_tetromino(Tetromino_t *tetromino, int dx, int dy) {
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
      if (game_data.current_tetromino.shape[y][x]) {
        int field_x = game_data.current_tetromino.x + x;
        int field_y = game_data.current_tetromino.y + y;

        if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
            field_x < FIELD_WIDTH) {
          game_data.game_info.field[field_y][field_x] =
              game_data.current_tetromino.type + 1;
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

bool check_collision(const Tetromino_t *tetromino) {
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (tetromino->shape[y][x]) {
        int field_x = tetromino->x + x;
        int field_y = tetromino->y + y;

        if (field_x < 0 || field_x >= FIELD_WIDTH || field_y >= FIELD_HEIGHT) {
          return true;
        }

        if (field_y >= 0 && game_data.game_info.field[field_y][field_x] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}

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
    if (game_data.game_info.field[line][x] == 0) {
      return false;
    }
  }
  return true;
}

void drop_lines_above(int cleared_line) {
  for (int y = cleared_line; y > 0; y--) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      game_data.game_info.field[y][x] = game_data.game_info.field[y - 1][x];
    }
  }

  for (int x = 0; x < FIELD_WIDTH; x++) {
    game_data.game_info.field[0][x] = 0;
  }
}

void calculate_score(int lines_cleared) {
  int const points[] = {0, 100, 300, 700, 1500};
  if (lines_cleared >= 1 && lines_cleared <= 4) {
    game_data.game_info.score +=
        points[lines_cleared] * game_data.game_info.level;
  }

  if (game_data.game_info.score > game_data.game_info.high_score) {
    game_data.game_info.high_score = game_data.game_info.score;
    save_high_score();  // Сохраняем новый рекорд сразу
  }
}

void update_level_and_speed(void) {
  int new_level = game_data.game_info.score / 600 + 1;
  if (new_level > 10) new_level = 10;

  if (new_level != game_data.game_info.level) {
    game_data.game_info.level = new_level;
    game_data.fall_interval = get_fall_speed(new_level);
    game_data.game_info.speed = new_level;
  }
}

bool is_game_over(void) {
  for (int x = 0; x < FIELD_WIDTH; x++) {
    if (game_data.game_info.field[0][x] != 0) {
      return true;
    }
  }
  return false;
}

bool should_fall(void) {
  return get_elapsed_time(&game_data.last_fall_time) >=
         game_data.fall_interval * 1000;
}

void reset_fall_timer(void) { gettimeofday(&game_data.last_fall_time, NULL); }

long get_elapsed_time(const struct timeval *last_time) {
  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  return (current_time.tv_sec - last_time->tv_sec) * 1000000 +
         (current_time.tv_usec - last_time->tv_usec);
}

void save_high_score(void) {
  FILE *file = fopen("high_score.txt", "w");
  if (file) {
    fprintf(file, "%d\n", game_data.game_info.high_score);
    fflush(file);  // Принудительно записываем данные
    fclose(file);
  }
}

void load_high_score(void) {
  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    int score = 0;
    if (fscanf(file, "%d", &score) == 1 && score >= 0) {
      game_data.game_info.high_score = score;
    } else {
      game_data.game_info.high_score = 0;
    }
    fclose(file);
  } else {
    game_data.game_info.high_score = 0;
  }
}

GameInfo_t updateCurrentState(void) {
  update_fsm();
  update_next_field();
  return game_data.game_info;
}

void update_next_field(void) {
  // Очищаем поле next
  clear_matrix(game_data.game_info.next, NEXT_FIELD_SIZE, NEXT_FIELD_SIZE);

  // Отображаем следующую фигуру в поле next
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (game_data.next_tetromino.shape[y][x]) {
        game_data.game_info.next[y][x] = game_data.next_tetromino.type + 1;
      }
    }
  }
}

void drop_tetromino_down(void) {
  // Ускоренное падение - перемещаем фигуру до конца вниз
  while (move_tetromino(&game_data.current_tetromino, 0, 1)) {
    // Продолжаем движение вниз пока возможно
  }
  // После падения сразу переходим к присоединению
  game_data.state = STATE_ATTACHING;
}

int get_fall_speed(int level) {
  int const speeds[] = {1000, 900, 800, 700, 600, 500, 400, 300, 200, 100, 50};
  if (level > 10) level = 10;
  return speeds[level - 1];
}
