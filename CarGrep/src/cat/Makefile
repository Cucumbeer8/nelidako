SHELL := /bin/bash

Flags = -Werror -Wall -Wextra
gcc = gcc $(Flags)
clang_path = ../../materials/build/scripts/
check_path = ../../materials/linters
OB = s21_cat


OS := $(shell uname -s)
ifeq ($(OS), Darwin)
 gcc += -D OS_MAC
endif
ifeq ($(OS), Linux)
 gcc += -D OS_LINUX
endif


all: clean $(OB)
clear:
	clear
clean:
	rm -rf tests/$(OB) tests/*.out
	rm -rf $(OB) *.out
	rm -rf .clang-format
testing: clear all
	mv $(OB) tests
	cd tests; chmod u=rwx test_func_cat.sh ; sh test_func_cat.sh
clang:
	cp $(clang_path).clang-format ./
	clang-format -n *.c *.h
	rm -rf .clang-format
clang_replace:
	cp $(clang_path).clang-format ./
	clang-format -i *.c *.h
	rm -rf .clang-format
s21_cat:
	$(gcc) s21_cat.c $(cmn) -std=c11 -o s21_cat