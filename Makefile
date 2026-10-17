compile:
	gcc -lreadline main.c input.c builtin/builtin.c history.c exec.c builtin/sudoku.c builtin/carlo.c -o unixsh

dev: compile
	./unixsh

run:
	./unixsh
