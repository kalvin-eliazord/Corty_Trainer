#include "ConsoleManager.h"

void ConsoleManager::InitConsole()
{
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);
}

void ConsoleManager::DestroyConsole()
{
	if (file)
		fclose(file);

	FreeConsole();
}

void ConsoleManager::PrintMyName()
{
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
}


void ConsoleManager::PrintErrorPtrInit()
{
	if (!file)
		InitConsole();

	system("cls");
	PrintMyName();

	std::cout << " >> Pointer initialization error << \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "--> press SUPPR to exit <-- \n";
}

void ConsoleManager::PrintCheatOptions()
{
	system("cls");
	PrintMyName();
	std::cout << "--> [[ RIGHT CLICK TO USE THE AIMBOT! ]] <-- \n";
	std::cout << "AIMBOT: press F9 " << std::boolalpha << "-->[[" << CheatOptions::bAimbot << "]] \n";
	std::cout << "ESP   : press F8 " << std::boolalpha << "-->[[" << CheatOptions::bESP << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "SMOOTH VALUE  : press F3 (-) or press F4 (+) " << "-->[[" << CheatOptions::smoothValue << "]] \n";
	std::cout << "FOV           : press F5 (-) or press F6 (+) " << "-->[[" << CheatOptions::fovValue << "]] \n";
	std::cout << "TARGET LOCKING: press F2 " << std::boolalpha << "                       -->[[" << CheatOptions::bTargetLock << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "--> [[ press SUPPR to exit] ] <-- \n";
}