CC=gcc
OBJ=obj/main.o obj/img_read.o
EXE=bin/PathFinder
OPT=-Wall -std=c99 -g



all : $(OBJ)
	$(CC) $(OPT) -o $(EXE) $(OBJ) -lm

obj/img_read.o: img_read.c img_read.h
	$(CC) $(OPT) -o $@ -c $<

obj/%.o : %.c
	$(CC) $(OPT) -o $@ -c $<

clean:
	rm *- *.o $(EXE) $(OBJ)
