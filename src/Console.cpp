#include "headers/Console.hpp"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


Console::Console()
{
	cart = NULL;
	cpu  = new CPU(this);
	ppu  = new PPU(this);
}

Console::Console(Cartridge *cartridge)
{
	cart = cartridge;
	cpu = new CPU(cart, this);
	ppu = new PPU(this);
}

Console::~Console()
{
	// delete ppu;
	// delete cpu;
	// delete cart;
}

/*
 * Initializes the CPU and PPU with the provided cartridge.
 *
 * @param cartridge The cartridge that stores all the game data
 */
void Console::load_cartridge(Cartridge *cartridge)
{
	cart = cartridge;
	cpu = new CPU(cart, this);
	ppu = new PPU(this);
}

/*
 * Starts executing the program code that is loaded into the cpu,
 * thus essentially loading up the game and running it.
 * If no cartridge is provided then the game cannot be ran.
 */
void Console::power_on()
{
	// struct sigaction;
	// struct sigIntHandler;

	// sigIntHandler.sa_handler = power_off;
	// sigemptyset(&sigIntHandler.sa_mask);
	// sigIntHandler.sa_flags = 0;
	// sigaction(SIGINT, &sigIntHandler, NULL);

	if (cart == NULL) {
		std::cerr << "ERROR: No Cartridge was loaded." << std::endl << 
			"Exiting..." << std::endl;
		return;
	}
	cpu->set_PC(0xC000);
	for (;;) {
		cpu->fetch_and_execute();
	}
}

// void Console::power_off(int signal)
// {
// 	std::cout << "Exiting..." << std::endl;
// 	delete ppu;
// 	delete cpu;
// 	delete cart;

// 	exit(1);
// }

void Console::reset()
{
	// cpu->reset();
}

/*
 * Retrives the CPU.
 * This is needed so that the CPU and PPU can effectively communicate with
 * each other.
 */
CPU *Console::get_cpu()
{ 
	return cpu;
}

/*
 * Retrives the PPU.
 * This is needed so that the CPU and PPU can effectively communicate with
 * each other.
 */
PPU *Console::get_ppu()
{
	return ppu;
}