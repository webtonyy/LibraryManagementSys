CC = gcc
INCLUDE = ./include
BUILD = ./build
MAIN = ./main
SOURCE = ./src

ALL:
	$(CC) -c $(SOURCE)/catalogo.c -I $(INCLUDE)/* -o $(BUILD)/catalogo.o
	$(CC) $(MAIN)/main.c $(BUILD)/catalogo.o -I $(INCLUDE)/* -o biblioteca

RUN:
	./biblioteca

CLEAN:
	rm $(BUILD)/*
	rm biblioteca
