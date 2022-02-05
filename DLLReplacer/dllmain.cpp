#include <windows.h>
#include <string>
#include <fstream>
#include <regex>
#include "detour/detours.h"

#define REPLACE_DLL_IMPL
#include "ReplaceDLL.h"

void SetHook()
{
	std::fstream input("dll_replacer_config.txt");

	std::string name;
	std::getline(input, name);
	std::string regex;
	std::getline(input, regex);

	auto original_dll = name + "_old.dll";
	auto new_dll = name + ".dll";

	ReplaceDLL(original_dll.c_str(), new_dll.c_str(), std::regex(regex));
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    HANDLE file;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        SetHook();
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, hModule, 0, NULL);
        break;
    }
    return TRUE;
}