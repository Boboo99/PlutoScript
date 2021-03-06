// PlutoScript.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
namespace PlutoScript
{
	namespace Internal
	{
		using SpawnServerPrototype = void(__cdecl*)(int a1, int a2, int a3);
		SpawnServerPrototype SpawnServer;

		void HookedSpawnServer(int a1, int a2, int a3)
		{
			if (!FunctionManager::IsInitialized)
				FunctionManager::Initialize();
			if (!HookManager::IsInitialized)
				HookManager::Initialize();
			if (!ScriptManager::IsInitialized)
				ScriptManager::Initialize();

			return SpawnServer(a1, a2, a3);
		}


	}

	void Initialize()
	{
		Internal::SpawnServer = reinterpret_cast<Internal::SpawnServerPrototype>(PlutoScript::HookManager::Internal::DetourFunction(reinterpret_cast<BYTE*>(0x004FB7B0), reinterpret_cast<BYTE*>(&Internal::HookedSpawnServer), 0x5));
	}
}
