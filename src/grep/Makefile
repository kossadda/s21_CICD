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
	clang-format -n s21_grep.c s21_grep.h

clean:
	rm -rf s21_grep
