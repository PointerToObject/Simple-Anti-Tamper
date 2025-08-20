#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <vector>


bool ScanProcess();

std::vector<unsigned long> GetThreads();

bool LoopThreads();

