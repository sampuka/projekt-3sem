OBJS = Source.o DLL.o DTMF.o rc_fft.o RLRecorder.o DTMF_type.o goertzel.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11 -lpthread
LFLAGS = -Wall $(DEBUG) -lsfml-system -lsfml-audio -lpthread

bin : $(OBJS)
	$(CC) $(OBJS) -o bin $(LFLAGS)

Source.o : Source.cpp
	$(CC) $(CFLAGS) Source.cpp

DLL.o : DLL.cpp DLL.hpp DTMF.hpp
	$(CC) $(CFLAGS) DLL.cpp

DTMF.o : DTMF.cpp DTMF.hpp DTMF_type.hpp DTMF_type.hpp RLRecorder.hpp
	$(CC) $(CFLAGS) DTMF.cpp

RLRecorder.o : RLRecorder.cpp RLRecorder.hpp DTMF_type.hpp rc_fft.hpp goertzel.hpp
	$(CC) $(CFLAGS) RLRecorder.cpp

rc_fft.o : rc_fft.hpp rc_fft.cpp
	$(CC) $(CFLAGS) rc_fft.cpp

goertzel.o : goertzel.hpp goertzel.cpp
	$(CC) $(CFLAGS) goertzel.cpp

DTMF_type.o : DTMF_type.hpp DTMF_type.cpp
	$(CC) $(CFLAGS) DTMF_type.cpp

clean_all:
	\rm $(OBJS) bin

clean:
	\rm $(OBJS)

