all: semControl.o semWrite.o
	gcc -o control semControl.o
	gcc -o write semWrite.o
semControl.o: semControl.c semaphore.h
	gcc -c semControl.c
semWrite.o: semWrite.c semaphore.h
	gcc -c semWrite.c
