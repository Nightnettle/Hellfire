#pragma once
#include "..\SDK\CUserCmd.h"

typedef void(__thiscall* SceneEndFn)(void*);

extern SceneEndFn original_scene_end;

void __fastcall HookedSceneEnd(void*, void*);