#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetPID(const char* ProcessName) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);


	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	Process32First(processesSnapshot, &processInfo);
	if (!strcmp(processInfo.szExeFile, ProcessName))
	{
		CloseHandle(processesSnapshot);
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!strcmp(processInfo.szExeFile, ProcessName))
		{
			CloseHandle(processesSnapshot);
		}
	}
	CloseHandle(processesSnapshot);
	return processInfo.th32ProcessID;
}

MODULEENTRY32 GetModule(const char* moduleName, unsigned long ProcessID)
{
	MODULEENTRY32 modEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, ProcessID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!strcmp(curr.szModule, moduleName))
				{
					modEntry = curr;
					break;
				}
			} while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return modEntry;
}

int main()
{
	std::cout << "JSR HD Character Selector\n";
	std::cout << "-------------------------\n";
	unsigned long long pid = GetPID("jetsetradio.exe");
	MODULEENTRY32 module = GetModule("jetsetradio.exe", pid);
	HANDLE phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	int cubeID = 6;
	int characterID;

	std::cout << "Character IDs:\n";
	std::cout << "Beat = 0\n";
	std::cout << "Gum = 1\n";
	std::cout << "Tab = 2\n";
	std::cout << "Combo = 3\n";
	std::cout << "Slate = 4\n";
	std::cout << "Piranha = 5\n";
	std::cout << "Cube = 6\n";
	std::cout << "Mew = 7\n";
	std::cout << "Yo-Yo = 8\n";
	std::cout << "Garam = 9\n";
	std::cout << "Poison Jam = 10\n";
	std::cout << "Love Shockers = 11\n";
	std::cout << "Noise Tank = 12\n";
	std::cout << "Goji = 13\n";
	std::cout << "Pots = 14\n";
	std::cout << "Unmasked Poison Jam = 15\n";
	std::cout << "If it's a bonus character do not use their ID on the main menu since it will crash the game.\n";
	std::cout << "Do not change characters during the New Game tutorial as it'll crash the game.\n";
	std::cout << "\n";

	std::cout << "Enter the ID for the character you want." << std::endl;
	std::cin >> characterID;
	std::cout << "Changing character ID to " << characterID << "...\n";

	unsigned long long result;
	ReadProcessMemory(phandle, (void*)((unsigned long long)module.modBaseAddr + 0x009FD3B8), &result, sizeof(result), 0);
	
	while (true)
	{
		WriteProcessMemory(phandle, (void*)((unsigned long long)result + 0x78), &characterID, sizeof(characterID), 0);
	}

	system("pause");
}