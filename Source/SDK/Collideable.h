#pragma once
#include "../includes.h"
#include "../Utils/Interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/IViewRenderBeams.h"
#include "../FEATURES/Backtracking.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CTrace.h"	
#include "../FEATURES/Resolver.h"
#include "../SDK/CGlobalVars.h"
#include "../FEATURES/Visuals.h"
#include "../Utils/render.h"
#include "../SDK/IVDebugOverlay.h"
#include <string.h>

namespace SDK
{
	class Collideable
	{

	public:

		virtual void unk0();
		virtual Vector &Mins() const;
		virtual Vector &Maxs() const;
	};
}