OBJS = main.o Cartridge.o CPU.o execute.o
TEST_OBJS = CPU_test.o
INCLUDE = -l boost_unit_test_framework
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

execute.o:	execute.cpp
	$(CC) $(CFLAGS) execute.cpp

Cartridge.o:	Cartridge.cpp
	$(CC) $(CFLAGS) Cartridge.cpp

CPU_test.o:	test/CPU_test.cpp
	$(CC) $(CFLAGS) CPU.cpp test/CPU_test.cpp

all_tests:	CPU_test.o
	$(CC) -o unit_test $(LFLAGS) $(INCLUDE) CPU.cpp execute.cpp test/CPU_test.cpp
	./unit_test --log_level=error

clean:
	rm -f *.o core
