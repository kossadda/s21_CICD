CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
CPPFLAGS = —enable=all —suppress=missingIncludeSystem

all: clean s21_grep

s21_grep:
	gcc $(CFLAGS) s21_grep.c -o s21_grep

test: 
	bash tests.sh

test_leaks:
	bash test_leaks.sh

cpp:
	cppcheck $(CPPFLAGS) s21_grep.c
	cppcheck $(CPPFLAGS) s21_grep.h

clang:
	clang-format -style=Google -n -Werror s21_grep.*

clean:
	rm -rf s21_grep
