#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	class CBaseWeapon;
}

class CAimbot
{
public:
	void ShootEnemy(SDK::CUserCmd* cmd);
	bool GoodBTTick(int tick);
	void LBYBacktrack(SDK::CUserCmd * pCmd, SDK::CBaseEntity * pLocal, SDK::CBaseEntity * pEntity);
	//bool ValidTick(LagRecord * targetRecord, LagRecord * prevRecord);
	//void GetBestTarget(SDK::CBaseEntity *& targetPlayer, Vector & targetVector, LagRecord *& targetRecord);
	float AcceptedInaccuracy(SDK::CBaseWeapon * weapon);
	Vector SmartBaimPoint(SDK::CBaseEntity * entity);
	bool CanShoot();
	void RemoveRecoil(SDK::CUserCmd* cmd);
	void RotateMovement(float yaw, SDK::CUserCmd * cmd);
	Vector GetHitboxPosition(SDK::CBaseEntity* entity, int hitbox_id);
	void AstroWalk(SDK::CUserCmd*);
private:
	int SelectTarget();
	void QuickStop(SDK::CUserCmd * cmd);
	int HitboxScan(SDK::CBaseEntity * entity); //int
	Vector HitPointScan(SDK::CBaseEntity * entity);
	int SmartBaim(SDK::CBaseEntity * entity);
	SDK::mstudiobbox_t* GetHitbox(SDK::CBaseEntity* entity, int hitbox_index);
	bool MeetsRequirements(SDK::CBaseEntity* entity);
	int GetDamage(Vector position);
	bool SpreadLimit(SDK::CBaseWeapon* weapon);
	float SeedChance(Vector Point);
};

extern CAimbot* aimbot;