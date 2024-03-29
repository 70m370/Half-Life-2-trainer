#pragma once

#define	WIN32_LEAN_AND_MEAN 

#define DEBUG( text, ... ) printf( text "\n", __VA_ARGS__ )

#include<Windows.h>
#include<TlHelp32.h>
#include<iostream>
#include<thread>
#include<Psapi.h>

#include"mem.hpp"

#include<d3d9.h>

//imgui
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx9.h"

#include<stdexcept> // to use exceptions


#include<intrin.h>

//minhook
#include"minhook/MinHook.h"