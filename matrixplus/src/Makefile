CC = g++
SRC = s21_matrix_oop.cpp
OBJ = s21_matrix_oop.o
OBJ_TEST = s21_matrix_test.o
LIB = s21_matrix_oop.a
FLAGS= -Wall -Werror -Wextra -std=c++17
TESTFLAGS= -pthread -lgtest -lgmock
CHECK_FLAG = -lstdc++
LCOVFLAGS = --ignore-errors inconsistent --ignore-errors mismatch
OPEN= open

OS := $(shell uname -s)
ifeq ($(OS), Linux)
	OPEN = xdg-open
	TESTFLAGS= -pthread -lgtest -lgmock -lpthread
	FLAGS= -Wall -Werror -Wextra -std=c++17 -fPIC -fprofile-arcs -ftest-coverage -lstdc++fs
endif

all: clean gcov_report

s21_matrix_oop.a:
	$(CC) -c $(FLAGS) $(SRC) -o $(OBJ)
	ar -rcs $(LIB) $(OBJ)
	ranlib $(LIB)
	rm -rf *.o

test: clean s21_matrix_oop.a
	$(CC) $(FLAGS) -c test.cpp -o test.o
	$(CC) $(FLAGS) $(CHECK_FLAG) test.o $(LIB) $(TESTFLAGS) -o test
	rm -rf *.o

gcov_report: add_coverage_flag test
	./test
	lcov -t "test" -o test.info -c -d ./ --ignore-errors empty --ignore-errors inconsistent --ignore-errors mismatch --include "*/s21_matrix_oop.*"
	lcov --remove test.info 'test.cpp' -o test.info --ignore-errors unused
	genhtml -o report test.info $(LCOVFLAGS)
	$(OPEN) report/index.html

add_coverage_flag:
	$(eval FLAGS += --coverage)

clean:
	rm -rf *.out *.o *.gcno *.info s21_matrix_oop.a *.gcda *.a *.o *.txt
	rm -rf report
	rm -rf test

clang-format:
	clang-format -i *.cpp *.h