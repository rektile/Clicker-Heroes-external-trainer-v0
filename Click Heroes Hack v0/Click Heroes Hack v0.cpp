#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <vector>
#include "memstuff.h"
#include "Offsets.h"
#include "MemoryStructs.h"

using namespace std;

// from https://stackoverflow.com/questions/34842526/update-console-without-flickering-c
void cls()
{
    // Get the Win32 handle representing standard output.
    // This generally only has to be done once, so we make it static.
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = { 0, 0 };

    // std::cout uses a buffer to batch writes to the underlying console.
    // We need to flush that to the console because we're circumventing
    // std::cout entirely; after we clear the console, we don't want
    // stale buffered text to randomly be written out.
    std::cout.flush();

    // Figure out the current width and height of the console window
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
        // TODO: Handle failure!
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

    DWORD written;

    // Flood-fill the console with spaces to clear it
    FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

    // Reset the attributes of every character to the default.
    // This clears all background colour formatting, if any.
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

    // Move the cursor back to the top left for the next sequence of writes
    SetConsoleCursorPosition(hOut, topLeft);
}

int main()
{
    cout << "Click Heroes Hack" << endl;
    cout << "game version: 1.0e12-4443" << endl;
    
    DWORD procId = GetProcessId(L"Clicker Heroes.exe");

    if (procId == 0)
    {
        cout << "Could not find process 'Clicker Heroes.exe'!" << endl;
        return 0;
    }


    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"mono-2.0-bdwgc.dll");

    if (moduleBase == 0)
    {
        cout << "Unable to get module base!" << endl;
    }

    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    if (hProcess == 0)
    {
        cout << "Could not open handle to process!" << endl;
        return 0;
    }



    uintptr_t dynamicPtrBaseAddr = moduleBase + coinsBaseOffset;

    uintptr_t coinsAddress = GetAddressWithOffset(hProcess, dynamicPtrBaseAddr, coinsOffsets);
    BigNumber coins;
    ReadProcessMemory(hProcess, (BYTE*)coinsAddress, &coins, sizeof(coins), nullptr);

    dynamicPtrBaseAddr = moduleBase + clickDamageBaseOffset;

    uintptr_t clickDamageAddress = GetAddressWithOffset(hProcess, dynamicPtrBaseAddr, clickDamageOffsets);
    BigNumber clickDamage;
    ReadProcessMemory(hProcess, (BYTE*)clickDamageAddress, &clickDamage, sizeof(clickDamage), nullptr);

    bool enInfCoins = false;
    bool enOnePunchDeath = false;

    while (1)
    {   
        cout << "Click Heroes Hack" << endl;
        cout << "Game version: 1.0e12-4443" << endl;

        cout << "Infinite coins: " << enInfCoins << endl;

        cout << "One punch death: " << enOnePunchDeath << endl;

        if (GetAsyncKeyState(VK_NUMPAD7) & 1)
        {
            enInfCoins = !enInfCoins;
        }
        if (GetAsyncKeyState(VK_NUMPAD8) & 1)
        {
            enOnePunchDeath = !enOnePunchDeath;
        }

        double newValue = DBL_MAX * 0.75;

        if (enInfCoins)
        {
            coins.numBase = newValue;
            coins.numPow = newValue;
            WriteProcessMemory(hProcess, (BYTE*)coinsAddress, &coins, sizeof(coins), 0);
        }
        if (enOnePunchDeath)
        {
            clickDamage.numBase = newValue;
            clickDamage.numPow = newValue;
            WriteProcessMemory(hProcess, (BYTE*)clickDamageAddress, &clickDamage, sizeof(clickDamage), 0);

        }

        Sleep(10);
        cls();
    }


    return 0;
}
