CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic
CPPFLAGS = —enable=all —suppress=missingIncludeSystem

all: clean s21_cat

s21_cat:
	gcc $(CFLAGS) s21_cat.c -o s21_cat

test:
	bash tests_cat.sh

cpp:
	cppcheck $(CPPFLAGS) s21_cat.c
	cppcheck $(CPPFLAGS) s21_cat.h

clang:
	clang-format -style=Google -n -Werror s21_cat.*

clean:
	rm -rf s21_cat
