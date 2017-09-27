OBJS = DTMF.o test.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG) -lsfml-system -lsfml-audio

bin : $(OBJS)
	$(CC) $(OBJS) -o bin $(LFLAGS)

test.o : test.cpp
	$(CC) $(CFLAGS) test.cpp

DTMF.o : DTMF.cpp DTMF.hpp
	$(CC) $(CFLAGS) DTMF.cpp

clean_all:
	\rm DTMF.o test.o bin

clean:
	\rm DTMF.o test.o

