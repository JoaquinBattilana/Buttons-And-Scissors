FLAGS = -Wall -pedantic -std=c99
all:
	gcc -o tpe.exe main.c buttonsAndScissorsBack.h buttonsAndScissorsBack.c buttonsAndScissorsFront.h buttonsAndScissorsFront.c random.h random.c $(FLAGS)
clean:
	rm tpe.exe
