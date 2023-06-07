#include"includes.hpp"
#include"mem.hpp" // my memory library.
#include"hooks.hpp"
#include"gui.hpp"

void Setup(const HMODULE instance)
{
    try
    {
        gui::Setup();
        hooks::Setup();
    
    }
    catch (const std::exception& error)
    {
        MessageBeep(MB_ICONERROR);
        MessageBox(
            0,
            error.what(),
            "error",
            MB_OK | MB_ICONEXCLAMATION
            );

        goto UNLOAD;
    }

    while (!GetAsyncKeyState(VK_END))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

UNLOAD:
    hooks::Destroy();
    gui::Destroy();
    FreeLibraryAndExitThread(instance, 0);
}



BOOL WINAPI DllMain(
    const HMODULE instance,
    const std::uintptr_t reason,
    const void* reserved
)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(instance);

        const auto thread = CreateThread(
            nullptr,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
            instance,
            0,
            nullptr
            );
        if (thread)
            CloseHandle(thread);
        
    }

    return TRUE;
}


//TO-do
//
// - out - doesnt make much sense in hl2.
//multi-client through mutex manipulation - half life case is at launcher.dll 
//
// to-do
//create a way of bypassing the sv_cheats restriction
//
//create a way to spawn specific cheats
//guns, noclip, spawn a boat and jeep, god mode too, will overwride the life hack
