#include"mem.hpp"

//global section

DWORD ServerModule = (DWORD)GetModuleHandle("server.dll");


//
//print on game internal console
typedef void(__cdecl* Fn_noclip_sub_26B030)(DWORD* thisptr, int a2, int message, int a4, int a5, int a6, int a7);
Fn_noclip_sub_26B030 PrintGameConsole = (Fn_noclip_sub_26B030)(ServerModule + 0x26B030);
//PrintGameConsole(playerBase, 2, (int)"test NOCLIP-OFF", 0, 0, 0, 0);

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
//	NOCLIP section
//	IDA refference
// [server.dll]_sub_134AE0 - related to noclip switch
// 0x20000 = off / 0x80000 = on
void hack_hl2::noclipON()
{

	DWORD* playerBase = *(DWORD**)(ServerModule + 0x68B8DC);

	playerBase[78] = 0x80000;
}

void hack_hl2::noclipOFF()
{	
	DWORD* playerBase = *(DWORD**)(ServerModule + 0x68B8DC);


	playerBase[78] = 0x20000; 

}

//
//	GODMODE SECTION
//	IDA refference
//[server.dll] sub_134810 - godmode switch
void hack_hl2::godmodeON()
{
	DWORD* playerBase = *(DWORD**)(ServerModule + 0x68B8DC);


	playerBase[65] = 0x14081;
}

void hack_hl2::godmodeOFF()
{
	DWORD* playerBase = *(DWORD**)(ServerModule + 0x68B8DC);


	playerBase[65] = 0x10081;
}
