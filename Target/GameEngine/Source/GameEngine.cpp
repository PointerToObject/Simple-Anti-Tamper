#include "../GameEngine.h"

unsigned __int64 entityList[10] = {};

void inject()
{
	const char* path = "C:\\Users\\aiden\\source\\repos\\Target\\x64\\Debug\\Simple Anti-Tamper.dll";
	LoadLibraryA(path);
}

void Init()
{
	srand(time(NULL));

	int entity_count = (rand() % 10) + 1;

	for (int i = 0; i < entity_count; i++)
	{
		Entity* alloc = (Entity*)malloc(sizeof(Entity) * entity_count);

		if (alloc->health)
		{
			alloc->health = (rand() % 100) + 1;
			alloc->name = "Player on the heap!";
		}

		entityList[i] = (unsigned __int64)alloc;
	}

	inject();
}