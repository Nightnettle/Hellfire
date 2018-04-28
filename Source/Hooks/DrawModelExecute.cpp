#include "DrawModelExecute.h"

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
#include "xstring"
#include <xstring>
#include "../SDK/CGlobalVars.h"
#include "../Utils/variables.h"



typedef void(__thiscall* DrawModelExecuteFn)(void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);

DrawModelExecuteFn original_draw_model_execute;
void ForceMaterial(Colors::Color color, SDK::IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		CColor blend;
		float alpha = (float)color.a() / 255.f;

		Interfaces::RenderView->SetBlend(alpha);
		Interfaces::RenderView->SetColorModulation(blend);
	}

	if (forceMaterial)
		Interfaces::ModelRender->ForcedMaterialOverride(material);
}
void __fastcall HookedDrawModelExecute(void* ecx, void* edx, SDK::IMatRenderContext* context, const SDK::DrawModelState_t& state, const SDK::ModelRenderInfo_t& render_info, matrix3x4_t* matrix)
{
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		for (int i = 1; i < 65; i++)
		{
			auto entity = Interfaces::ClientEntityList->GetClientEntity(i);
			auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

			if (SETTINGS::settings.noflash) {
				if (true)
				{
					*(float*)((DWORD)local_player + 0xA2FA) = 0;
				}
			}

			if (!entity)
				continue;

			if (!local_player)
				continue;

			std::string strModelName = Interfaces::ModelInfo->GetModelName(render_info.pModel);

			


			if (SETTINGS::settings.wirehands)
			{
				if (strModelName.find("arms") != std::string::npos && SETTINGS::settings.wirehands)
				{
					SDK::IMaterial* WireHands = Interfaces::MaterialSystem->FindMaterial(strModelName.c_str(), TEXTURE_GROUP_MODEL);
					WireHands->SetMaterialVarFlag(SDK::MATERIAL_VAR_WIREFRAME, true);
					Interfaces::ModelRender->ForcedMaterialOverride(WireHands);
				}
			}
			

			if (entity && entity->GetIsScoped() && in_tp && entity == local_player)
			{
				Interfaces::RenderView->SetBlend(0.4);
			}
		}
	}

	original_draw_model_execute(ecx, context, state, render_info, matrix);
}
