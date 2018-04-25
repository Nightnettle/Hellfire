#pragma once
#include "..\SDK\CUserCmd.h"
#include "../SDK/CViewSetup.h"

typedef void(__thiscall* OverrideViewFn)(void* ecx, SDK::CViewSetup* pSetup);

extern OverrideViewFn original_override_view;

void __fastcall HookedOverrideView(void* ecx, void* edx, SDK::CViewSetup* pSetup);