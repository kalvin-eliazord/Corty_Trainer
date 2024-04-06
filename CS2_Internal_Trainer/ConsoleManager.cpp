#include "ConsoleManager.h"

void ConsoleManager::InitConsole()
{
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);

	PrintCheatOptions();
}

void ConsoleManager::DestroyConsole()
{
	if (file)
		fclose(file);

	FreeConsole();
}

void ConsoleManager::PrintCheatOptions()
{
	// Initialize a usable console
	if(file == nullptr)
		freopen_s(&file, "CONOUT$", "w", stdout);

	system("cls");

	std::cout << R"(
 _____            _           _____         _                 
/  __ \          | |         |_   _|       (_)                
| /  \/ ___  _ __| |_ _   _    | |_ __ __ _ _ _ __   ___ _ __ 
| |    / _ \| '__| __| | | |   | | '__/ _` | | '_ \ / _ \ '__|
| \__/\ (_) | |  | |_| |_| |   | | | | (_| | | | | |  __/ |   
 \____/\___/|_|   \__|\__, |   \_/_|  \__,_|_|_| |_|\___|_|   
                       __/ |                                  
                      |___/                 
		)" << "\n";

	std::cout << "-------------------------------------------------------------- \n";

	if (*GamePointer::gameStateIdPtr == GameChecker::inGameId)
		std::cout << "GAME STATUS: IN GAME, HAVE FUN \n";
	else
		std::cout << "GAME STATUS: WAITING FOR A GAME \n";

	std::cout << "AIMBOT   : press F9 " << std::boolalpha << "-->[[" << CheatManager::bAimbot   << "]] \n";

	if (CheatManager::bAimbot)
	{
		std::cout << "-------------------------------------------------------------- \n";
		std::cout << "SMOOTH VALUE: press F3 (-) or press F4 (+) \n" << "-->[[" << AimbotOptions::smoothValue << "]] \n";
		std::cout << "FOV: press F5 (-) or press F6 (+) \n" << "-->[[" << AimbotOptions::fovValue << "]] \n";
		std::cout << "TARGET LOCKING: press F2 \n" << std::boolalpha << "-->[[" << AimbotOptions::bTargetLock << "]] \n";
		std::cout << "-------------------------------------------------------------- \n";
	}

	std::cout << "--> press SUPPR to exit <-- \n";
}