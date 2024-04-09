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

	std::cout << "AIMBOT: press F9 " << std::boolalpha << "-->[[" << AimbotOptions::bAimbot << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "SMOOTH VALUE: press F3 (-) or press F4 (+) \n" << "-->[[" << AimbotOptions::smoothValue << "]] \n";
	std::cout << "FOV: press F5 (-) or press F6 (+) \n" << "-->[[" << AimbotOptions::fovValue << "]] \n";
	std::cout << "TARGET LOCKING: press F2 \n" << std::boolalpha << "-->[[" << AimbotOptions::bTargetLock << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "--> press SUPPR to exit <-- \n";
}