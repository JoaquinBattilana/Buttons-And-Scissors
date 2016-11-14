FLAGS = -Wall -pedantic -std=c99
EXEC_NAME = tpe.exe
all:
	gcc -o $(EXEC_NAME) main.c buttonsAndScissorsBack.h buttonsAndScissorsBack.c buttonsAndScissorsFront.h buttonsAndScissorsFront.c random.h random.c $(FLAGS)

clean:
	rm $(EXEC_NAME)
