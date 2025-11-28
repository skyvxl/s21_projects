#ifndef __DECIMAL_TEST_H__
#define __DECIMAL_TEST_H__

#include <check.h>
#include <limits.h>

#include "../s21_decimal.h"
#include "../utils/s21_utils.h"

/*
 !  - not created : not implemented
 ? - edit : under development
 * - done : completed and verified
 */

// * comparison
Suite *test_is_less(void);              // ?
Suite *test_is_less_or_equal(void);     // *
Suite *test_is_greater(void);           // *
Suite *test_is_greater_or_equal(void);  // *
Suite *test_is_equal(void);             // *
Suite *test_is_not_equal(void);         // *

// ? other
Suite *test_truncate(void);  // *
Suite *test_negate(void);    // *
Suite *test_floor(void);     // *
Suite *test_round(void);     // *

// ? conversion
Suite *test_from_decimal_to_float(void);  // *
Suite *test_from_decimal_to_int(void);    // *
Suite *test_from_int_to_decimal(void);    // *
Suite *test_from_float_to_decimal(void);  // *

// ? arithmetical
Suite *test_add(void);  // *
Suite *test_sub(void);  // ?
Suite *test_mul(void);  // *
Suite *test_div(void);  // ?

#endif  // __DECIMAL_TEST_H__