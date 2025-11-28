#ifndef __FRONTEND_H__
#define __FRONTEND_H__

#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "../../brick_game/tetris/backend.h"

#define GAME_AREA_WIDTH \
  (FIELD_WIDTH * 2 + 2)  // Каждый блок 2 символа + границы
#define GAME_AREA_HEIGHT (FIELD_HEIGHT + 2)  // Поле + границы
#define INFO_PANEL_WIDTH 16
#define INFO_PANEL_HEIGHT GAME_AREA_HEIGHT
#define NEXT_PREVIEW_SIZE 6

// Позиции на экране
#define GAME_START_Y 2
#define GAME_START_X 2
#define INFO_START_Y 2
#define INFO_START_X (GAME_START_X + GAME_AREA_WIDTH + 2)

// Символы для отображения
#define EMPTY_CELL "  "
#define FILLED_CELL "██"
#define BORDER_HORIZONTAL "─"
#define BORDER_VERTICAL "│"
#define BORDER_CORNER_TL "┌"
#define BORDER_CORNER_TR "┐"
#define BORDER_CORNER_BL "└"
#define BORDER_CORNER_BR "┘"

// Цвета для тетромино
typedef enum {
  COLOR_EMPTY = 0,
  COLOR_I = 1,  // Cyan
  COLOR_O,      // Yellow
  COLOR_T,      // Purple
  COLOR_S,      // Green
  COLOR_Z,      // Red
  COLOR_J,      // Blue
  COLOR_L,      // Orange
  COLOR_BORDER  // White
} TetrisColor_t;

// Структура для управления экраном
typedef struct {
  WINDOW *game_win;  // Окно игрового поля
  WINDOW *info_win;  // Окно информационной панели
  WINDOW *next_win;  // Окно предпросмотра следующей фигуры
  bool initialized;
  int terminal_width;
  int terminal_height;
} Screen_t;

// Глобальная переменная экрана
extern Screen_t screen;

int init_frontend(void);
void cleanup_frontend(void);
void run_game_loop(void);

bool init_ncurses(void);
bool init_colors(void);
bool init_windows(void);
bool check_terminal_size(void);

void draw_game(void);
void draw_field(void);
void draw_current_tetromino(void);
void draw_info_panel(void);
void draw_next_tetromino(void);
void draw_borders(void);
void draw_game_over_screen(void);
void draw_start_screen(void);
void draw_pause_screen(void);

UserAction_t get_user_input(void);
UserAction_t process_key(int key);
bool is_action_key(int key);

void init_color_pairs(void);
int get_tetromino_color(int value);
void set_color_for_value(int value);

void draw_score_info(void);
void draw_level_info(void);
void draw_lines_info(void);
void draw_controls_info(void);
void draw_centered_text(WINDOW *win, int y, const char *text);

void clear_windows(void);
void refresh_windows(void);
void resize_windows(void);

void draw_block(WINDOW *win, int y, int x, int value);
void draw_tetromino_shape(WINDOW *win, int start_y, int start_x,
                          int shape[TETROMINO_SIZE][TETROMINO_SIZE]);
void center_window_text(WINDOW *win, int y, const char *format, ...);

void handle_timing(void);
bool should_update_display(void);
void limit_frame_rate(void);

#endif  // __FRONTEND_H__
