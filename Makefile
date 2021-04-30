all: ising

ising: ising.c
	gcc -lm ising.c -o ising

