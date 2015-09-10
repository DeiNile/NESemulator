OBJS = main.o Cartridge.o CPU.o
#HEADERS = headers/Cartridge.h headers/CPU.h
SRC = Cartridge.cpp CPU.cpp
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all:	$(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o nes

main.o:	CPU.cpp Cartridge.cpp main.cpp 
	$(CC) $(CFLAGS) main.cpp

CPU.o:	CPU.cpp
	$(CC) $(CFLAGS) CPU.cpp

Cartridge.o:	Cartridge.cpp
	$(CC) $(CFLAGS) headers/Cartridge.h Cartridge.cpp

clean:
	rm -f *.o core