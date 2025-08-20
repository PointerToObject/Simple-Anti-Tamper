#include "GameEngine/GameEngine.h"

void testThread()
{
	for (;;)
	{
		Sleep(1);
	}
}


int main()
{
	Init();

	printf("list -> %llX\n", &entityList);
	printf("Try to hack me!\n");

	uint8_t code = 0;

a:
	if (!code)
	{
		if (GetAsyncKeyState(VK_F3) & 1)
		{
			void* hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)testThread, 0, 0, 0);
			printf("Thread Created -> %llX\n", hThread);
			code = 1;
		}
	}


	goto a;
}