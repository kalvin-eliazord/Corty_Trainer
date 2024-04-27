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

void ConsoleManager::PrintErrorPtrInit(std::map <std::string, intptr_t > pPointers)
{
	if (!file)
		InitConsole();

	system("cls");
	PrintMyName();

	std::cout << "[!] >> Pointer initialization error << \n";

	for (auto pointer : pPointers)
	{
		if (!pointer.second)
			std::cout << "[!] ";
		else
			std::cout << "[+]";

			std::cout << pointer.first << " = " << pointer.second << "\n";
	}

	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] --> press SUPPR to exit <-- \n";
}

std::string ConsoleManager::GetTargetedPart()
{
	 return CheatHKeys::bHeadPos ? "HEAD" : "PELVIS";
}

void ConsoleManager::PrintCheatOptions()
{
	system("cls");
	PrintMyName();
	std::cout << "[+] F1 to SWITCH targeted part --> [[" << GetTargetedPart() << "]] \n";
	std::cout << "[+] AIMBOT: RIGHT click " << std::boolalpha << "-->[[" << CheatHKeys::bAimbot << "]] \n";
	std::cout << "[+] ESP   : press F8 " << std::boolalpha << "-->[[" << CheatHKeys::bESP << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] SMOOTH VALUE  : press F3 (-) or press F4 (+) " << "-->[[" << CheatHKeys::smoothValue << "]] \n";
	std::cout << "[+] FOV           : press F5 (-) or press F6 (+) " << "-->[[" << CheatHKeys::fovValue << "]] \n";
	std::cout << "[+] TARGET LOCKING: press F2 " << std::boolalpha << "                    -->[[" << CheatHKeys::bTargetLock << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] --> [[ press SUPPR to exit] ] <-- \n";
}