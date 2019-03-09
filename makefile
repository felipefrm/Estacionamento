########## Makefile ############

all: main.o entradaSaida.o configEstacionamento.o manobras.o
	gcc entradaSaida.o configEstacionamento.o manobras.o main.o -o tp

main.o: main.c entradaSaida.h configEstacionamento.h manobras.h
	gcc -g -c main.c
entradaSaida.o: entradaSaida.h entradaSaida.c
	gcc -g -c entradaSaida.c
configEstacionamento.o: configEstacionamento.h configEstacionamento.c
	gcc -g -c configEstacionamento.c
manobras.o: manobras.h manobras.c
	gcc -g -c manobras.c
