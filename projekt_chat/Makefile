OBJS = DLL.o test.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG)

bin : $(OBJS)
	$(CC) $(OBJS) -o bin $(LFLAGS)

test.o : test.cpp
	$(CC) $(CFLAGS) test.cpp

DTMF.o : DLL.cpp DLL.hpp
	$(CC) $(CFLAGS) DLL.cpp

clean_all:
	\rm DLL.o test.o bin

clean:
	\rm DLL.o test.o

