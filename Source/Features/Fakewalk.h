#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CFakewalk
{
public:
	void do_fakewalk(SDK::CUserCmd * cmd);
};

class CNoRecoil
{
public:
	void DoNoRecoil(SDK::CUserCmd * cmd);
};

extern CFakewalk* slidebitch;