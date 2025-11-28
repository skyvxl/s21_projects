#ifndef __TETROMINO_SHAPES_H__
#define __TETROMINO_SHAPES_H__

// Константы для форм тетромино
#define TETROMINO_SIZE 4
#define TETROMINO_TYPES 7

// Типы тетромино (дублируем здесь для избежания циклических зависимостей)
typedef enum {
  TETROMINO_I = 0,  // Палка
  TETROMINO_O,      // Квадрат
  TETROMINO_T,      // Т-образная
  TETROMINO_S,      // S-образная
  TETROMINO_Z,      // Z-образная
  TETROMINO_J,      // J-образная
  TETROMINO_L       // L-образная
} TetrominoType_t;

// Объявление массива форм тетромино (определение в tetromino_shapes.c)
extern const int tetromino_shapes[TETROMINO_TYPES][4][TETROMINO_SIZE]
                                 [TETROMINO_SIZE];

#endif  // __TETROMINO_SHAPES_H__
