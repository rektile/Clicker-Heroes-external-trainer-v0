#pragma once
#include <vector>
#include <Windows.h>

using namespace std;

DWORD GetProcessId(const wchar_t* processName);
uintptr_t GetModuleBaseAddress(const DWORD procId, const wchar_t* moduleName);
uintptr_t GetAddressWithOffset(HANDLE hProc, uintptr_t base, vector<unsigned int> offsets);