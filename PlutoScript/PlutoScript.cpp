// PlutoScript.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
namespace PlutoScript
{
	void Initialize()
	{
		ScriptManager::Initialize();
		HookManager::Initialize();
	}
}
