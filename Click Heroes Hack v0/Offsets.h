#pragma once
#include <vector>
#include <Windows.h>

using namespace std;

int coinsBaseOffset = 0x009F0378;
vector<unsigned int> coinsOffsets = { 0x150, 0x0, 0x28, 0x4E0 };

int clickDamageBaseOffset = 0x009F0378;
vector<unsigned int> clickDamageOffsets = { 0x150, 0x8, 0x28 };