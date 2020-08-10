CC=g++
CFLAGS= -g -Wall
TARGET=assn3

all:$(TARGET)

clean:
	delete $(TARGET)

assn3:assn3.cpp Block.cpp Tetrimino.cpp Gamemanager.cpp
	$(CC) -o $@ assn3.cpp Block.cpp Tetrimino.cpp GameManager.cpp

