#include"includes.hpp"

namespace mem
{
	void Patchbin(BYTE* dst, BYTE* inst, int size);
}

namespace hack_hl2
{
	DWORD hacklife(int val);
	DWORD hackammo();
	DWORD superpistol();

	//restorebytes section
	DWORD restoreammo();
}