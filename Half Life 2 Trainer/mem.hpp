#include"includes.hpp"

namespace mem
{
	void Patchbin(BYTE* dst, BYTE* inst, int size);
	DWORD Patternscan(const char* BasseAddrs, const char* Patternsequence, const char* mask);
}

//
//change this - ugly and unreliable - 
namespace hack_hl2
{
	//super ugly, refacto needed
	DWORD hacklife(int val);
	DWORD hackammo();
	DWORD superpistol();

	//restorebytes section
	DWORD restoreammo();

	//test function call
	void noclipON();
	void noclipOFF();
}