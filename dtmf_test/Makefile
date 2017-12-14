OBJS = Source.o DTMF.o RLRecorder.o DTMF_type.o goertzel.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11 -lpthread
LFLAGS = -Wall $(DEBUG) -lsfml-system -lsfml-audio -lpthread

bin : $(OBJS)
	$(CC) $(OBJS) -o bin $(LFLAGS)

Source.o : Source.cpp DTMF.hpp
	$(CC) $(CFLAGS) Source.cpp

DTMF.o : DTMF.cpp DTMF.hpp DTMF_type.hpp DTMF_type.hpp RLRecorder.hpp
	$(CC) $(CFLAGS) DTMF.cpp

RLRecorder.o : RLRecorder.cpp RLRecorder.hpp DTMF_type.hpp goertzel.hpp
	$(CC) $(CFLAGS) RLRecorder.cpp

goertzel.o : goertzel.hpp goertzel.cpp
	$(CC) $(CFLAGS) goertzel.cpp

DTMF_type.o : DTMF_type.hpp DTMF_type.cpp
	$(CC) $(CFLAGS) DTMF_type.cpp

clean_all:
	\rm $(OBJS) bin

clean:
	\rm $(OBJS)

