#include"mem.hpp"

//global section

DWORD ServerModule = (DWORD)GetModuleHandle("server.dll");

DWORD* playerBase = *(DWORD**)(ServerModule + 0x68B8DC);

//
//print on game internal console
typedef void(__cdecl* Fn_noclip_sub_26B030)(DWORD* thisptr, int a2, int message, int a4, int a5, int a6, int a7);
Fn_noclip_sub_26B030 PrintGameConsole = (Fn_noclip_sub_26B030)(ServerModule + 0x26B030);


/*
//guns - ammo
DWORD* player_pistol_and_rifles = (DWORD*)(mem_module + 0xE73FA);//reversed some part of this function - check on ida
DWORD* player_revol = (DWORD*)(mem_module + 0X3C030E);
DWORD* player_shotgun = (DWORD*)(mem_module + 0x3D9D6B);//main shotgun function
DWORD* player_shotgun_second_shot = (DWORD*)(mem_module + 0x3DA331);
DWORD* player_secondary_shot = (DWORD*)(mem_module + 0XF0BC4);// function related to reload

//super pistol
DWORD* pistol_projectile_plus = (DWORD*)(mem_module + 0xF6E3D);
*/


//
// Mem functions

void mem::Patchbin(BYTE* dst, BYTE* inst, int size)
{
    DWORD oldprotec;
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotec);
    memcpy(dst, inst, size);
    VirtualProtect(dst, size, oldprotec, &oldprotec);
}


DWORD mem::Patternscan(const char* BasseAddrs, const char* Patternsequence, const char* mask)
{
	//getting the module handle
	HMODULE CurModuleBaseAddrs = GetModuleHandle(BasseAddrs);

	std::cout << std::hex << CurModuleBaseAddrs << "\n";

	MODULEINFO module_in;

	if (CurModuleBaseAddrs = 0)
	{
		std::cout << "HMODULE NOT VALID" << std::endl;
	}
	else
	{
		GetModuleInformation(GetCurrentProcess(), CurModuleBaseAddrs, &module_in, sizeof(MODULEINFO));
		std::cout << "Whole Module size = " << module_in.SizeOfImage << "\n";
	}

	DWORD base = (DWORD)GetModuleHandle(BasseAddrs);
	DWORD size = (DWORD)module_in.SizeOfImage;

	DWORD patternLenght = (DWORD)strlen(mask);

	DWORD oldprotec;

	VirtualProtect((DWORD*)base, sizeof(DWORD), PAGE_EXECUTE_READ, &oldprotec);

	for (DWORD i = 0; i < size - patternLenght; i++)
	{
		bool found = true;

		for (DWORD j = 0; j < patternLenght; j++)
		{
			found &= mask[j] == '?' || Patternsequence[j] == *(char*)(base + i + j);
		}
		if (found)
		{
			std::cout << " FOUND " << std::hex << base + i << "\n";

			return base + i;
		}

	}
	VirtualProtect((DWORD*)base, sizeof(DWORD), oldprotec, &oldprotec);
	return 0;
}


//
// hack functions - will go out
DWORD hack_hl2::hacklife(int val)
{
	playerBase[56] = val;

    //int* life = (int*)(*playerBase + 0xE0);
    //*life = val;
    return 0;
}

//
// ugly part - remake all this shit - .370 is working. will make everything right
/*
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
*/

//
//	NOCLIP section
//	IDA refference
// [server.dll]_sub_134AE0 - related to noclip switch
void hack_hl2::noclipON()
{
	/*
	typedef void(__cdecl* Fn_noclip_sub_1358A0)(DWORD* thisptr);
	Fn_noclip_sub_1358A0 CallNoclipON = (Fn_noclip_sub_1358A0)(ServerModule + 0x1358A0);
	CallNoclipON(playerBase);*/
	playerBase[78] = 0x80000;
}

void hack_hl2::noclipOFF()
{	
	playerBase[78] = 0x20000; // 0x20000 = off / 0x80000 = on
	//PrintGameConsole(playerBase, 2, (int)"test NOCLIP-OFF", 0, 0, 0, 0);
}

//
//	GODMODE SECTION
//	IDA refference
//[server.dll] sub_134810 - godmode switch
void hack_hl2::godmodeON()
{
	playerBase[65] = 0x14081;
}

void hack_hl2::godmodeOFF()
{
	playerBase[65] = 0x10081;
}
