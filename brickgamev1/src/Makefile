# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic
GCOV_FLAGS = --coverage
LDFLAGS = -lncurses
LDFLAGS_TEST = -lcheck -lsubunit -lm

# Library and executables
LIB_TARGET = s21_brick_game.a
GAME_EXE = tetris
TEST_EXE = test_tetris

# Directories
OBJDIR = ../obj
TESTDIR = tests
DISTDIR = ../dist
REPORTDIR = ../report
DVI_DIR = dvi

# Source files
LIB_SRCS = $(wildcard brick_game/tetris/*.c) $(wildcard gui/cli/*.c)
TEST_SRCS = $(wildcard $(TESTDIR)/*.c)
MAIN_SRC = main.c

# Object files
LIB_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(LIB_SRCS))
MAIN_OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(MAIN_SRC))

# Default target
all: $(LIB_TARGET)

# Build static library
$(LIB_TARGET): $(LIB_OBJS)
	ar rcs $@ $(LIB_OBJS)
	@echo "Library $(LIB_TARGET) created."

# Build game executable
$(GAME_EXE): $(MAIN_OBJ) $(LIB_TARGET)
	$(CC) $(CFLAGS) $(MAIN_OBJ) -L. -l:$(LIB_TARGET) -o $(GAME_EXE) $(LDFLAGS)

# Generic rule for object files
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: brick_game/tetris/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: gui/cli/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Targets
install: build
	@mkdir -p $(DISTDIR)/bin
	@cp $(GAME_EXE) $(DISTDIR)/bin/

uninstall:
	@rm -f $(DISTDIR)/bin/$(GAME_EXE)

clean:
	@rm -rf $(OBJDIR) $(LIB_TARGET) $(GAME_EXE) $(TEST_EXE)
	@rm -rf $(REPORTDIR) *.gcno *.gcda *.info
	@rm -rf $(DVI_DIR)/html $(DVI_DIR)/latex

dist:
	@mkdir -p $(DISTDIR)
	@tar czf $(DISTDIR)/brickgame_tetris-1.0.tar.gz .

dvi:
	@mkdir -p $(DVI_DIR)
	@rm -rf $(DVI_DIR)/html $(DVI_DIR)/latex $(DVI_DIR)/*.png $(DVI_DIR)/*.dot $(DVI_DIR)/*.map
	@cd $(DVI_DIR) && doxygen Doxyfile

test: $(LIB_TARGET)
	$(CC) $(CFLAGS) $(TEST_SRCS) -L. -l:$(LIB_TARGET) $(LDFLAGS_TEST) $(LDFLAGS) -o $(TEST_EXE)
	./$(TEST_EXE)

gcov_report: clean
	@mkdir -p $(REPORTDIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(LIB_SRCS) $(MAIN_SRC) -o $(GAME_EXE) $(LDFLAGS)
	./$(GAME_EXE)
	lcov --capture --directory . \
		--output-file $(REPORTDIR)/tetris.info \
		--ignore-errors mismatch \
		--rc branch_coverage=1 \
		--no-external \
		--test-name "$(LIB_TARGET)"
	genhtml --output-directory $(REPORTDIR) \
		--branch-coverage \
		--function-coverage \
		--legend \
		--title "$(LIB_TARGET) Code Coverage Report" \
		$(REPORTDIR)/tetris.info
	@rm -rf *.gcno *.gcda

run: $(GAME_EXE)
	./$(GAME_EXE)

build: $(GAME_EXE)

valgrind: test
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_EXE)

cppcheck:
	cppcheck --enable=all --std=c11 --check-level=exhaustive --disable=information --suppress=missingIncludeSystem --suppress=missingInclude --suppress=checkersReport ./

format_check:
	clang-format --style=Google --dry-run --Werror $$(find . -type f \( -name '*.c' -o -name '*.h' \))

format:
	clang-format --style=Google -i $$(find . -type f \( -name '*.c' -o -name '*.h' \))

.PHONY: all install uninstall clean dist dvi open_docs test gcov_report run build valgrind cppcheck format_check format
