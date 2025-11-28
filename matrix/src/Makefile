FLAGS=-Wall -Wextra -Werror -std=c11 -lm
TEST_FLAGS=-lcheck -lsubunit -lm

BUILD_PATH=../
EXE=test.out

SRC_DIR=.
TEST_DIR=./tests
OBJ_DIR=../build

# Исходные файлы
SRC_C_FILES := $(wildcard $(SRC_DIR)/functions/*.c)
OBJS_C_FILES := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_C_FILES:.c=.o)))

all: clean s21_matrix.a

s21_matrix.a: mkdir $(OBJS_C_FILES)
	ar rcs s21_matrix.a $(OBJS_C_FILES)

mkdir:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/functions/%.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	rm -rf *.a
	rm -rf *.gcno *.gcda coverage.info test.out
	rm -rf $(OBJ_DIR)
	rm -rf s21_matrix_test report
	cd .. && rm -rf test_io

cppcheck:
	cppcheck --enable=all --inconclusive --std=c11 --language=c --platform=unix64 --check-level=exhaustive --suppress=missingIncludeSystem $(SRC_DIR) $(TEST_DIR)

style_check:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=Google -Werror -n

style_fix:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -style=Google -Werror -i

test: s21_matrix.a
	$(CC) $(CFLAGS) $(TEST_DIR)/*.c s21_matrix.a -o s21_matrix_test $(TEST_FLAGS)
	./s21_matrix_test

gcov_report: clean
	$(CC) $(CFLAGS) --coverage $(SRC_C_FILES) $(TEST_DIR)/*.c $(TEST_FLAGS) -o $(BUILD_PATH)$(EXE)
	./$(EXE)
	lcov -t "s21_matrix" --rc lcov_branch_coverage=all -o report.info -c -d . --exclude "*/tests/*"
	genhtml --branch-coverage -o report report.info
	rm -rf *.gcda *.gcno report.info
	open report/index.html

final_check:
	cd .. && docker run --rm -v .:/project checkimages/matrix

rebuild: clean all

.PHONY: all clean test gcov_report rebuild cppcheck style_check style_fix final_check