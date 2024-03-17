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
	clang-format -n s21_cat.c s21_cat.h

clean:
	rm -rf s21_cat
