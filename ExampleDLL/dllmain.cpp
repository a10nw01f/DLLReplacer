#include <Windows.h>
#include <iostream>

__declspec(dllexport) void ExampleFunc()
{
	std::cout << "Exmaple Func\n";
	//std::cout << "Exmaple Func Modified\n";
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    return TRUE;
}

