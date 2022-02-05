#pragma once

#include <windows.h>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
#include "detour/detours.h"

void ReplaceDLL(HMODULE old_dll, HMODULE new_dll, const std::regex& regex);
void ReplaceDLL(const char* old_dll_file, const char* new_dll_file, const std::regex& regex);

class DLLReplacer
{
public:
	void ReplaceDLL(const char* old_dll, const char* new_dll, const std::regex& regex);
private:
	std::unordered_map<std::string, std::vector<HMODULE>> m_Modules;
};

#ifdef REPLACE_DLL_IMPL

namespace
{
	template<class F>
	void ForEachExport(HMODULE lib, F&& func)
	{
		DetourEnumerateExports(lib, &func, [](_In_opt_ PVOID pContext,
			_In_ ULONG nOrdinal,
			_In_opt_ LPCSTR pszName,
			_In_opt_ PVOID pCode)
		{
			(*(F*)pContext)(pszName, pCode);
			return BOOL(true);
		});
	}

	bool IsFunction(void* address)
	{
		constexpr uint64_t mask = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
		MEMORY_BASIC_INFORMATION buffer;
		auto size = VirtualQuery(address, &buffer, sizeof(buffer));
		return size && (buffer.Protect & mask);
	}

	std::string GetNewGUID()
	{
		UUID uuid = { 0 };
		std::string guid;

		::UuidCreate(&uuid);

		RPC_CSTR szUuid = NULL;
		if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
		{
			guid = (char*)szUuid;
			::RpcStringFreeA(&szUuid);
		}

		return guid;
	}

	HMODULE LoadLibraryWithRename(const char* name)
	{
		auto tmp_name = GetNewGUID() + "_tmp.dll";
		rename(name, tmp_name.c_str());
		auto library = LoadLibraryA(tmp_name.c_str());
		rename(tmp_name.c_str(), name);

		return library;
	}
}

void ReplaceDLL(HMODULE old_dll, HMODULE new_dll, const std::regex& regex)
{
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	ForEachExport(new_dll, [&](const char* name, void* address)
	{
		if (IsFunction(address) && std::regex_match(name, regex))
		{
			if (auto old_address = GetProcAddress(old_dll, name); old_address)
			{
				DetourAttach(&(PVOID&)old_address, address);
			}
		}
	});

	DetourTransactionCommit();
}

void ReplaceDLL(const char* old_dll_file, const char* new_dll_file, const std::regex& regex)
{
	ReplaceDLL(
		LoadLibraryWithRename(old_dll_file),
		LoadLibraryWithRename(new_dll_file),
		regex);
}

void DLLReplacer::ReplaceDLL(const char* old_dll, const char* new_dll, const std::regex& regex)
{
	auto new_module = LoadLibraryWithRename(old_dll);
	auto old_module = LoadLibraryWithRename(new_dll);

	auto& previous_modules = m_Modules[old_dll];
	if (previous_modules.empty())
	{
		previous_modules.emplace_back(old_module);
	}

	for (auto lib : previous_modules)
	{
		::ReplaceDLL(new_module, lib, regex);
	}

	previous_modules.emplace_back(new_module);
}

#endif
