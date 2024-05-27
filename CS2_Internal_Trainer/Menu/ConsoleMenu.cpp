#include "ConsoleMenu.h"

void ConsoleMenu::InitConsole()
{
	AllocConsole();
	freopen_s(&file, "CONOUT$", "w", stdout);

	ConsoleMenu::PrintCheatOptions();
}

void ConsoleMenu::DestroyConsole()
{
	if (file)
		fclose(file);

	FreeConsole();
}

bool ConsoleMenu::IsOptionChanged()
{
	if (GetAsyncKeyState(VK_F1) & 1)
	{
		ConsoleMenu::bHeadPos = !ConsoleMenu::bHeadPos;
		return true;
	}
	else if (GetAsyncKeyState(VK_F2) & 1)
	{
		ConsoleMenu::bTargetLock = !ConsoleMenu::bTargetLock;
		return true;
	}
	else if (GetAsyncKeyState(VK_F3) & 1 &&
		ConsoleMenu::smoothValue > 0)
	{
		--ConsoleMenu::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F4))
	{
		++ConsoleMenu::smoothValue;
		return true;
	}
	else if (GetAsyncKeyState(VK_F5) & 1 &&
		ConsoleMenu::fovValue > 10)
	{
		ConsoleMenu::fovValue -= 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F6) & 1 &&
		ConsoleMenu::fovValue < 100)
	{
		ConsoleMenu::fovValue += 10;
		return true;
	}
	else if (GetAsyncKeyState(VK_F8) & 1)
	{
		ConsoleMenu::bESP = !ConsoleMenu::bESP;
		return true;
	}
	else if (GetAsyncKeyState(VK_F9) & 1)
	{
		ConsoleMenu::bAimbot = !(ConsoleMenu::bAimbot);
		return true;
	}
	else if (GetAsyncKeyState(VK_F10) & 1)
	{
		ConsoleMenu::bTeamCheck = !ConsoleMenu::bTeamCheck;
		return true;
	}

	return false;
}

bool ConsoleMenu::SetDefaultTeamCheck(bool pIsDeathMatch)
{
	if (pIsDeathMatch)
	{
		ConsoleMenu::bTeamCheck = false;
		bInitTeamCheck = true;
		return true;
	}

	return false;
}


void ConsoleMenu::PrintMyName()
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

void ConsoleMenu::PrintErrorPtrInit(const std::map <std::string_view, intptr_t >& pPointers)
{
	if (!file)
		InitConsole();

	system("cls");
	PrintMyName();

	std::cout << "[+] >> PatternScan initialization error << \n";

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

std::string_view ConsoleMenu::GetTargetedPart()
{
	 return ConsoleMenu::bHeadPos ? "HEAD" : "PELVIS";
}

void ConsoleMenu::PrintCheatOptions()
{
	system("cls");
	PrintMyName();
	std::cout << "[+] TARGETED PART  : [F1] --> [[" << GetTargetedPart() << "]] \n";
	std::cout << "[+] AIMBOT         : [F9] " << std::boolalpha << "-->[[" << ConsoleMenu::bAimbot << "]] \n";
	std::cout << "[+] ESP            : [F8] " << std::boolalpha << "-->[[" << ConsoleMenu::bESP << "]] \n";
	std::cout << "[+] TEAMCHECK      : [F10] " << std::boolalpha << "-->[[" << ConsoleMenu::bTeamCheck << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+] SMOOTH VALUE   : [F3] (-) or F4 (+) " << "-->[[" << ConsoleMenu::smoothValue << "]] \n";
	std::cout << "[+] FOV            : [F5] (-) or F6 (+) " << "-->[[" << ConsoleMenu::fovValue << "]] \n";
	std::cout << "[+] TARGET LOCKING : [F2] " << std::boolalpha << "              -->[[" << ConsoleMenu::bTargetLock << "]] \n";
	std::cout << "-------------------------------------------------------------- \n";
	std::cout << "[+]        --> [[ press SUPPR to exit] ] <-- \n";
}