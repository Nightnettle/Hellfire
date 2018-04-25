#pragma once
#include "..\SDK\CUserCmd.h"

typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);

extern PaintTraverseFn original_paint_traverse;

void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce);