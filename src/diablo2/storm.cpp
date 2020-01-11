#include <diablo2/storm.h>

#include <Windows.h>

char* diablo2::storm::get_base() {
	static auto base = reinterpret_cast<char*>(GetModuleHandle("storm.dll"));
	return base;
}
