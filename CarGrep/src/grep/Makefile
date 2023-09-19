CC = gcc -std=c11 -D_GNU_SOURCE
CFLAGS = -Wall -Wextra -Werror
GREP = s21_grep.c
clang_path	= ../../materials/build/scripts/

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) $(GREP) -o s21_grep

test:
	chmod u=rwx test.sh ;./test.sh

clean:
	rm -rf s21_grep
	rm -rf *.a *.out *.o

clang:
	cp $(clang_path).clang-format ./
	clang-format -n *.c *.h
	rm -rf .clang-format

clang_replace:
	cp $(clang_path).clang-format ./
	clang-format -i *.c *.h
	rm -rf .clang-format

check:
	cppcheck --enable=all --suppress=missingIncludeSystem ../../src/
	python3 ../../materials/linters/cpplint.py --extensions=c ../*/*.c
	CK_FORK=no leaks --atExit -- ./s21_grep

rebuild:
	make clean
	make all
	rm -rf  *.o *.a