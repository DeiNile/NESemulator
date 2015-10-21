OBJS = main.o Cartridge.o CPU.o execute.o Cartridge_header.o
TEST_OBJS = CPU_test.o Cartridge_header_test.o CPU.o execute.o Cartridge_header.o
INCLUDE = -l boost_unit_test_framework
SRC = Cartridge.cpp CPU.cpp
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all:	$(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o nes

main.o:	CPU.o Cartridge.o
	$(CC) $(CFLAGS) main.cpp

CPU.o:	execute.o
	$(CC) $(CFLAGS) CPU.cpp

execute.o:	
	$(CC) $(CFLAGS) execute.cpp

Cartridge.o:	Cartridge_header.o
	$(CC) $(CFLAGS) Cartridge.cpp

Cartridge_header.o:	
	$(CC) $(CFLAGS) Cartridge_header.cpp

CPU_test.o:	
	$(CC) $(CFLAGS) CPU.cpp execute.cpp test/CPU_test.cpp

Cartridge_header_test.o:	
	$(CC) $(CFLAGS) Cartridge_header.cpp test/Cartridge_header_test.cpp

all_tests:	$(TEST_OBJS)
	$(CC) $(LFLAGS) $(INCLUDE) $(TEST_OBJS) -o unit_test
	./unit_test --log_level=error

clean:
	rm -f *.o core nes unit_test
