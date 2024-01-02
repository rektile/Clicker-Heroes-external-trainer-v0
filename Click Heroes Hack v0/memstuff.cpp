#include "memstuff.h"
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <TlHelp32.h>

using namespace std;

DWORD GetProcessId(const wchar_t* processName)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(entry);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	DWORD procId = 0;

	while (Process32Next(snapshot, &entry))
	{
		if (!_wcsicmp(entry.szExeFile, processName))
		{

			procId = entry.th32ProcessID;
			break;
		}
	}

	CloseHandle(snapshot);
	return procId;
}

uintptr_t GetModuleBaseAddress(const DWORD procId, const wchar_t* moduleName)
{
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(entry);

	uintptr_t baseAddress = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (snapshot != INVALID_HANDLE_VALUE) {

		while (Module32Next(snapshot, &entry))
		{
			if (!_wcsicmp(entry.szModule, moduleName))
			{
				baseAddress = (uintptr_t)entry.modBaseAddr;
				break;
			}
		}

	}
	CloseHandle(snapshot);
	return baseAddress;
}

uintptr_t GetAddressWithOffset(HANDLE hProc, uintptr_t base, vector<unsigned int> offsets)
{
	uintptr_t addr = base;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;
}