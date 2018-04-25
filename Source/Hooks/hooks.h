#pragma once

namespace SDK
{
	class IMatRenderContext;
	class Ray_t;
	class ITraceFilter;

	struct CViewSetup;
	struct DrawModelState_t;
	struct ModelRenderInfo_t;
	struct trace_t;
}

namespace HOOKS
{
	typedef void(__thiscall* TraceRayFn)(void *thisptr, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter* pTraceFilter, SDK::trace_t *pTrace);
	typedef bool(__thiscall* SvCheatsGetBoolFn)(void*);

	extern TraceRayFn original_trace_ray;
	extern SvCheatsGetBoolFn original_get_bool;

	extern VMT::VMTHookManager iclient_hook_manager;
	extern VMT::VMTHookManager panel_hook_manager;
	extern VMT::VMTHookManager model_render_hook_manager;
	extern VMT::VMTHookManager render_view_hook_manager;
	extern VMT::VMTHookManager trace_hook_manager;
	extern VMT::VMTHookManager net_channel_hook_manager;
	extern VMT::VMTHookManager override_view_hook_manager;
	extern VMT::VMTHookManager input_table_manager;
	extern VMT::VMTHookManager get_bool_manager;

	void InitHooks();

	// virtual function hooks
	void __fastcall HookedTraceRay(void *thisptr, void*, const SDK::Ray_t &ray, unsigned int fMask, SDK::ITraceFilter *pTraceFilter, SDK::trace_t *pTrace);
	bool __fastcall HookedGetBool(void* pConVar, void* edx);
	


	// netvar hooks
	void InitNetvarHooks();
}