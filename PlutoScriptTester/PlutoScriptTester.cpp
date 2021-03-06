// PlutoScriptTester.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "PlutoScriptTester.h"
#include <Datatypes.h>
#include <string>
#include <FunctionManager.h>
#include <HookManager.h>


using namespace PlutoScript;

void OnSay(Entity* x,int y,std::string z)
{	
	WriteToServerConsole(z + "\n");
}

void OnConnect(Entity* e)
{
	WriteToServerConsole(std::string(e->Shared.Client->Session.Clientstate.Name) + " connected! \n");
}

void OnDisconnect(Entity* e)
{
	WriteToServerConsole(std::string(e->Shared.Client->Session.Clientstate.Name) + " disconnected! \n");
}

void OnPlayerKilled(Entity* playerWhoDied, Entity* inflictor, Entity* playerWhoKilled, int* damage, int* mod, int* weaponIndex, bool* alternateWeapon, Vector3D direction, int* hitLocation)
{
	WriteToServerConsole(std::string(playerWhoDied->Shared.Client->Session.Clientstate.Name) + " was killed by " + std::string(playerWhoKilled->Shared.Client->Session.Clientstate.Name) + "\n");
}

void OnPlayerDamaged(Entity* playerWhoWasDamaged, Entity* inflictor, Entity* playerWhoDamaged, int* damage, int* mod, int* weaponIndex, bool* alternateWeapon, Vector3D director, int* hitlocation)
{
	WriteToServerConsole(std::string(playerWhoWasDamaged->Shared.Client->Session.Clientstate.Name) + " was damaged by " + std::string(playerWhoDamaged->Shared.Client->Session.Clientstate.Name) + "\n");
}

void PlutoScriptTester::Entry()
{
	InstallOnSay(OnSay);
	InstallOnConnect(OnConnect);
	InstallOnDisconnect(OnDisconnect);
	InstallOnPlayerKilled(OnPlayerKilled);
	InstallOnPlayerDamaged(OnPlayerDamaged);
}
