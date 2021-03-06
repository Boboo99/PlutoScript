#include "stdafx.h"
std::vector<PlutoScript::HookManager::OnSay> PlutoScript::HookManager::Internal::OnSayCallbacks;
PlutoScript::HookManager::Internal::OnSayNative PlutoScript::HookManager::Internal::OnSayReturn;

std::vector<PlutoScript::HookManager::OnConnect> PlutoScript::HookManager::Internal::OnConnectCallbacks;
std::vector<PlutoScript::HookManager::OnDisconnect> PlutoScript::HookManager::Internal::OnDisconnectCallbacks;

std::vector<PlutoScript::HookManager::OnPlayerKilled> PlutoScript::HookManager::Internal::OnPlayerKilledCallbacks;
PlutoScript::HookManager::Internal::OnPlayerKilledNative PlutoScript::HookManager::Internal::OnPlayerKilledReturn;

PlutoScript::HookManager::Internal::OnNotifyNative PlutoScript::HookManager::Internal::OnNotifyReturn;

std::vector<PlutoScript::HookManager::OnPlayerDamaged> PlutoScript::HookManager::Internal::OnPlayerDamagedCallbacks;
PlutoScript::HookManager::Internal::OnPlayerDamagedNative PlutoScript::HookManager::Internal::OnPlayerDamagedReturn;

bool PlutoScript::HookManager::IsInitialized = false;


namespace PlutoScript
{
	namespace HookManager
	{
		namespace Internal
		{
			void* DetourFunction(BYTE* src, BYTE* dst, const int len)
			{
				BYTE* jmp = (BYTE*)malloc(len + 5);
				DWORD dwBack;

				VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, &dwBack);
				memcpy(jmp, src, len);

				jmp += len;
				jmp[0] = 0xE9;

				*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;

				src[0] = 0xE9;
				*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
				for (int i = 5; i < len; i++)
					src[i] = 0x90;
				VirtualProtect(src, len, dwBack, &dwBack);

				return (jmp - len);
			}

			void HookedOnSay(Entity* entity, int team, char* message)
			{
				for (auto &callback : OnSayCallbacks)
					callback(entity, team, message);

				return OnSayReturn(entity, team, message);
			}

			void HookedOnPlayerKilled(Entity* playerWhoDied, Entity* inflictor, Entity* playerWhoKilled, int damage,
				int mod, int weaponIndex, bool alternateWeapon, Vector3D direction, int hitLocation, int a1, int a2)
			{
				for (auto &callback : OnPlayerKilledCallbacks)
					callback(playerWhoDied, inflictor, playerWhoKilled, &damage, &mod, &weaponIndex, &alternateWeapon, direction, &hitLocation);

				return OnPlayerKilledReturn(playerWhoDied, inflictor, playerWhoKilled, damage, mod, weaponIndex, alternateWeapon, direction, hitLocation, a1, a2);
			}

			void HookedOnPlayerDamaged(Entity* playerWhoWasDamaged, Entity* inflictor, Entity* playerWhoDamaged, int damage,
				int a1, int mod, int weaponIndex, bool alternateWeapon, Vector3D direction, int a2, int hitLocation, int a3)
			{
				for (auto &callback : OnPlayerDamagedCallbacks)
					callback(playerWhoWasDamaged, inflictor, playerWhoDamaged, &damage, &mod, &weaponIndex, &alternateWeapon, direction, &hitLocation);

				return OnPlayerDamagedReturn(playerWhoWasDamaged, inflictor, playerWhoDamaged, damage, a1, mod, weaponIndex, alternateWeapon, direction, a2, hitLocation, a3);
			}

			void HookedOnNotify(int object, int notify, int a1)
			{
				if (object)
				{

					if (notify == 8907)
						for (auto &callback : OnConnectCallbacks)
							callback(FunctionManager::Internal::GetEntityFromObjectReference(object));
					if (notify == 3188 && a1 == 2 && reinterpret_cast<ScriptEnviorment*>(0x001F58080)->Top->Value.Integer == 1422)
					{
						for (auto &callback : OnDisconnectCallbacks)
							callback(FunctionManager::Internal::GetEntityFromObjectReference(object));
						//auto scriptEnv = reinterpret_cast<ScriptEnviorment*>(0x001F58080);
						//char buffer[1024];

						//auto x = scriptEnv->Top;
						//auto y = &scriptEnv->Top[-1];
						//auto z = &scriptEnv->Top[-2];
						//sprintf_s(buffer, "Menusresponse: InParams: %i | OutParams: %i \n Top_Type : %i | Top_Value : %i \n -1_Typ:e %i | -1_Value: %i \n -2_Type: %i | -2_Value : %i\n", scriptEnv->InParametersCount,scriptEnv->OutParametersCount,x->Type,x->Value.Integer,y->Type,y->Value.Integer,z->Type,z->Value.Integer);
						//FunctionManager::WriteToServerConsole(buffer);
					}
				}
				return OnNotifyReturn(object, notify, a1);
			}
		}

		void Initialize()
		{
			Internal::OnSayReturn = reinterpret_cast<Internal::OnSayNative>(Internal::DetourFunction(reinterpret_cast<BYTE*>(0x0047E900), reinterpret_cast<BYTE*>(Internal::HookedOnSay), 0x6));

			Internal::OnPlayerKilledReturn = reinterpret_cast<Internal::OnPlayerKilledNative>(Internal::DetourFunction(reinterpret_cast<BYTE*>(0x004AD030), reinterpret_cast<BYTE*>(Internal::HookedOnPlayerKilled), 0x6));
			Internal::OnPlayerDamagedReturn = reinterpret_cast<Internal::OnPlayerDamagedNative>(Internal::DetourFunction(reinterpret_cast<BYTE*>(0x004ACE50), reinterpret_cast<BYTE*>(Internal::HookedOnPlayerDamaged), 0x6));

			Internal::OnNotifyReturn = reinterpret_cast<Internal::OnNotifyNative>(Internal::DetourFunction(reinterpret_cast<BYTE*>(0x004EFBB0), reinterpret_cast<BYTE*>(Internal::HookedOnNotify), 0x7));
			IsInitialized = true;
		}
	}

	__declspec(dllexport) void InstallOnSay(HookManager::OnSay onSay)
	{
		HookManager::Internal::OnSayCallbacks.push_back(onSay);
	}

	__declspec(dllexport) void InstallOnConnect(HookManager::OnConnect onConnect)
	{
		HookManager::Internal::OnConnectCallbacks.push_back(onConnect);
	}

	__declspec(dllexport) void InstallOnDisconnect(HookManager::OnDisconnect onDisconnect)
	{
		HookManager::Internal::OnDisconnectCallbacks.push_back(onDisconnect);
	}

	__declspec(dllexport) void InstallOnPlayerKilled(HookManager::OnPlayerKilled onPlayerKilled)
	{
		HookManager::Internal::OnPlayerKilledCallbacks.push_back(onPlayerKilled);
	}

	__declspec(dllexport) void InstallOnPlayerDamaged(HookManager::OnPlayerDamaged onPlayerDamaged)
	{
		HookManager::Internal::OnPlayerDamagedCallbacks.push_back(onPlayerDamaged);
	}
}