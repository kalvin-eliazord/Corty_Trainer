#include "ConsoleCheatMenu.h"

void ConsoleCheatMenu::InitConsole()
{
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);

	ConsoleCheatMenu::PrintCheatOptions();
}

void ConsoleCheatMenu::DestroyConsole()
{
	if (file)
		fclose(file);

	FreeConsole();
}

void ConsoleCheatMenu::PrintMyName()
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
		)" << '\n';
	std::cout << "-------------------------------------------------------------- \n";
}

void ConsoleCheatMenu::PrintErrorPtrInit(const std::map <std::string_view, intptr_t >& pPointers)
{
	if (!file)
		InitConsole();

	system("cls");
	PrintMyName();

	std::cout << "[+] >> GamePointers initialization error << \n";

	for (const auto& pointer : pPointers)
	{
		if (!pointer.second)
			std::cout << "[!] ";
		else
			std::cout << "[+] ";

			std::cout << pointer.first << " = " << pointer.second << '\n';
	}

	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] --> press SUPPR to exit <-- \n";
}

std::string_view ConsoleCheatMenu::GetTargetedPart()
{
	 return CheatHKeys::bHeadPos ? "HEAD" : "PELVIS";
}

void ConsoleCheatMenu::PrintCheatOptions()
{
	system("cls");
	PrintMyName();
	std::cout << "[+] TARGETED PART  : F1 --> [[" << GetTargetedPart() << "]] \n";
	std::cout << "[+] AIMBOT         : RCLICK " << std::boolalpha << "-->[[" << CheatHKeys::bAimbot << "]] \n";
	std::cout << "[+] ESP            : F8 " << std::boolalpha << "-->[[" << CheatHKeys::bESP << "]] \n";
	std::cout << "[+] TEAMCHECK      : F9 " << std::boolalpha << "-->[[" << CheatHKeys::bTeamCheck << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] SMOOTH VALUE   : F3 (-) or F4 (+) " << "-->[[" << CheatHKeys::smoothValue << "]] \n";
	std::cout << "[+] FOV            : F5 (-) or F6 (+) " << "-->[[" << CheatHKeys::fovValue << "]] \n";
	std::cout << "[+] TARGET LOCKING : F2 " << std::boolalpha << "              -->[[" << CheatHKeys::bTargetLock << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] --> [[ press SUPPR to exit] ] <-- \n";
}