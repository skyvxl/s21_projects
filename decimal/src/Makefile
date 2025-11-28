CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -lm

ARF_PATH=./arithmetical
COMP_PATH=./comparison
CONV_PATH=./conversion
OTH_PATH=./other
UTIL_PATH=./utils

SRC=$(wildcard $(ARF_PATH)/*.c $(COMP_PATH)/*.c $(CONV_PATH)/*.c $(OTH_PATH)/*.c $(UTIL_PATH)/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))

UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
	TEST_FLAGS=-lcheck -lpthread -lsubunit -lm
else
	TEST_FLAGS=-lcheck
endif

TEST_PATH=./tests
TEST_SRC=$(wildcard $(TEST_PATH)/*.c $(TEST_PATH)/*/*.c)

all: clean s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rcs $@ $^

clean:
	rm -rf *.a
	rm -rf *.gcno *.gcda test.info report
	rm -rf $(OBJ)
	rm -rf s21_decimal_test
	find . -name "*.gcno" -type f -delete
	find . -name "*.gcda" -type f -delete
	find . -name "*.o" -type f -delete
	rm -rf gcov_temp

style:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=Google -Werror -i

cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 --language=c --platform=unix64 --suppress=missingIncludeSystem $(SRC) $(TEST_SRC)

test: s21_decimal.a
	$(CC) $(CFLAGS) $(SRC) $(TEST_SRC) -o s21_decimal_test $(TEST_FLAGS)
	./s21_decimal_test

valgrind_test: s21_decimal.a test
	valgrind --leak-check=full ./s21_decimal_test

gcov_report: clean
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage $(SRC) $(TEST_SRC) $(TEST_FLAGS) -o s21_decimal_test
	./s21_decimal_test
	lcov --rc lcov_branch_coverage=1 -t "s21_decimal" -o report.info -c -d .
	genhtml -o report report.info --branch-coverage --function-coverage
	rm -rf *.gcda *.gcno report.info