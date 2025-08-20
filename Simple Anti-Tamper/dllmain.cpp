// dllmain.cpp : Defines the entry point for the DLL application.
#include "mem/mem.h"



unsigned long __stdcall processLoop(void*)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
a:
    if (ScanProcess())
        TerminateProcess(GetCurrentProcess(), 2);
    Sleep(100);
    goto a;
}

unsigned long __stdcall threadLoop(void*)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
a:
    if (LoopThreads())
        TerminateProcess(GetCurrentProcess(), 3);
    Sleep(250);
    goto a;
}

unsigned long __stdcall debugCheck(void*)
{
a:
    if (IsDebuggerPresent())
        TerminateProcess(GetCurrentProcess(), 4);
    Sleep(250);
    goto a;
}

unsigned long __stdcall thread(void*)
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)processLoop, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)threadLoop, 0, 0, 0);
    CreateThread(0, 0, (LPTHREAD_START_ROUTINE)debugCheck, 0, 0, 0);

a:
    Sleep(1000);
    goto a;
}

int __stdcall DllMain(HMODULE hModule,DWORD fwreason,LPVOID)
{
    if (fwreason == 1)
    {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)thread, 0, 0, 0);
        printf("[+] Anti-Tamper Initialized [+]\n");
    }
    return TRUE;
}

