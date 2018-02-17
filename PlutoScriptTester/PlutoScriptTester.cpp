// PlutoScriptTester.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "PlutoScriptTester.h"
#include "HookManager.h"

void OnSay(Entity* x,int y,char* z)
{
	MessageBoxA(NULL, "OnSay 2 called", "", 0);
}

void PlutoScriptTester::Entry()
{
	MessageBoxA(NULL, "PlutoScriptTester Loaded!", "", 0);
	HookManager::InstallOnSay(OnSay);
}
