#pragma once

#include <Windows.h>
#include <iostream>

// Game Structures
struct Entity
{
	int health;
	const char* name;
};

extern unsigned __int64 entityList[10];
// Game Structures


// Game Engine Functions 
void Init();

void inject();