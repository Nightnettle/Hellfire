#include "FrameStageNotify.h"

#include "../Utils/Interfaces.h"
#include "../Utils/offsets.h"

#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../Utils/render.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../Utils/NetvarHookManager.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"

#include "../FEATURES/Movement.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/Chams.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/FakeWalk.h"
#include "../FEATURES/FakeLag.h"


typedef void(__thiscall* FrameStageNotifyFn)(void*, int);

FrameStageNotifyFn original_frame_stage_notify;



void __fastcall HookedFrameStageNotify(void* ecx, void* edx, int stage)
{
	Vector aim_punch_old;
	Vector view_punch_old;

	Vector* aim_punch = nullptr;
	Vector* view_punch = nullptr;

	switch (stage)
	{
	case FRAME_NET_UPDATE_POSTDATAUPDATE_START:

		//--- Angle Resolving and Such ---//
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		{
			for (int i = 1; i < 65; i++)
			{
				auto entity = Interfaces::ClientEntityList->GetClientEntity(i);
				auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

				if (!entity)
					continue;

				if (!local_player)
					continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

				if (is_local_player)
					continue;

				if (is_teammate)
					continue;

				if (entity->GetHealth() <= 0)
					continue;


				if (SETTINGS::settings.aim_type == 0)
					resolver->resolve(entity);
			}
		}
		break;

	case FRAME_NET_UPDATE_POSTDATAUPDATE_END:

		break;

	case FRAME_RENDER_START:

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		{
			auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

			if (!local_player)
				return;	

			if (in_tp && !SETTINGS::settings.tp_bool)
				in_tp = false;

			if (in_tp && local_player->GetHealth() <= 0)
				in_tp = false;

			if (in_tp)
				*(Vector*)((DWORD)local_player + 0x31C8) = GLOBAL::real_angles;
		}
		break;
	case FRAME_NET_UPDATE_START:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		{
			if (SETTINGS::settings.beam_bool)
				visuals->DrawBulletBeams();
		}
		break;
	case FRAME_NET_UPDATE_END:

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		{
			for (int i = 1; i < 65; i++)
			{
				auto entity = Interfaces::ClientEntityList->GetClientEntity(i);
				auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

				if (!entity)
					continue;

				if (!local_player)
					continue;

				bool is_local_player = entity == local_player;
				bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

				if (is_local_player)
					continue;

				if (is_teammate)
					continue;

				if (entity->GetHealth() <= 0)
					continue;

				if (SETTINGS::settings.aim_type == 0)
					backtracking->DisableInterpolation(entity);

				if (SETTINGS::settings.novisualrecoil_bool)
				{
					SDK::CBaseEntity* pLocalEntity = NULL;
					float* m_LocalViewAngles = NULL;

					__asm
					{
						MOV pLocalEntity, ESI
						MOV m_LocalViewAngles, EBX
					}

					Vector viewPunch = local_player->GetPunchView(); 
					Vector aimPunch = local_player->GetPunchAngles();

					m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
					m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
					m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
				}
			}
		}

		break;
	}

	original_frame_stage_notify(ecx, stage);
}