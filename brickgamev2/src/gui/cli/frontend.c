#define _DEFAULT_SOURCE  // Для usleep
#include "frontend.h"

#include <string.h>

Screen_t screen = {NULL, NULL, NULL, false, 0, 0};
GameType_t current_game_type = GAME_TETRIS;
GameInfo_t current_game_info;

static bool confirm_exit_prompt(void) {
  int center_y = LINES / 2 + 3;
  int center_x = COLS / 2;

  mvprintw(center_y, center_x - 16, "Exit program? Press Y to confirm.");
  mvprintw(center_y + 1, center_x - 18,
           "Press any other key to return to menu.");
  refresh();

  int ch = getch();
  return ch == 'y' || ch == 'Y';
}

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

  // Цвета для Tetris
  init_pair(1, COLOR_CYAN, COLOR_BLACK);     // I
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // O
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);  // T
  init_pair(4, COLOR_GREEN, COLOR_BLACK);    // S
  init_pair(5, COLOR_RED, COLOR_BLACK);      // Z
  init_pair(6, COLOR_BLUE, COLOR_BLACK);     // J
  init_pair(7, COLOR_WHITE, COLOR_BLACK);    // L
  init_pair(8, COLOR_WHITE, COLOR_BLACK);    // Border

  // Цвета для Snake
  init_pair(9, COLOR_GREEN, COLOR_BLACK);  // Snake body
  init_pair(10, COLOR_RED, COLOR_BLACK);   // Apple
  init_pair(11, COLOR_CYAN, COLOR_BLACK);  // Snake head

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
  bool space_pressed = false;

  while (game_running) {
    while (fsm_get_state() != STATE_GAME_OVER) {
      UserAction_t action = get_user_input();
      if (action == Terminate) {
        if (space_pressed && current_game_type == GAME_SNAKE) {
          space_pressed = false;
          userInput(Action, false);
        }
        game_running = false;
        break;
      }

      // Handle space key hold/release for snake acceleration
      if (action == Action && current_game_type == GAME_SNAKE) {
        if (!space_pressed) {
          space_pressed = true;
          userInput(action, true);  // Space pressed down
        }
      } else {
        if (space_pressed && current_game_type == GAME_SNAKE) {
          space_pressed = false;
          userInput(Action, false);  // Space released
        }
        if (action != INVALID_ACTION) {
          userInput(action, false);
        }
      }

      current_game_info =
          updateCurrentState();  // Обновляем состояние и получаем info
      draw_game();

      usleep(16667);  // ~60 FPS
    }

    if (!game_running) break;

    draw_game_over_screen();
    refresh_windows();

    nodelay(stdscr, FALSE);  // Блокируем ввод для ожидания R или Q

    while (true) {
      int key = getch();
      UserAction_t action = process_key(key);

      if (action == Start) {
        if (fsm_state.game_callbacks && fsm_state.game_callbacks->reset_game) {
          fsm_state.game_callbacks->reset_game();
        }
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
  int ch = wgetch(stdscr);
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
      if (current_game_type == GAME_TETRIS) {
        return Action;  // Rotate for Tetris
      } else {
        return Up;  // Up for Snake
      }
    case 'w':
    case 'W':
      if (current_game_type == GAME_TETRIS) {
        return Action;  // Rotate for Tetris
      } else {
        return Up;  // Up for Snake
      }
    case 'z':
    case 'Z':
      if (current_game_type == GAME_TETRIS) {
        return Action;  // Rotate for Tetris
      } else {
        return Down;  // Down for Snake (alternative)
      }
    case ' ':
      return Action;  // Space for boost in Snake, or action in Tetris
    default:
      return INVALID_ACTION;
  }
}

void draw_game(void) {
  clear_windows();

  switch (fsm_get_state()) {
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
      if (current_game_type == GAME_TETRIS) {
        draw_current_tetromino();
      } else {
        draw_snake();
      }
      draw_info_panel();
      break;
  }

  refresh_windows();
}

void draw_start_screen(void) {
  werase(screen.game_win);
  box(screen.game_win, 0, 0);

  const char* game_name =
      (current_game_type == GAME_TETRIS) ? "TETRIS" : "SNAKE";
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 - 2, game_name);
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

  GameInfo_t game_info = current_game_info;

  // Check for victory condition in Snake
  if (current_game_type == GAME_SNAKE && game_info.score >= 200) {
    draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 - 2, "VICTORY!");
    draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 - 1,
                       "You reached 200 points!");
  } else {
    draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2 - 2, "GAME OVER");
  }

  char score_text[32];
  snprintf(score_text, sizeof(score_text), "Score: %d", game_info.score);
  draw_centered_text(screen.game_win, GAME_AREA_HEIGHT / 2, score_text);

  char level_text[32];
  snprintf(level_text, sizeof(level_text), "Level: %d", game_info.level);
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

  const char* game_name =
      (current_game_type == GAME_TETRIS) ? "TETRIS" : "SNAKE";
  draw_centered_text(screen.game_win, 0, game_name);

  GameInfo_t game_info = current_game_info;

  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = 0; x < FIELD_WIDTH; x++) {
      int value = game_info.field[y][x];
      draw_block(screen.game_win, y + 1, x * 2 + 1, value);
    }
  }
}

