#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <exception>
#include <thread>

#include "Helpers/Helpers.hpp"
#include "ImpureDX/ImpureDX.hpp"

#pragma comment(lib, "Helpers.lib")
#pragma comment(lib, "ImpureDX.lib")

static FILE* console_thread = nullptr;

void Init() {
    static bool IsHooked = false;
    if (IsHooked) return;

    IsHooked = true;
    TCHAR szFileName[MAX_PATH];
    GetModuleFileName(NULL, szFileName, MAX_PATH);
    uintptr_t ModuleHandle = (uintptr_t)GetModuleHandle(szFileName);

    std::cout << "ModuleHandle: " << std::hex << ModuleHandle << std::endl;

    mLogger.SetPath("./app/azop.log");

    try {
        ImpureDX::Get().Hook();
    }
    catch (std::exception err) {
        MessageBoxA(NULL, err.what(), "Error", MB_ICONERROR);

        exit(-1);
    }
}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
#ifdef _DEBUG
        AllocConsole();
        freopen_s(&console_thread, "CONOUT$", "w", stdout);

        std::cout << "Debug Console Attached" << std::endl;

        while (!IsDebuggerPresent())
            Sleep(100);
#endif
        std::thread(Init).detach();

        break;
    }

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        if (lpReserved != nullptr) {
            break;
        }
        ImpureDX::Get().Unhook();
#ifdef _DEBUG
        fclose(console_thread);
        FreeConsole();
#endif

        break;
    }
    return TRUE;
}


