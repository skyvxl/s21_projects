#include "tetris_test.h"

// Инициализация и очистка для каждого теста
void setup(void) { init_game(); }

void teardown(void) { cleanup_game(); }

// Тест 1: Инициализация игры
START_TEST(test_init_game) {
  ck_assert_int_eq(game_data.game_info.score, 0);
  ck_assert_int_eq(game_data.game_info.level, 1);
  ck_assert_int_eq(game_data.state, STATE_START);
  ck_assert_ptr_nonnull(game_data.game_info.field);
  ck_assert_ptr_nonnull(game_data.game_info.next);
}
END_TEST

// Тест 2: Начало игры и спавн фигуры
START_TEST(test_start_and_spawn) {
  userInput(Start, false);
  update_fsm();  // STATE_START -> STATE_MOVING
  ck_assert_int_eq(game_data.state, STATE_MOVING);
  update_fsm();  // STATE_MOVING -> STATE_MOVING
  ck_assert_int_eq(game_data.state, STATE_MOVING);
  ck_assert_int_ge(game_data.current_tetromino.type, 0);
  ck_assert_int_le(game_data.current_tetromino.type, 6);
}
END_TEST

// Тест 3: Движение влево
START_TEST(test_move_left) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  int initial_x = game_data.current_tetromino.x;
  userInput(Left, false);
  ck_assert_int_eq(game_data.current_tetromino.x, initial_x - 1);
}
END_TEST

// Тест 4: Движение вправо
START_TEST(test_move_right) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  int initial_x = game_data.current_tetromino.x;
  userInput(Right, false);
  ck_assert_int_eq(game_data.current_tetromino.x, initial_x + 1);
}
END_TEST

// Тест 5: Ускоренное падение (изменен под новую логику)
START_TEST(test_move_down) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  int initial_y = game_data.current_tetromino.y;
  userInput(Down, false);  // Теперь это ускоренное падение
  ck_assert_int_gt(game_data.current_tetromino.y,
                   initial_y);  // Должна упасть ниже
  ck_assert_int_eq(game_data.state,
                   STATE_ATTACHING);  // И перейти в состояние присоединения
}
END_TEST

// Тест 6: Поворот фигуры
START_TEST(test_rotate) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  int initial_rotation = game_data.current_tetromino.rotation;
  userInput(Action, false);
  int new_rotation = game_data.current_tetromino.rotation;
  ck_assert_int_ne(initial_rotation, new_rotation);
}
END_TEST

// Тест 7: Пауза
START_TEST(test_pause) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  userInput(Pause, false);
  ck_assert_int_eq(game_data.state, STATE_PAUSE);
  ck_assert_int_eq(game_data.game_info.pause, 1);
  userInput(Pause, false);
  ck_assert_int_eq(game_data.state, STATE_MOVING);
  ck_assert_int_eq(game_data.game_info.pause, 0);
}
END_TEST

// Тест 8: Завершение игры
START_TEST(test_terminate) {
  userInput(Terminate, false);
  update_fsm();
  ck_assert_int_eq(game_data.state, STATE_GAME_OVER);
}
END_TEST

// Тест 9: Присоединение фигуры и очистка линии
START_TEST(test_attach_and_clear) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  // Заполняем линию вручную
  for (int i = 0; i < FIELD_WIDTH; ++i) {
    game_data.game_info.field[FIELD_HEIGHT - 1][i] = 1;
  }
  game_data.current_tetromino.y = FIELD_HEIGHT - 2;
  game_data.state = STATE_SHIFTING;
  update_fsm();  // Должно привести к ATTACHING
  ck_assert_int_eq(game_data.state, STATE_ATTACHING);
  update_fsm();  // Должно очистить линию и перейти в SPAWN
  ck_assert_int_eq(game_data.game_info.score, 100);  // 1 линия = 100 очков
  ck_assert_int_eq(game_data.state, STATE_SPAWN);
}
END_TEST

// Тест 10: Функция updateCurrentState
START_TEST(test_update_current_state) {
  GameInfo_t info = updateCurrentState();
  ck_assert_ptr_nonnull(info.field);
  ck_assert_ptr_nonnull(info.next);
  ck_assert_int_ge(info.score, 0);
  ck_assert_int_ge(info.level, 1);
}
END_TEST

// Тест 11: Ускоренное падение
START_TEST(test_drop_down) {
  userInput(Start, false);
  update_fsm();
  update_fsm();
  int initial_y = game_data.current_tetromino.y;
  userInput(Down, false);  // Ускоренное падение
  ck_assert_int_gt(game_data.current_tetromino.y, initial_y);
  ck_assert_int_eq(game_data.state, STATE_ATTACHING);
}
END_TEST

// Тест 12: Проверка коллизий
START_TEST(test_collision_detection) {
  userInput(Start, false);
  update_fsm();
  update_fsm();

  // Устанавливаем фигуру на край поля
  game_data.current_tetromino.x = FIELD_WIDTH - 1;
  bool result = move_tetromino(&game_data.current_tetromino, 1, 0);
  ck_assert_int_eq(result, false);  // Не должна двигаться за границу
}
END_TEST

// Тест 13: Обновление уровня и скорости
START_TEST(test_level_update) {
  game_data.game_info.score = 600;
  update_level_and_speed();
  ck_assert_int_eq(game_data.game_info.level, 2);
  ck_assert_int_lt(game_data.fall_interval, 1000);  // Скорость увеличилась
}
END_TEST

// Тест 14: Сохранение и загрузка рекорда
START_TEST(test_high_score) {
  game_data.game_info.high_score = 500;
  save_high_score();

  game_data.game_info.high_score = 0;  // Сбрасываем
  load_high_score();
  ck_assert_int_eq(game_data.game_info.high_score, 500);
}
END_TEST

// Тест 15: Матричные операции
START_TEST(test_matrix_operations) {
  int **matrix = allocate_matrix(5, 5);
  ck_assert_ptr_nonnull(matrix);

  clear_matrix(matrix, 5, 5);
  ck_assert_int_eq(matrix[0][0], 0);
  ck_assert_int_eq(matrix[4][4], 0);

  free_matrix(matrix, 5);
}
END_TEST

Suite *tetris_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Tetris Logic");
  tc_core = tcase_create("Core");

  // Добавляем фикстуры для установки и очистки
  tcase_add_checked_fixture(tc_core, setup, teardown);

  // Добавляем тесты
  tcase_add_test(tc_core, test_init_game);
  tcase_add_test(tc_core, test_start_and_spawn);
  tcase_add_test(tc_core, test_move_left);
  tcase_add_test(tc_core, test_move_right);
  tcase_add_test(tc_core, test_move_down);
  tcase_add_test(tc_core, test_rotate);
  tcase_add_test(tc_core, test_pause);
  tcase_add_test(tc_core, test_terminate);
  tcase_add_test(tc_core, test_attach_and_clear);
  tcase_add_test(tc_core, test_update_current_state);
  tcase_add_test(tc_core, test_drop_down);
  tcase_add_test(tc_core, test_collision_detection);
  tcase_add_test(tc_core, test_level_update);
  tcase_add_test(tc_core, test_high_score);
  tcase_add_test(tc_core, test_matrix_operations);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s = tetris_suite();
  SRunner *sr = srunner_create(s);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}
