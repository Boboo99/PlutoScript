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

void OnConnect(Entity* e)
{
	FunctionManager::WriteToServerConsole(std::string(e->Shared.Client->Session.Clientstate.Name) + " connected! \n");
}

void OnDisconnect(Entity* e)
{
	FunctionManager::WriteToServerConsole(std::string(e->Shared.Client->Session.Clientstate.Name) + " disconnected! \n");
}

void OnPlayerKilled(Entity* playerWhoDied, Entity* inflictor, Entity* playerWhoKilled, int* damage, int* mod, int* weaponIndex, bool* alternateWeapon, Vector3D direction, int* hitLocation)
{
	FunctionManager::WriteToServerConsole(std::string(playerWhoDied->Shared.Client->Session.Clientstate.Name) + " was killed by " + std::string(playerWhoKilled->Shared.Client->Session.Clientstate.Name) + "\n");
}

void PlutoScriptTester::Entry()
{
	HookManager::InstallOnSay(OnSay);
	HookManager::InstallOnConnect(OnConnect);
	HookManager::InstallOnDisconnect(OnDisconnect);
	HookManager::InstallOnPlayerKilled(OnPlayerKilled);
}
