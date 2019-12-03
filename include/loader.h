#pragma once

//Since we can't overload C-exported functions I've created this crutch. I'm sorry.

#ifdef PLUGY
#define EP_HEADER __declspec(dllexport) void __stdcall Init() {
#define EP_FOOTER }
#endif

#ifdef D2_MOD
#define EP_HEADER __declspec(dllexport) void* __stdcall Init(const char* a1) {
#define EP_FOOTER return nullptr; }
#endif