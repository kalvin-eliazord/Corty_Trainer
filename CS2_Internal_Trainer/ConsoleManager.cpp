#include "ConsoleManager.h"

ConsoleManager::ConsoleManager()
{
	// Initialize a usable console
	AllocConsole();
	freopen_s(&this->file, "CONOUT$", "w", stdout);
}

ConsoleManager::~ConsoleManager()
{
	if (this->file)
		fclose(file);

	FreeConsole();
}

void ConsoleManager::PrintCheatOptions()
{
	system("cls");

	if(*GameChecker::gameStateIdPtr == GameChecker::inGameId)
		std::cout << "IN GAME, HAVE FUN. \n";
	else 
		std::cout << "WAITING FOR A GAME. \n";

	std::cout << "------------------- \n";
	std::cout << "SMOOTH VALUE: press F3 (-) or press F4 (+) \n" << "-->[[" << AimbotOptions::smoothValue << "]]" << "\n";
	std::cout << "FOV: press F5 (-) or press F6 (+) \n" << "-->[[" << AimbotOptions::fovValue << "]]" << "\n";
	std::cout << "TARGET LOCKING: press F2 \n" << std::boolalpha << "-->[[" << AimbotOptions::bTargetLock << "]] " << "\n";
}