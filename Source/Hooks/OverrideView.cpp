#include "OverrideView.h"


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
#include "../SDK/CBaseAnimState.h"
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

typedef void(__thiscall* OverrideViewFn)(void* ecx, SDK::CViewSetup* pSetup);

OverrideViewFn original_override_view;

void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (GetAsyncKeyState(VK_XBUTTON2) & 1)
		in_tp = !in_tp;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		auto GetCorrectDistance = [&local_player](float ideal_distance) -> float
		{
			Vector inverse_angles;
			Interfaces::Engine->GetViewAngles(inverse_angles);

			inverse_angles.x *= -1.f, inverse_angles.y += 180.f;

			Vector direction;
			MATH::AngleVectors(inverse_angles, &direction);

			SDK::CTraceWorldOnly filter;
			SDK::trace_t trace;
			SDK::Ray_t ray;

			ray.Init(local_player->GetVecOrigin() + local_player->GetViewOffset(), (local_player->GetVecOrigin() + local_player->GetViewOffset()) + (direction * (ideal_distance + 5.f)));
			Interfaces::Trace->TraceRay(ray, MASK_ALL, &filter, &trace);

			return ideal_distance * trace.flFraction;
		};

		if (SETTINGS::settings.tp_bool && in_tp)
		{
			if (local_player->GetHealth() <= 0)
				local_player->SetObserverMode(5);

			if (!Interfaces::Input->m_fCameraInThirdPerson)
			{
				Interfaces::Input->m_fCameraInThirdPerson = true;
				Interfaces::Input->m_vecCameraOffset = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, GetCorrectDistance(100));
				Vector camForward;

				MATH::AngleVectors(Vector(Interfaces::Input->m_vecCameraOffset.x, Interfaces::Input->m_vecCameraOffset.y, 0), &camForward);
			}
		}
		else
		{
			Interfaces::Input->m_fCameraInThirdPerson = false;
			Interfaces::Input->m_vecCameraOffset = Vector(GLOBAL::real_angles.x, GLOBAL::real_angles.y, 0);
		}

		if (!local_player->GetIsScoped())
		{
			pSetup->fov = SETTINGS::settings.fov;
		}
	}
	original_override_view(ecx, pSetup);
}