# Simple Anti-Tamper DLL + Target Process Demo

This repository contains two solutions for experimenting with anti-tamper techniques on Windows x64:

1. **Simple Anti-Tamper DLL** – A lightweight, multithreaded DLL designed to protect 64-bit Windows processes from common tampering/reversing techniques.
2. **Target Process Demo** – A small mock "game" with a basic entity system used to test the anti-tamper features.

---

## Solution #1 – Simple Anti-Tamper DLL

A multithreaded DLL that detects debugger activity, hardware breakpoints, thread manipulation, and banned processes. The DLL terminates the target process immediately if suspicious activity is detected.

### Features

- **Debugger detection**  
  Uses `IsDebuggerPresent()` to detect active debugging.

- **Hardware breakpoint checks**  
  Reads debug registers (`Dr0–Dr3`) using `GetThreadContext()`. Detection triggers immediate termination.

- **Thread integrity monitoring**  
  - Captures a baseline snapshot of all threads on startup.  
  - Terminates the process if thread count changes unexpectedly.  
  - Verifies each thread's start address; threads starting outside `ntdll.dll` are flagged as suspicious.

- **Banned process detection**  
  Checks running processes against a predefined blacklist (Cheat Engine, x64dbg, IDA, Process Hacker, etc.). Detection triggers termination.

- **Lightweight & multithreaded**  
  All checks run on low-priority background threads to minimize performance impact.

### Exit Codes

| Exit Code | Reason                        |
|-----------|-------------------------------|
| 2         | Banned process detected       |
| 3         | Thread baseline changed       |
| 4         | Debugger detected             |
| Dr0 value | Hardware breakpoint hit       |

### How to Use

Load the DLL into your process:

```cpp
void inject()
{
    const char* path = "C:\\path\\to\\Simple Anti-Tamper.dll";
    LoadLibraryA(path);
}
```

Once loaded, the DLL automatically spins up monitoring threads.

For external injection, you can also use:
```
VirtualAllocEx + WriteProcessMemory + CreateRemoteThread(LoadLibraryA)
```
…but that's optional for this proof-of-concept.

### Building

- **Target:** Windows x64
- **Headers:** Windows.h, TlHelp32.h, psapi.h  
- **Libraries:** Link against Psapi.lib for GetModuleInformation()

---

## Solution #2 – Target Process Demo

A small C++ "game" to test the anti-tamper system.

### Features

- Defines an Entity struct with health and name.
- Creates an entityList with up to 10 heap-allocated entities.
- Prints the base address of the list to make tampering attempts easier.
- Press F3 to spawn a new thread (triggers thread-monitoring logic).

### Interaction with Anti-Tamper DLL

- Calls `inject()` inside `Init()` to load the anti-tamper DLL.
- Monitoring begins immediately.
- Debugger activity, banned process detection, or unexpected thread changes terminate the process.

---



## Project Layout

```
/AntiTamper/
    mem.h
    mem.cpp
    dllmain.cpp

/Target/
    GameEngine.h
    GameEngine.cpp
    Main.cpp
```

---

## Disclaimer

This is a proof-of-concept. It is not production-ready and should not be used in security-critical applications. Its goal is to demonstrate basic anti-tamper techniques and provide a small playground for testing.
