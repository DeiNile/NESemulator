OBJS = main.o Cartridge.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all:	$(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o nes

main.o:	headers/Cartridge.h Cartridge.cpp main.cpp 
	$(CC) $(CFLAGS) main.cpp

Cartridge.o:	headers/Cartridge.h Cartridge.cpp
	$(CC) $(CFLAGS) headers/Cartridge.h Cartridge.cpp

clean:
	rm -f *.o core