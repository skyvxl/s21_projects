#define _DEFAULT_SOURCE  // Для usleep
#include "frontend.h"

Screen_t screen = {NULL, NULL, NULL, false, 0, 0};

int init_frontend(void) {
  initscr();
  int height, width;
  getmaxyx(stdscr, height, width);
  endwin();

  int min_width = GAME_AREA_WIDTH + INFO_PANEL_WIDTH + 4;
  int min_height = GAME_AREA_HEIGHT + 1;

  if (width < min_width || height < min_height) {
    fprintf(stderr, "Terminal too small. Need at least %dx%d, got %dx%d\n",
            min_width, min_height, width, height);
    return 0;
  }

  if (!init_ncurses()) return 0;
  if (!init_colors()) return 0;
  if (!init_windows()) return 0;

  screen.initialized = true;
  return 1;
}

void cleanup_frontend(void) {
  if (screen.game_win) delwin(screen.game_win);
  if (screen.info_win) delwin(screen.info_win);
  if (screen.next_win) delwin(screen.next_win);

  endwin();
  screen.initialized = false;
}

bool init_ncurses(void) {
  initscr();
  if (!has_colors()) {
    endwin();
    return false;
  }

  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  keypad(stdscr, TRUE);
  curs_set(0);

  // Заливаем весь экран черным цветом
  clear();
  refresh();

  // Устанавливаем черный фон для всего экрана
  bkgd(' ' | COLOR_PAIR(0));

  return true;
}

bool init_colors(void) {
  if (start_color() == ERR) {
    // Цвета не поддерживаются - продолжаем без них
    return true;
  }

  // Инициализируем базовую пару для черного фона
  init_pair(0, COLOR_WHITE, COLOR_BLACK);  // Базовый фон

  init_pair(1, COLOR_CYAN, COLOR_BLACK);     // I
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // O
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);  // T
  init_pair(4, COLOR_GREEN, COLOR_BLACK);    // S
  init_pair(5, COLOR_RED, COLOR_BLACK);      // Z
  init_pair(6, COLOR_BLUE, COLOR_BLACK);     // J
  init_pair(7, COLOR_WHITE, COLOR_BLACK);    // L
  init_pair(8, COLOR_WHITE, COLOR_BLACK);    // Border

  // Применяем черный фон ко всему экрану
  assume_default_colors(COLOR_WHITE, COLOR_BLACK);
  bkgd(COLOR_PAIR(0));

  return true;
}

bool init_windows(void) {
  screen.game_win =
      newwin(GAME_AREA_HEIGHT, GAME_AREA_WIDTH, GAME_START_Y, GAME_START_X);
  if (!screen.game_win) return false;

  screen.info_win =
      newwin(INFO_PANEL_HEIGHT, INFO_PANEL_WIDTH, INFO_START_Y, INFO_START_X);
  if (!screen.info_win) return false;

  screen.next_win = newwin(NEXT_PREVIEW_SIZE, NEXT_PREVIEW_SIZE,
                           INFO_START_Y + 2, INFO_START_X + 2);
  if (!screen.next_win) return false;

  keypad(screen.game_win, TRUE);
  nodelay(screen.game_win, TRUE);

  return true;
}

void run_game_loop(void) {
  bool game_running = true;

  while (game_running) {
    while (game_data.state != STATE_GAME_OVER) {
      UserAction_t action = get_user_input();
      if (action == Terminate) {
        game_running = false;
        break;
      }

      userInput(action, false);
      updateCurrentState();  // Используем правильную функцию API
      draw_game();

      usleep(16667);  // ~60 FPS официально по слухам
    }

    if (!game_running) break;

    draw_game_over_screen();
    refresh_windows();

    nodelay(stdscr, FALSE);  // Блокируем ввод для ожидания R или Q

    while (true) {
      int key = getch();
      UserAction_t action = process_key(key);

      if (action == Start) {
        reset_game();
        break;
      } else if (action == Terminate) {
        game_running = false;
        break;
      }
    }

    nodelay(stdscr, TRUE);  // Возвращаем неблокирующий ввод
  }
}

UserAction_t get_user_input(void) {
  int ch = wgetch(screen.game_win);
  return process_key(ch);
}

UserAction_t process_key(int key) {
  switch (key) {
    case 'q':
    case 'Q':
      return Terminate;
    case 'p':
    case 'P':
      return Pause;
    case 'r':
    case 'R':
      return Start;
    case KEY_LEFT:
    case 'a':
    case 'A':
      return Left;
    case KEY_RIGHT:
    case 'd':
    case 'D':
      return Right;
    case KEY_DOWN:
    case 's':
    case 'S':
      return Down;
    case KEY_UP:
    case 'w':
    case 'W':
    case 'z':
    case 'Z':
      return Action;
    default:
      return INVALID_ACTION;
  }
}

void draw_game(void) {
  clear_windows();

  switch (game_data.state) {
    case STATE_START:
      draw_start_screen();
      break;
    case STATE_PAUSE:
      draw_field();
      draw_pause_screen();
      draw_info_panel();
      break;
    case STATE_GAME_OVER:
      draw_game_over_screen();
      break;
    default:
      draw_field();
      draw_current_tetromino();
      draw_info_panel();
      break;
  }

  refresh_windows();
}

