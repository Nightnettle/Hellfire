#pragma once
#include "..\SDK\CUserCmd.h"
#include "..\Utils\vmatrix.h"

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

typedef void(__thiscall* DrawModelExecuteFn)(void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);

extern DrawModelExecuteFn original_draw_model_execute;

void __fastcall HookedDrawModelExecute(void*, void*, SDK::IMatRenderContext*, const SDK::DrawModelState_t&, const SDK::ModelRenderInfo_t&, matrix3x4_t*);