#pragma once
#include "..\SDK\CUserCmd.h"
typedef void(__thiscall* FrameStageNotifyFn)(void*, int);

extern FrameStageNotifyFn original_frame_stage_notify;

void __fastcall HookedFrameStageNotify(void*, void*, int);