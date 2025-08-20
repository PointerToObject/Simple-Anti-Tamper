#include "../mem.h"

bool bit = false;

typedef NTSTATUS(NTAPI* pNtQueryInformationThread)(
    HANDLE, LONG, PVOID, ULONG, PULONG);

//devenv.exe (visual studio)

const wchar_t* banned[10] = {L"ProcessHacker.exe",L"devenv.exe",L"notepad.exe", L"cheatengine-x86_64-SSE4-AVX2.exe",
L"aesopengine-x86_64.exe", L"cheatengine-x86_64.exe", L"cheatengine-i386.exe", L"ReClass.NET.exe", L"x64dbg.exe", L"ida64.exe"};

bool ScanProcess()
{

    bool found = false;

    void* hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 proc;
        proc.dwSize = sizeof(proc);

        if (Process32First(hSnap, &proc))
        {
            do {

                for (int i = 0; i < _countof(banned); i++)
                {
                    if (!wcscmp(proc.szExeFile, banned[i]))
                    {
                        found = true;
                        break;
                    }
                }

            } while (Process32Next(hSnap, &proc));
        }
    }
    else
    {
        printf("[+] Failed to capture the kernel [+]\n"); // my brain typed this for some reason, stared at it and laughed and remembered this message from windbg, so now it stays..  
    }

    CloseHandle(hSnap);
    return found;
}


std::vector<unsigned long> GetThreads()
{
    std::vector<unsigned long> threads;
    unsigned long pid = GetCurrentProcessId();

    // grab ntdll range
    MODULEINFO mi{};
    GetModuleInformation(GetCurrentProcess(),
        GetModuleHandleW(L"ntdll.dll"),
        &mi, sizeof(mi));
    char* ntdllBase = (char*)mi.lpBaseOfDll;
    char* ntdllEnd = ntdllBase + mi.SizeOfImage;

    // resolve NtQueryInformationThread
    auto NtQIT = (pNtQueryInformationThread)GetProcAddress(
        GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationThread");

    void* snap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (snap == INVALID_HANDLE_VALUE) return threads;

    THREADENTRY32 te;
    te.dwSize = sizeof(te);
    if (Thread32First(snap, &te))
    {
        do {
            if (te.th32OwnerProcessID == pid)
            {
                void* hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, te.th32ThreadID);
                if (hThread)
                {

                    CONTEXT ctx{};
                    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

                    if (GetThreadContext(hThread, &ctx))
                    {
                        if (ctx.Dr0 || ctx.Dr1 || ctx.Dr2 || ctx.Dr3)
                        {
                            TerminateProcess(GetCurrentProcess(), ctx.Dr0);
                        }
                    }

                    void* start{};
                    if (NtQIT && NtQIT(hThread, 9, &start, sizeof(start), nullptr) == 0)
                    {
                        char* sa = (char*)start;
                        if (sa < ntdllBase || sa >= ntdllEnd)
                            threads.push_back(te.th32ThreadID);
                    }
                    CloseHandle(hThread);
                }
            }
        } while (Thread32Next(snap, &te));
    }
    CloseHandle(snap);
    return threads;
}

bool LoopThreads()
{
    static int s_size;
    bool found = false;
    std::vector<unsigned long> threads = GetThreads();

    if (!bit)
    {
        s_size = threads.size();
        bit = true;
    }

    int size = threads.size();

    if (s_size != size)
        found = true;

    return found;
}
