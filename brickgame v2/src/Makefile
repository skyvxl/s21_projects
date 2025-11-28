# Detect OS
UNAME_S := $(shell uname -s)

# Compiler and flags
CC = gcc
CXX = g++
CFLAGS = -std=c11 -g -Wall -Wextra -Wpedantic
CXXFLAGS = -std=c++20 -g -Wall -Wextra -Wpedantic
GCOV_FLAGS = --coverage

PREFIX ?= ../dist
INSTALL_BINDIR = $(PREFIX)/bin
INSTALL_LIBDIR = $(PREFIX)/lib
INSTALL_SHAREDIR = $(PREFIX)/share/brickgame

GTEST_DIR ?= /opt/homebrew/opt/googletest
GTEST_INC = $(GTEST_DIR)/include
GTEST_LIB = $(GTEST_DIR)/lib
GTEST_LIBS = -L$(GTEST_LIB) -lgtest -lgtest_main -lpthread
CXXFLAGS_TEST = $(CXXFLAGS) -I$(GTEST_INC)

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)  # macOS
    NCURSES_PATH = /opt/homebrew/opt/ncurses
    CFLAGS += -I$(NCURSES_PATH)/include
    CXXFLAGS += -I$(NCURSES_PATH)/include
    LDFLAGS = -lncurses -L$(NCURSES_PATH)/lib

    	LDFLAGS += \
    			-L/opt/homebrew/opt/ncurses/lib \
    			-I/opt/homebrew/include \
    			-L/opt/homebrew/lib

    else  # Linux
        LDFLAGS = -lncurses
    endif

	# Add C++ standard library for linking
	LDFLAGS += -lstdc++

# Library and executables
LIB_TARGET = s21_brick_game.a
GAME_EXE = tetris
TEST_EXE = snake_tests
QT_EXE = desktop_game

# Directories
OBJDIR = ../obj
TESTDIR = tests
DISTDIR = ../dist
REPORTDIR = ../report
DVI_DIR = dvi
QT_DIR = gui/desktop
BUILD_DIR = ../build

CLI_REPORT_DIR = $(REPORTDIR)/cli
DESKTOP_REPORT_DIR = $(REPORTDIR)/desktop
CLI_COVERAGE_INFO = $(CLI_REPORT_DIR)/cli.info
DESKTOP_COVERAGE_INFO = $(DESKTOP_REPORT_DIR)/desktop.info

