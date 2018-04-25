#include "SceneEnd.h"

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

typedef void(__thiscall* SceneEndFn)(void*);

SceneEndFn original_scene_end;

static auto linegoesthrusmoke = Utils::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");

void __fastcall HookedSceneEnd(void* ecx, void* edx)
{
	original_scene_end(ecx);

	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	static SDK::IMaterial* ignorez = chams->CreateMaterial(true, true, false);
	static SDK::IMaterial* notignorez = chams->CreateMaterial(false, true, false);
	

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

			if (SETTINGS::settings.localesp == false)
			{
				if (is_local_player)
					continue;
			}

			if (SETTINGS::settings.teamesp == false)
			{
				if (is_teammate)
					continue;
			}
			if (SETTINGS::settings.drawfake_bool)
			{
				if (local_player)
				{
					if (notignorez)
					{
						Vector OrigAng;
						OrigAng = local_player->GetEyeAngles();
						//---p100 fake angle detector---//
						local_player->SetAngle2(Vector(0, local_player->GetLowerBodyYaw(), 0));
						//---End---//
						//---Incase you dont use the fake_angle_col---//
						bool LbyColor = false;
						float NormalColor[3] = { 1,1,1 };
						float lbyupdatecolor[3] = { 0,1,0 };
						//---End---//
						Interfaces::RenderView->SetColorModulation(SETTINGS::settings.fake_angle_col); //color thingy on the menu//
						Interfaces::ModelRender->ForcedMaterialOverride(notignorez);
						local_player->DrawModel(0x1, 255);
						Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
						local_player->SetAngle2(OrigAng);
					}
				}
			}
			//--- Colored Models ---//
			if (entity && SETTINGS::settings.chams_type == 2)
			{
				if (is_teammate)
				{
					ignorez->ColorModulate(SETTINGS::settings.teamimodel_col); //255, 40, 200
				}
				else if (is_local_player)
				{
					ignorez->ColorModulate(SETTINGS::settings.selfimodel_col);
				}
				else
				{
					ignorez->ColorModulate(SETTINGS::settings.imodel_col);
				} //255, 40, 200
				Interfaces::ModelRender->ForcedMaterialOverride(ignorez);
				entity->DrawModel(0x1, 255);
				if (is_teammate)
				{
					notignorez->ColorModulate(SETTINGS::settings.teamvmodel_col); //255, 40, 200
				}
				else if (is_local_player)
				{
					notignorez->ColorModulate(SETTINGS::settings.selfvmodel_col);
				}
				else
				{
					notignorez->ColorModulate(SETTINGS::settings.vmodel_col);
				} //0, 125, 255
				Interfaces::ModelRender->ForcedMaterialOverride(notignorez);
				entity->DrawModel(0x1, 255);
				Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
			}
			else if (entity && SETTINGS::settings.chams_type == 1)
			{
				if (is_teammate)
				{
					notignorez->ColorModulate(SETTINGS::settings.teamvmodel_col); //255, 40, 200
				}
				else if (is_local_player)
				{
					notignorez->ColorModulate(SETTINGS::settings.selfvmodel_col);
				}
				else
				{
					notignorez->ColorModulate(SETTINGS::settings.vmodel_col);
				}
				Interfaces::ModelRender->ForcedMaterialOverride(notignorez);
				entity->DrawModel(0x1, 255);
				Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
			}
		}

		//--- Wireframe Smoke ---//
		std::vector<const char*> vistasmoke_wireframe =
		{
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
		};

		std::vector<const char*> vistasmoke_nodraw =
		{
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		};

		for (auto mat_s : vistasmoke_wireframe)
		{

			SDK::IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
			if (!SETTINGS::settings.SmokeType)
			{
				mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_WIREFRAME, false); // wireframe
			}
			else if (SETTINGS::settings.SmokeType)
			{
				mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_WIREFRAME, true); //wireframe
			}
		}

		for (auto mat_n : vistasmoke_nodraw)
		{
			SDK::IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(mat_n, TEXTURE_GROUP_OTHER);
			mat->SetMaterialVarFlag(SDK::MATERIAL_VAR_NO_DRAW, true);
		}



		static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);
		*(int*)(smokecout) = 0;

	}
}