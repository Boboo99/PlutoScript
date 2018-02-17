// PlutoScriptTester.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "PlutoScriptTester.h"
#include "HookManager.h"
#include "FunctionManager.h"

void OnSay(Entity* x,int y,std::string z)
{	
	FunctionManager::WriteToServerConsole(z + "\n");
}

void PlutoScriptTester::Entry()
{
	//FunctionManager::WriteToServerConsole("Loaded!\n");
	HookManager::InstallOnSay(OnSay);
}
