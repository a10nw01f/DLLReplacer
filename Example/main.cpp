#include <iostream>
#include <Windows.h>

#pragma comment(lib, "ExampleDll.lib")

__declspec(dllimport) void ExampleFunc();

int main()
{
	while (true)
	{
		ExampleFunc();
		char c;
		std::cin >> c;
	}
	return 0;
}