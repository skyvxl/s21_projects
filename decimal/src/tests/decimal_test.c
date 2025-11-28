#include "decimal_test.h"

#include <check.h>

int main() {
  int failed = 0;
  Suite *decimal_test[] = {test_add(),
                           test_sub(),
                           test_mul(),
                           test_div(),
                           test_is_less(),
                           test_is_less_or_equal(),
                           test_is_greater(),
                           test_is_greater_or_equal(),
                           test_is_equal(),
                           test_is_not_equal(),
                           test_from_int_to_decimal(),
                           test_from_float_to_decimal(),
                           test_from_decimal_to_int(),
                           test_from_decimal_to_float(),
                           test_floor(),
                           test_round(),
                           test_truncate(),
                           test_negate(),
                           NULL};

  for (int i = 0; decimal_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(decimal_test[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  if (failed == 0) {
    printf("\033[1;32mAll tests passed\033[0m\n");
  } else {
    printf("\033[1;31mFailed tests: %d\033[0m\n", failed);
  }
  return (failed == 0) ? 0 : 1;
}