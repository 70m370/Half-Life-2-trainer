#include"mem.hpp"

//global section

DWORD mem_module = (DWORD)GetModuleHandle("server.dll");

DWORD* player_life = (DWORD*)(mem_module + 0x68B8DC);

//guns - ammo
DWORD* player_pistol_and_rifles = (DWORD*)(mem_module + 0xE73FA);//reversed some part of this function - check on ida
DWORD* player_revol = (DWORD*)(mem_module + 0X3C030E);
DWORD* player_shotgun = (DWORD*)(mem_module + 0x3D9D6B);//main shotgun function
DWORD* player_shotgun_second_shot = (DWORD*)(mem_module + 0x3DA331);
DWORD* player_secondary_shot = (DWORD*)(mem_module + 0XF0BC4);// function related to reload

//super pistol
DWORD* pistol_projectile_plus = (DWORD*)(mem_module + 0xF6E3D);


void mem::Patchbin(BYTE* dst, BYTE* inst, int size)
{
    DWORD oldprotec;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotec);
    memcpy(dst, inst, size);
    VirtualProtect(dst, size, oldprotec, &oldprotec);
}

DWORD hack_hl2::hacklife(int val)
{
    int* life = (int*)(*player_life + 0xe0);
    *life = val;
    return 0;
}

DWORD hack_hl2::hackammo()
{
    mem::Patchbin((BYTE*)player_pistol_and_rifles, (BYTE*)"\x90\x90", 2);
    mem::Patchbin((BYTE*)player_revol, (BYTE*)"\x8D\x1F\x90", 3);
    mem::Patchbin((BYTE*)player_shotgun, (BYTE*)"\x90\x90\x90\x90\x90\x90", 6);
    mem::Patchbin((BYTE*)player_shotgun_second_shot, (BYTE*)"\x90\x90\x90\x90\x90\x90", 6);
    mem::Patchbin((BYTE*)player_secondary_shot, (BYTE*)"\x90\x90", 2);


    return 0;
}

DWORD hack_hl2::superpistol()
{
    //based on the things i reversed on IDA
    mem::Patchbin((BYTE*)pistol_projectile_plus, (BYTE*)"\x8B\x45\x00", 3);
   
    return 0;
}

//restoration part

DWORD hack_hl2::restoreammo()
{ 
    mem::Patchbin((BYTE*)player_pistol_and_rifles, (BYTE*)"\x89\x3E", 2);
    mem::Patchbin((BYTE*)player_revol, (BYTE*)"\x8B\x3E", 2);
    mem::Patchbin((BYTE*)player_shotgun, (BYTE*)"\x89\x83\xAC\x04\x00\x00", 6);
    mem::Patchbin((BYTE*)player_shotgun_second_shot, (BYTE*)"\x89\x87\xAC\x04\x00\x00", 6);
    mem::Patchbin((BYTE*)player_secondary_shot, (BYTE*)"\x89\x1E\x5F", 2);

    //superpistol
    mem::Patchbin((BYTE*)pistol_projectile_plus, (BYTE*)"\x8B\x45\x98", 3);

    return 0;
}