void draw_current_tetromino(void) {
  GameState_t state = fsm_get_state();
  if (state == STATE_MOVING || state == STATE_SHIFTING) {
    Tetromino_t tetromino = getCurrentTetromino();

    for (int y = 0; y < TETROMINO_SIZE; y++) {
      for (int x = 0; x < TETROMINO_SIZE; x++) {
        if (tetromino.shape[y][x]) {
          int field_x = tetromino.x + x;
          int field_y = tetromino.y + y;

          if (field_y >= 0 && field_y < FIELD_HEIGHT && field_x >= 0 &&
              field_x < FIELD_WIDTH) {
            draw_block(screen.game_win, field_y + 1, field_x * 2 + 1,
                       tetromino.type + 1);
          }
        }
      }
    }
  }
}

void draw_snake(void) {
  // Snake drawing is handled in draw_field since snake elements are in the
  // field No additional drawing needed as snake body and apple are already
  // drawn in the field
}

void draw_info_panel(void) {
  werase(screen.info_win);
  box(screen.info_win, 0, 0);

  draw_centered_text(screen.info_win, 0, "INFO");

  if (current_game_type == GAME_TETRIS) {
    mvwaddstr(screen.info_win, 2, 2, "Next:");
    draw_next_tetromino();
  } else {
    // For Snake, we could show something else or leave empty
    mvwaddstr(screen.info_win, 2, 2, "Snake");
  }

  GameInfo_t game_info = current_game_info;

  mvwprintw(screen.info_win, 8, 2, "Score: %d", game_info.score);
  mvwprintw(screen.info_win, 9, 2, "High:  %d", game_info.high_score);
  mvwprintw(screen.info_win, 10, 2, "Level: %d", game_info.level);
  mvwprintw(screen.info_win, 11, 2, "Speed: %d", game_info.speed);

  mvwaddstr(screen.info_win, 13, 2, "Controls:");
  if (current_game_type == GAME_TETRIS) {
    mvwaddstr(screen.info_win, 14, 2, "A/D Move");
    mvwaddstr(screen.info_win, 15, 2, "S   Drop");
    mvwaddstr(screen.info_win, 16, 2, "W/Z Rotate");
  } else {
    mvwaddstr(screen.info_win, 14, 2, "A/D Move");
    mvwaddstr(screen.info_win, 15, 2, "W/Z Turn");
    mvwaddstr(screen.info_win, 16, 2, "Space Boost");
  }
  mvwaddstr(screen.info_win, 17, 2, "P   Pause");
  mvwaddstr(screen.info_win, 18, 2, "Q   Quit");
}