# Source files for library (only game logic, no UI)
LIB_SRCS = $(wildcard brick_game/fsm/*.c) $(wildcard brick_game/tetris/*.c) $(wildcard brick_game/snake/*.cpp)
CLI_SRCS = $(wildcard gui/cli/*.c)
TEST_SRCS = $(wildcard $(TESTDIR)/*.cpp)
MAIN_SRC = main.c

# Object files
LIB_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(filter %.c,$(LIB_SRCS))) $(patsubst %.cpp,$(OBJDIR)/%.o,$(filter %.cpp,$(LIB_SRCS)))
CLI_OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(CLI_SRCS))
MAIN_OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(MAIN_SRC))
TEST_OBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(TEST_SRCS))

# Default target
all: $(LIB_TARGET)

# Build static library (only game logic)
$(LIB_TARGET): $(LIB_OBJS)
	ar rcs $@ $(LIB_OBJS)
	@echo "Library $(LIB_TARGET) created."

# Build CLI game executable
$(GAME_EXE): $(MAIN_OBJ) $(CLI_OBJS) $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) $(CLI_OBJS) $(LIB_TARGET) -o $(GAME_EXE) $(LDFLAGS)

# Build Qt desktop application
desktop: $(LIB_TARGET)
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ../src/$(QT_DIR) && make
	@echo "Desktop application built in $(BUILD_DIR)/BrickGame"

# Generic rule for C object files
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for C++ object files
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: brick_game/fsm/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: brick_game/tetris/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: brick_game/snake/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: brick_game/snake/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: gui/cli/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: tests/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS_TEST) -c $< -o $@

# Targets
install: build
	@mkdir -p $(INSTALL_BINDIR) $(INSTALL_LIBDIR) $(INSTALL_SHAREDIR)
	@cp $(LIB_TARGET) $(INSTALL_LIBDIR)/
	@cp $(GAME_EXE) $(INSTALL_BINDIR)/
	@if [ -f ../snake_high_score.txt ]; then \
		cp ../snake_high_score.txt $(INSTALL_SHAREDIR)/snake_high_score.txt; \
	fi

uninstall:
	@rm -f $(INSTALL_LIBDIR)/$(LIB_TARGET)
	@rm -f $(INSTALL_BINDIR)/$(GAME_EXE)
	@rm -f $(INSTALL_SHAREDIR)/snake_high_score.txt

clean:
	@rm -rf $(OBJDIR) $(LIB_TARGET) $(GAME_EXE) $(TEST_EXE)
	@rm -rf $(REPORTDIR) *.gcno *.gcda *.info
	@rm -rf $(DVI_DIR)/html $(DVI_DIR)/latex
	# @rm -rf $(BUILD_DIR)
	@echo "Cleaned all build artifacts."

dist: clean
	@mkdir -p $(DISTDIR)
	tar czf $(DISTDIR)/brickgame_v2.tar.gz -C .. src README.md

dvi:
	@mkdir -p $(DVI_DIR)
	@rm -rf $(DVI_DIR)/html
	@cd $(DVI_DIR) && doxygen Doxyfile

test: $(LIB_TARGET) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJS) $(LIB_TARGET) $(GTEST_LIBS) $(LDFLAGS) -o $(TEST_EXE)
	./$(TEST_EXE)

gcov_cli_build: clean
	@echo "[coverage] Building CLI game with instrumentation"
	@mkdir -p $(CLI_REPORT_DIR)
	$(MAKE) CFLAGS="$(CFLAGS) $(GCOV_FLAGS)" CXXFLAGS="$(CXXFLAGS) $(GCOV_FLAGS)" LDFLAGS="$(LDFLAGS) $(GCOV_FLAGS)" build
	@echo "[coverage] CLI build ready. Run ./$(GAME_EXE) to collect execution data."

gcov_cli_report:
	@if [ ! -x $(GAME_EXE) ]; then \
		echo "CLI binary not found. Run 'make gcov_cli_build' first."; \
		exit 1; \
	fi
	@if [ -z "$$(find $(OBJDIR) -name '*.gcda' -print -quit 2>/dev/null)" ]; then \
		echo "No coverage data found. Run the instrumented CLI game before generating a report."; \
		exit 1; \
	fi
	@mkdir -p $(CLI_REPORT_DIR)
	@rm -f $(CLI_COVERAGE_INFO)
	lcov --capture --directory $(OBJDIR) \
		--base-directory . \
		--output-file $(CLI_COVERAGE_INFO) \
		--ignore-errors mismatch,empty,unsupported,inconsistent,gcov \
		--rc branch_coverage=1 \
		--no-external
	genhtml --output-directory $(CLI_REPORT_DIR) \
		--branch-coverage \
		--function-coverage \
		--legend \
		--ignore-errors unsupported,inconsistent,corrupt,category \
		$(CLI_COVERAGE_INFO)
	@echo "[coverage] CLI report generated at $(CLI_REPORT_DIR)/index.html"

gcov_desktop_build: clean
	@echo "[coverage] Building desktop game with instrumentation"
	@mkdir -p $(DESKTOP_REPORT_DIR)
	$(MAKE) CFLAGS="$(CFLAGS) $(GCOV_FLAGS)" CXXFLAGS="$(CXXFLAGS) $(GCOV_FLAGS)" LDFLAGS="$(LDFLAGS) $(GCOV_FLAGS)" $(LIB_TARGET)
	@rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_FLAGS="$(CFLAGS) $(GCOV_FLAGS)" \
		-DCMAKE_CXX_FLAGS="$(CXXFLAGS) $(GCOV_FLAGS)" \
		-DCMAKE_EXE_LINKER_FLAGS="$(GCOV_FLAGS)" ../src/$(QT_DIR)
	cd $(BUILD_DIR) && make
	@echo "[coverage] Desktop build ready. Run $(BUILD_DIR)/BrickGame to collect execution data."

gcov_desktop_report:
	@if [ ! -x $(BUILD_DIR)/BrickGame ]; then \
		echo "Desktop binary not found. Run 'make gcov_desktop_build' first."; \
		exit 1; \
	fi
	@if [ -z "$$(find $(OBJDIR) $(BUILD_DIR) -name '*.gcda' -print -quit 2>/dev/null)" ]; then \
		echo "No coverage data found. Run the instrumented desktop game before generating a report."; \
		exit 1; \
	fi
	@mkdir -p $(DESKTOP_REPORT_DIR)
	@rm -f $(DESKTOP_COVERAGE_INFO)
	lcov --capture --directory $(OBJDIR) --directory $(BUILD_DIR) \
		--base-directory . \
		--output-file $(DESKTOP_COVERAGE_INFO) \
		--ignore-errors mismatch,empty,unsupported,inconsistent,gcov,source \
		--rc branch_coverage=1 \
		--no-external
	lcov --remove $(DESKTOP_COVERAGE_INFO) '*/BrickGame_autogen/*' \
		--output-file $(DESKTOP_COVERAGE_INFO) \
		--ignore-errors empty,unsupported,inconsistent
	genhtml --output-directory $(DESKTOP_REPORT_DIR) \
		--branch-coverage \
		--function-coverage \
		--legend \
		--ignore-errors unsupported,inconsistent,corrupt,category \
		$(DESKTOP_COVERAGE_INFO)
	@echo "[coverage] Desktop report generated at $(DESKTOP_REPORT_DIR)/index.html"

run: $(GAME_EXE)
	./$(GAME_EXE)

run_desktop: build_desktop
	$(BUILD_DIR)/BrickGame

build: $(GAME_EXE)

build_desktop: $(LIB_TARGET) desktop

cppcheck:
	cppcheck --enable=all --std=c++20 --check-level=exhaustive --disable=information --suppress=missingIncludeSystem --suppress=missingInclude --suppress=checkersReport ./

format_check:
	clang-format --style=Google --dry-run --Werror $$(find . -type f \( -name '*.c' -o -name '*.h' -o -name '*.cpp' \))

format:
	clang-format --style=Google -i $$(find . -type f \( -name '*.c' -o -name '*.h' -o -name '*.cpp' \))

.PHONY: all install uninstall clean dist dvi open_docs test run build valgrind cppcheck format_check format desktop run_desktop build_desktop \
	gcov_cli_build gcov_cli_report gcov_desktop_build gcov_desktop_report