void draw_start_screen(void) {
  werase(screen.game_win);
  box(screen.game_win, 0, 0);

  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 - 2, "TETRIS");
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2,
                     "Press 'R' to start");
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 + 1,
                     "Press 'Q' to quit");
}

void draw_pause_screen(void) {
  int center_y = GAME_AREA_HEIGHT / 2;
  int center_x = GAME_AREA_WIDTH / 2;

  wattron(screen.game_win, COLOR_PAIR(8));
  mvwaddstr(screen.game_win, center_y, center_x - 3, "PAUSED");
  wattroff(screen.game_win, COLOR_PAIR(8));
}

void draw_game_over_screen(void) {
  clear_windows();

  werase(screen.game_win);
  box(screen.game_win, 0, 0);

  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 - 2, "GAME OVER");

  char score_text[32];
  snprintf(score_text, sizeof(score_text), "Score: %d",
           game_data.game_info.score);
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2, score_text);

  char level_text[32];
  snprintf(level_text, sizeof(level_text), "Level: %d",
           game_data.game_info.level);
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 + 1, level_text);

  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 + 3,
                     "Press 'R' to restart");
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 + 4,
                     "Press 'Q' to exit");
  draw_info_panel();
}

void draw_field(void) {
  werase(screen.game_win);
  box(screen.game_win, 0, 0);

  draw_centered_text(screen.game_win, 0, "TETRIS");

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      int value = game_data.game_info.field[y][x];
      draw_block(screen.game_win, y + 1, x * 2 + 1, value);
    }
  }
}

void draw_current_tetromino(void) {
  if (game_data.state == STATE_MOVING || game_data.state == STATE_SHIFTING) {
    const Tetromino_t *tetromino = &game_data.current_tetromino;

    for (int y = 0; y < TETROMINO_SIZE; y++) {
      for (int x = 0; x < TETROMINO_SIZE; x++) {
        if (tetromino->shape[y][x]) {
          int field_x = tetromino->x + x;
          int field_y = tetromino->y + y;

          if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
              field_x < FIELD_WIDTH) {
            draw_block(screen.game_win, field_y + 1, field_x * 2 + 1,
                       tetromino->type + 1);
          }
        }
      }
    }
  }
}

void draw_info_panel(void) {
  werase(screen.info_win);
  box(screen.info_win, 0, 0);

  draw_centered_text(screen.info_win, 0, "INFO");

  mvwaddstr(screen.info_win, 2, 2, "Next:");
  draw_next_tetromino();

  mvwprintw(screen.info_win, 8, 2, "Score: %d", game_data.game_info.score);
  mvwprintw(screen.info_win, 9, 2, "High:  %d", game_data.game_info.high_score);
  mvwprintw(screen.info_win, 10, 2, "Level: %d", game_data.game_info.level);
  mvwprintw(screen.info_win, 11, 2, "Speed: %d", game_data.game_info.speed);

  mvwaddstr(screen.info_win, 13, 2, "Controls:");
  mvwaddstr(screen.info_win, 14, 2, "A/D Move");
  mvwaddstr(screen.info_win, 15, 2, "S   Drop");
  mvwaddstr(screen.info_win, 16, 2, "W/Z Rotate");
  mvwaddstr(screen.info_win, 17, 2, "P   Pause");
  mvwaddstr(screen.info_win, 18, 2, "Q   Quit");
}

void draw_next_tetromino(void) {
  for (int y = 3; y < 7; y++) {
    for (int x = 3; x < 7; x++) {
      mvwaddstr(screen.info_win, y, x, " ");
    }
  }

  const Tetromino_t *next = &game_data.next_tetromino;
  for (int y = 0; y < TETROMINO_SIZE; y++) {
    for (int x = 0; x < TETROMINO_SIZE; x++) {
      if (next->shape[y][x]) {
        int color = next->type + 1;
        if (has_colors()) {
          wattron(screen.info_win, COLOR_PAIR(color));
          mvwaddstr(screen.info_win, 3 + y, 3 + x, "#");
          wattroff(screen.info_win, COLOR_PAIR(color));
        } else {
          mvwaddstr(screen.info_win, 3 + y, 3 + x, "#");
        }
      }
    }
  }
}

void draw_block(WINDOW *win, int y, int x, int value) {
  if (value > 0 && value <= 7) {
    if (has_colors()) {
      wattron(win, COLOR_PAIR(value));
      mvwaddstr(win, y, x, "[]");
      wattroff(win, COLOR_PAIR(value));
    } else {
      // Без цветов - используем простые ASCII символы
      mvwaddstr(win, y, x, "##");
    }
  } else {
    mvwaddstr(win, y, x, "  ");
  }
}

void draw_centered_text(WINDOW *win, int y, const char *text) {
  int win_width = getmaxx(win);
  int text_len = strlen(text);
  int x = (win_width - text_len) / 2;
  mvwaddstr(win, y, x, text);
}

void clear_windows(void) {
  clear();
  refresh();

  if (screen.game_win) werase(screen.game_win);
  if (screen.info_win) werase(screen.info_win);
}

void refresh_windows(void) {
  if (screen.game_win) wrefresh(screen.game_win);
  if (screen.info_win) wrefresh(screen.info_win);
  refresh();
}