void draw_next_tetromino(void) {
  for (int y = 3; y < 7; y++) {
    for (int x = 3; x < 7; x++) {
      mvwaddstr(screen.info_win, y, x, " ");
    }
  }

  GameInfo_t game_info = current_game_info;

  // Отображаем поле next (4x4)
  for (int y = 0; y < NEXT_FIELD_SIZE; y++) {
    for (int x = 0; x < NEXT_FIELD_SIZE; x++) {
      int value = game_info.next[y][x];
      if (value > 0) {
        if (has_colors()) {
          wattron(screen.info_win, COLOR_PAIR(value));
          mvwaddstr(screen.info_win, 3 + y, 3 + x, "#");
          wattroff(screen.info_win, COLOR_PAIR(value));
        } else {
          mvwaddstr(screen.info_win, 3 + y, 3 + x, "#");
        }
      }
    }
  }
}

void draw_block(WINDOW* win, int y, int x, int value) {
  if (value > 0) {
    if (current_game_type == GAME_SNAKE) {
      // Snake colors: 1=body(green), 2=apple(red), 3=head(cyan)
      int color_pair = 9 + (value - 1);  // 9, 10, 11
      if (has_colors()) {
        wattron(win, COLOR_PAIR(color_pair));
        mvwaddstr(win, y, x, "[]");
        wattroff(win, COLOR_PAIR(color_pair));
      } else {
        mvwaddstr(win, y, x, "##");
      }
    } else {
      // Tetris colors: 1-7
      if (value <= 7) {
        if (has_colors()) {
          wattron(win, COLOR_PAIR(value));
          mvwaddstr(win, y, x, "[]");
          wattroff(win, COLOR_PAIR(value));
        } else {
          mvwaddstr(win, y, x, "##");
        }
      } else {
        mvwaddstr(win, y, x, "  ");
      }
    }
  } else {
    mvwaddstr(win, y, x, "  ");
  }
}

void draw_centered_text(WINDOW* win, int y, const char* text) {
  int win_width = getmaxx(win);
  int text_len = strlen(text);
  int x = (win_width - text_len) / 2;
  mvwaddstr(win, y, x, text);
}

bool show_game_selection(void) {
  nodelay(stdscr, FALSE);

  while (true) {
    clear();
    refresh();

    int center_y = LINES / 2;
    int center_x = COLS / 2;

    mvprintw(center_y - 4, center_x - 10, "Select Game");
    mvprintw(center_y - 2, center_x - 10, "1. Tetris");
    mvprintw(center_y - 1, center_x - 10, "2. Snake");
    mvprintw(center_y + 1, center_x - 18, "Press 1 or 2 to select, Q to quit");

    refresh();

    int ch = getch();
    switch (ch) {
      case '1':
        current_game_type = GAME_TETRIS;
        nodelay(stdscr, TRUE);
        clear();
        refresh();
        return true;
      case '2':
        current_game_type = GAME_SNAKE;
        nodelay(stdscr, TRUE);
        clear();
        refresh();
        return true;
      case 'q':
      case 'Q':
        if (confirm_exit_prompt()) {
          nodelay(stdscr, TRUE);
          clear();
          refresh();
          return false;
        }
        break;
      default:
        break;
    }
  }
}

void init_selected_game(void) {
  GameCallbacks_t* callbacks = NULL;
  GameType_t game_type;

  if (current_game_type == GAME_TETRIS) {
    callbacks = get_tetris_callbacks();
    game_type = GAME_TETRIS;
  } else {
    callbacks = get_snake_callbacks();
    game_type = GAME_SNAKE;
  }

  if (callbacks) {
    fsm_init(game_type, callbacks);
  }
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

void shutdown_current_game(void) {
  fsm_cleanup();
  current_game_info = (GameInfo_t){0};
}
