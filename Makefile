TARGET: main

all: main.o simulation.o userCommand.o
	gcc main.o simulation.o userCommand.o -o main

main.o: main.c
	gcc -c main.c -o main.o

userCommand.o: userCommand.c userCommand.h
	gcc -c userCommand.c -o userCommand.o

simulation.o: Simulation.c Simulation.h
	gcc -c Simulation.c -o simulation.o

clean:
	rm *.o ./main