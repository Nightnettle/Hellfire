#include "../includes.h"
#include "../Utils/Interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/Fakewalk.h"
#include "../FEATURES/Aimbot.h"

#include <time.h>
#include <iostream>

void CFakewalk::do_fakewalk(SDK::CUserCmd* cmd)
{
	if (!SETTINGS::settings.fakewalk) return;
	if (GetAsyncKeyState(VK_SHIFT)) //make sure fakelag is set to max when u trigger fakewalk!
	{
		auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		if (!local_player || local_player->GetHealth() <= 0)
			return;

		uintptr_t* FPointer; __asm { MOV FPointer, EBP }
		byte* SendPacket = (byte*)(*FPointer - 0x1C);

		static int choked = 0;
		choked = choked > 7 ? 0 : choked + 1;
		cmd->forwardmove = choked < 2 || choked > 5 ? 0 : cmd->forwardmove;
		cmd->sidemove = choked < 2 || choked > 5 ? 0 : cmd->sidemove;
		*SendPacket = choked < 1;

	}
	
}

CFakewalk* slidebitch = new CFakewalk();