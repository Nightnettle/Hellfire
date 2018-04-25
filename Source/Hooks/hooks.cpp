#include "..\includes.h"

#include "hooks.h"
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

#include "../MENU/menu_framework.h"

#include <intrin.h>

//Hooks
#include "Createmove.h"
#include "PaintTraverse.h"
#include "FrameStageNotify.h"
#include "DrawModelExecute.h"
#include "SceneEnd.h"
#include "OverrideView.h"
#include "SendDataGram.h"

static bool tick = false;

static auto CAM_THINK = Utils::FindSignature("client.dll", "85 C0 75 30 38 86");

namespace HOOKS
{
	TraceRayFn original_trace_ray;
	SvCheatsGetBoolFn original_get_bool;

	VMT::VMTHookManager iclient_hook_manager;
	VMT::VMTHookManager panel_hook_manager;
	VMT::VMTHookManager model_render_hook_manager;
	VMT::VMTHookManager render_view_hook_manager;
	VMT::VMTHookManager trace_hook_manager;
	VMT::VMTHookManager net_channel_hook_manager;
	VMT::VMTHookManager override_view_hook_manager;
	VMT::VMTHookManager input_table_manager;
	VMT::VMTHookManager get_bool_manager;

	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace)
	{
		original_trace_ray(thisptr, ray, fMask, pTraceFilter, pTrace);
		pTrace->surface.flags |= SURF_SKY;
	}

	bool __fastcall HookedGetBool(void* pConVar, void* edx)
	{
		if ((uintptr_t)_ReturnAddress() == CAM_THINK)
			return true;

		return original_get_bool(pConVar);
	}

	
	void InitHooks()
	{
		

		Interfaces::Engine->ClientCmd_Unrestricted("clear");
		Sleep(200);
		
		Interfaces::Engine->ClientCmd_Unrestricted("toggleconsole");
		Interfaces::cvar->ConsoleColorPrintf(Color(0, 0, 0, 255), ("["));
		Interfaces::cvar->ConsoleColorPrintf(Color(255, 0, 255, 255), ("hellfire"));
		Interfaces::cvar->ConsoleColorPrintf(Color(0, 0, 0, 255), ("] "));
		Interfaces::cvar->ConsoleColorPrintf(Color(255, 0, 255, 255), ("successfully injected.\n\n"));

		iclient_hook_manager.Init(Interfaces::Client);
		original_frame_stage_notify = reinterpret_cast<FrameStageNotifyFn>(
		iclient_hook_manager.HookFunction<FrameStageNotifyFn>(36, HookedFrameStageNotify));

		panel_hook_manager.Init(Interfaces::Panel);
		original_paint_traverse = reinterpret_cast<PaintTraverseFn>(
		panel_hook_manager.HookFunction<PaintTraverseFn>(41, HookedPaintTraverse));

		model_render_hook_manager.Init(Interfaces::ModelRender);
		original_draw_model_execute = reinterpret_cast<DrawModelExecuteFn>(model_render_hook_manager.HookFunction<DrawModelExecuteFn>(21, HookedDrawModelExecute));

		render_view_hook_manager.Init(Interfaces::RenderView);
		original_scene_end = reinterpret_cast<SceneEndFn>(render_view_hook_manager.HookFunction<SceneEndFn>(9, HookedSceneEnd));

		trace_hook_manager.Init(Interfaces::Trace);
		original_trace_ray = reinterpret_cast<TraceRayFn>(trace_hook_manager.HookFunction<TraceRayFn>(5, HookedTraceRay));

		override_view_hook_manager.Init(Interfaces::ClientMode);
		original_override_view = reinterpret_cast<OverrideViewFn>(override_view_hook_manager.HookFunction<OverrideViewFn>(18, HookedOverrideView));
		original_create_move = reinterpret_cast<CreateMoveFn>(override_view_hook_manager.HookFunction<CreateMoveFn>(24, HookedCreateMove));

		auto sv_cheats = Interfaces::cvar->FindVar("sv_cheats");
		get_bool_manager = VMT::VMTHookManager(reinterpret_cast<DWORD**>(sv_cheats));
		original_get_bool = reinterpret_cast<SvCheatsGetBoolFn>(get_bool_manager.HookFunction<SvCheatsGetBoolFn>(13, HookedGetBool));
	}

	//IDk where to put this so it's here :joy:
	void EyeAnglesPitchHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

	}
	void EyeAnglesYawHook(const SDK::CRecvProxyData *pData, void *pStruct, void *pOut)
	{
		*reinterpret_cast<float*>(pOut) = pData->m_Value.m_Float;

		auto entity = reinterpret_cast<SDK::CBaseEntity*>(pStruct);
		if (!entity)
			return;

	}

	void InitNetvarHooks()
	{
		Utils::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[0]", EyeAnglesPitchHook);
		Utils::netvar_hook_manager.Hook("DT_CSPlayer", "m_angEyeAngles[1]", EyeAnglesYawHook);
	}
}