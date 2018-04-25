#include "..\SDK\CUserCmd.h"

typedef bool(__stdcall* CreateMoveFn)(float, SDK::CUserCmd*);

extern CreateMoveFn original_create_move;

bool __stdcall HookedCreateMove(float sample_input_frametime, SDK::CUserCmd * cmd);
bool __stdcall Hooked_InPrediction();