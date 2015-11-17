MAIN = main.o
OBJS = Cartridge.o CPU.o execute.o Cartridge_header.o NROM_Mapper.o
TEST_OBJS = Cartridge_header_test.o Cartridge_test.o CPU_test.o NROM_Mapper_test.o
BOOST = -l boost_unit_test_framework
SRC = Cartridge.cpp CPU.cpp
CC = g++
DEBUG = -g
MATH = -l m
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all:	$(OBJS) $(MAIN)
	$(CC) $(LFLAGS) $(OBJS) $(MAIN) -o nes

main.o:	CPU.o Cartridge.o main.cpp
	$(CC) $(CFLAGS) main.cpp

CPU.o:	execute.o CPU.cpp
	$(CC) $(CFLAGS) CPU.cpp

execute.o:	execute.cpp
	$(CC) $(CFLAGS) execute.cpp

Cartridge.o:	Cartridge_header.o Cartridge.cpp
	$(CC) $(CFLAGS) Cartridge.cpp

Cartridge_header.o:	Cartridge_header.cpp
	$(CC) $(CFLAGS) Cartridge_header.cpp

egg.o:	headers/NROM_Mapper.hpp
	$(CC) $(CFLAGS) headers/NROM_Mapper.hpp

NROM_Mapper.o:	Cartridge.o Cartridge_header.o NROM_Mapper.cpp egg.o
	$(CC) $(CFLAGS) NROM_Mapper.cpp
#$(MATH)

#
# THERE IS NO NEED TO SPECIFY OBJECT FILE WHEN COMPILING - LNKER WILL FIGURE IT OUT FOR ITSELF
#


CPU_test.o:	CPU.o execute.o test/CPU_test.cpp
	$(CC) $(CFLAGS) test/CPU_test.cpp

Cartridge_header_test.o:	Cartridge_header.o test/Cartridge_header_test.cpp
	$(CC) $(CFLAGS) test/Cartridge_header_test.cpp

Cartridge_test.o:	Cartridge.o test/Cartridge_test.cpp
	$(CC) $(CFLAGS) test/Cartridge_test.cpp

NROM_Mapper_test.o:	NROM_Mapper.o Cartridge.o test/Cartridge_test.cpp
	$(CC) $(CFLAGS) test/NROM_Mapper_test.cpp

all_tests:	$(OBJS) $(TEST_OBJS)
	$(CC) $(LFLAGS) $(BOOST) $(OBJS) $(TEST_OBJS) -o unit_test
	./unit_test --log_level=error

clean:
	rm -f *.o core nes unit_test
