OBJS = DTMF.o test.o rc_fft.o RLRecorder.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG) -lsfml-system -lsfml-audio

bin : $(OBJS)
	$(CC) $(OBJS) -o bin $(LFLAGS)

test.o : test.cpp
	$(CC) $(CFLAGS) test.cpp

DTMF.o : DTMF.cpp DTMF.hpp DTMF_type.hpp DTMF_type.hpp
	$(CC) $(CFLAGS) DTMF.cpp

RLRecorder.o : RLRecorder.cpp RLRecorder.hpp DTMF_type.hpp
	$(CC) $(CFLAGS) RLRecorder.cpp

rc_fft.o : rc_fft.hpp rc_fft.cpp
	$(CC) $(CFLAGS) rc_fft.cpp

clean_all:
	\rm DTMF.o test.o rc_fft.o RLRecorder.o bin

clean:
	\rm DTMF.o test.o rc_fft.o RLRecorder.o

