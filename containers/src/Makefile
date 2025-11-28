FLAGS = -Wall -Werror -Wextra -std=c++20 -ggdb -D_GLIBCXX_DEBUG -fsanitize=address
LIBS = -lgtest -lgtest_main -pthread
TEST_DIR = tests
BUILD_DIR = build
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_SRCS))

# --------------------------------------------------------------
# Progress bar configuration
TOTAL := $(words $(TEST_OBJS))
DONE := 0

# Dynamic bar width (80% of terminal width minus padding)
TERM_WIDTH := $(shell stty size 2>/dev/null | awk '{print $$2}')
TERM_WIDTH := $(if $(TERM_WIDTH),$(TERM_WIDTH),80)
BAR_WIDTH  := $(shell expr $(TERM_WIDTH) \* 80 / 100 - 20)

# Function to print progress bar
define print_bar
FILLED=$$(( ($1 * $(BAR_WIDTH)) / $2 )); \
UNFILLED=$$(( $(BAR_WIDTH) - FILLED )); \
BAR=$$(printf "%0.s▓" $$(seq 1 $$FILLED)); \
SP=""; \
if [ $$UNFILLED -gt 0 ]; then \
  SP=$$(printf "%0.s░" $$(seq 1 $$UNFILLED)); \
fi; \
PCT=$$(( 100 * $1 / $2 )); \
printf "\r$3 [$$BAR$$SP] %3d%%" $$PCT; \
if [ $1 -eq $2 ]; then printf "\n"; fi
endef
# --------------------------------------------------------------

all: clean test

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	@g++ $(FLAGS) -c $< -o $@
	$(eval DONE := $(shell expr $(DONE) + 1))
	@$(call print_bar,$(DONE),$(TOTAL),compilation progress:)

test: $(TEST_OBJS)
	@echo
	@$(call print_bar,1,2,linking executable: )
	@g++ $(FLAGS) -o $(BUILD_DIR)/test.out $(TEST_OBJS) $(LIBS)
	@$(call print_bar,2,2,linking executable: )
	@echo
	@./$(BUILD_DIR)/test.out

valgrind: test
	@echo
	@echo "Running valgrind check..."
	@valgrind --leak-check=full --track-origins=yes --show-reachable=yes -s ./$(BUILD_DIR)/test.out

clean:
	@rm -rf $(BUILD_DIR)/*

format_check:
	@cd .. && clang-format --style=Google --dry-run --Werror $$(find ./src -type f \( -name '*.cpp' -o -name '*.tpp' -o -name '*.hpp' -o -name '*.h' \))

format:
	@cd .. && clang-format --style=Google -i $$(find ./src -type f \( -name '*.cpp' -o -name '*.tpp' -o -name '*.hpp' -o -name '*.h' \))


.PHONY: all test valgrind clean docker_shell

docker-shell:

include ./docker/docker.mk