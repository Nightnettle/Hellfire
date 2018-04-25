#include "../includes.h"
#include "../UTILS/Interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ConVar.h"
#include "../SDK/AnimLayer.h"
#include "../UTILS/qangle.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"

Vector old_calcangle(Vector dst, Vector src)
{
	Vector angles;

	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[0] * delta[0] + delta[1] * delta[1]);
	angles.x = (float)(atan(delta[2] / hyp) * 180.0 / 3.14159265);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles.z = 0.0f;

	if (delta[0] >= 0.0)
	{
		angles.y += 180.0f;
	}

	return angles;
}

float old_normalize(float Yaw)
{
	if (Yaw > 180)
	{
		Yaw -= (round(Yaw / 360) * 360.f);
	}
	else if (Yaw < -180)
	{
		Yaw += (round(Yaw / 360) * -360.f);
	}
	return Yaw;
}

float curtime(SDK::CUserCmd* ucmd) {
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return 0;

	int g_tick = 0;
	SDK::CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = (float)local_player->GetTickBase();
	}
	else {
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::Globals->interval_per_tick;
	return curtime;
}

bool find_layer(SDK::CBaseEntity* entity, int act, SDK::CAnimationLayer *set)
{
	for (int i = 0; i < 13; i++)
	{
		SDK::CAnimationLayer layer = entity->GetAnimOverlay(i);
		const int activity = entity->GetSequenceActivity(layer.m_nSequence);
		if (activity == act) {
			*set = layer;
			return true;
		}
	}
	return false;
}

void CResolver::record(SDK::CBaseEntity* entity, float new_yaw)
{
	if (entity->GetVelocity().Length2D() > 36)
		return;

	auto c_baseweapon = reinterpret_cast<SDK::CBaseWeapon*>(Interfaces::ClientEntityList->GetClientEntity(entity->GetActiveWeaponIndex()));

	if (!c_baseweapon)
		return;

	auto &info = player_info[entity->GetIndex()];

	if (entity->GetActiveWeaponIndex() && info.last_ammo < c_baseweapon->GetLoadedAmmo()) {
		//ignore the yaw when it is from shooting (will be looking at you/other player)
		info.last_ammo = c_baseweapon->GetLoadedAmmo();
		return;
	}

	info.unresolved_yaw.insert(info.unresolved_yaw.begin(), new_yaw);
	if (info.unresolved_yaw.size() > 20) {
		info.unresolved_yaw.pop_back();
	}

	if (info.unresolved_yaw.size() < 2)
		return;

	auto average_unresolved_yaw = 0;
	for (auto val : info.unresolved_yaw)
		average_unresolved_yaw += val;
	average_unresolved_yaw /= info.unresolved_yaw.size();

	int delta = average_unresolved_yaw - entity->GetLowerBodyYaw();
	auto big_math_delta = abs((((delta + 180) % 360 + 360) % 360 - 180));

	info.lby_deltas.insert(info.lby_deltas.begin(), big_math_delta);
	if (info.lby_deltas.size() > 10) {
		info.lby_deltas.pop_back();
	}
}

static void nospread_resolve(SDK::CBaseEntity* player, int entID) //gay but very effective reversed aw air resolver
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	Vector local_position = local_player->GetVecOrigin() + local_player->GetViewOffset();

	float atTargetAngle = Utils::CalcAngle(local_player->GetHealth() <= 0 ? local_player->GetVecOrigin() : local_position, player->GetVecOrigin()).y;
	Vector velocityAngle;
	MATH::VectorAngles(player->GetVelocity(), velocityAngle);

	float primaryBaseAngle = player->GetLowerBodyYaw();
	float secondaryBaseAngle = velocityAngle.y;

	switch ((shots_missed[entID]) % 15)
	{
	case 0:
		player->EasyEyeAngles()->yaw = atTargetAngle + 180.f;
		break;
	case 1:
		player->EasyEyeAngles()->yaw = velocityAngle.y + 180.f;
		break;
	case 2:
		player->EasyEyeAngles()->yaw = primaryBaseAngle;
		break;
	case 3:
		player->EasyEyeAngles()->yaw = primaryBaseAngle - 45.f;
		break;
	case 4:
		player->EasyEyeAngles()->yaw = primaryBaseAngle + 90.f;
		break;
	case 5:
		player->EasyEyeAngles()->yaw = primaryBaseAngle - 130.f;
		break;
	case 6:
		player->EasyEyeAngles()->yaw = primaryBaseAngle - 180.f;
		break;
	case 7:
		player->EasyEyeAngles()->yaw = secondaryBaseAngle;
		break;
	case 8:
		player->EasyEyeAngles()->yaw = secondaryBaseAngle - 40.f;
		break;
	case 9:
		player->EasyEyeAngles()->yaw = secondaryBaseAngle - 90.f;
		break;
	case 10:
		player->EasyEyeAngles()->yaw = secondaryBaseAngle - 130.f;
		break;
	case 11:
		player->EasyEyeAngles()->yaw = secondaryBaseAngle - 70.f;
		break;
	case 12:
		player->EasyEyeAngles()->yaw = primaryBaseAngle + 45.f;
		break;
	case 13:
		player->EasyEyeAngles()->yaw = primaryBaseAngle + 135.f;
		break;
	case 14:
		player->EasyEyeAngles()->yaw = primaryBaseAngle - 90.f;
		break;
	}
}


void CResolver::resolve(SDK::CBaseEntity* entity)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!entity)
		return;

	if (!local_player)
		return;

	bool is_local_player = entity == local_player;
	bool is_teammate = local_player->GetTeam() == entity->GetTeam() && !is_local_player;

	if (is_local_player)
		return;

	if (is_teammate)
		return;

	if (entity->GetHealth() <= 0)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	
	auto &info = player_info[entity->GetIndex()];

	
	float fl_lby = entity->GetLowerBodyYaw();

	info.lby = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw(), 0.f);
	info.inverse = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() + 180.f, 0.f);
	info.last_lby = Vector(entity->GetEyeAngles().x, info.last_moving_lby, 0.f);
	info.inverse_left = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() + 115.f, 0.f);
	info.inverse_right = Vector(entity->GetEyeAngles().x, entity->GetLowerBodyYaw() - 115.f, 0.f);

	info.back = Vector(entity->GetEyeAngles().x, Utils::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y + 180.f, 0.f);
	info.right = Vector(entity->GetEyeAngles().x, Utils::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y + 70.f, 0.f);
	info.left = Vector(entity->GetEyeAngles().x, Utils::CalcAngle(entity->GetVecOrigin(), local_player->GetVecOrigin()).y - 70.f, 0.f);

	info.backtrack = Vector(entity->GetEyeAngles().x, lby_to_back[entity->GetIndex()], 0.f);

	shots_missed[entity->GetIndex()] = shots_fired[entity->GetIndex()] - shots_hit[entity->GetIndex()];
	if (SETTINGS::settings.fakefix_bool)
		info.is_moving = entity->GetVelocity().Length2D() > 0.1 && entity->GetFlags() & FL_ONGROUND && !info.could_be_slowmo;
	else
		info.is_moving = entity->GetVelocity().Length2D() > 0.1 && entity->GetFlags() & FL_ONGROUND;
	info.is_jumping = !entity->GetFlags() & FL_ONGROUND;
	info.could_be_slowmo = entity->GetVelocity().Length2D() > 6 && entity->GetVelocity().Length2D() < 36 && !info.is_crouching;
	info.is_crouching = entity->GetFlags() & FL_DUCKING;
	update_time[entity->GetIndex()] = info.next_lby_update_time;

	static float old_simtime[65];
	if (entity->GetSimTime() != old_simtime[entity->GetIndex()])
	{
		using_fake_angles[entity->GetIndex()] = entity->GetSimTime() - old_simtime[entity->GetIndex()] == Interfaces::Globals->interval_per_tick; //entity->GetSimTime() - old_simtime[entity->GetIndex()] >= TICKS_TO_TIME(2)
		old_simtime[entity->GetIndex()] = entity->GetSimTime();
	}

	//--- Actual Angle Resolving ---//
	if (!using_fake_angles[entity->GetIndex()])
	{
		if (backtrack_tick[entity->GetIndex()])
		{
			resolve_type[entity->GetIndex()] = 7;
			entity->SetEyeAngles(info.backtrack);
		}
		else if (info.stored_lby != entity->GetLowerBodyYaw())// || entity->GetSimTime() > info.next_lby_update_time) //lby prediction
		{
			entity->SetEyeAngles(info.lby);
			//info.next_lby_update_time = entity->GetSimTime() + 1.1;
			info.stored_lby = entity->GetLowerBodyYaw();
			resolve_type[entity->GetIndex()] = 3;
		}
		else if (info.is_jumping)
		{
			nospread_resolve(entity, entity->GetIndex());
		}
		else if (info.is_moving) //while moving
		{
			entity->SetEyeAngles(info.lby);
			info.last_moving_lby = entity->GetLowerBodyYaw();
			info.stored_missed = shots_missed[entity->GetIndex()];
			resolve_type[entity->GetIndex()] = 1;
		}
		else
		{
			if (shots_missed[entity->GetIndex()] > info.stored_missed) //1 shot since stop moved
			{
				resolve_type[entity->GetIndex()] = 4;
				switch (shots_missed[entity->GetIndex()] % 4)
				{
				case 0: entity->SetEyeAngles(info.inverse); break;
				case 1: entity->SetEyeAngles(info.left); break;
				case 2: entity->SetEyeAngles(info.back); break;
				case 3: entity->SetEyeAngles(info.right); break;
				}
			}
			else // QUANTIOM LOOK BACK HEREL ATER
			{
				entity->SetEyeAngles(info.last_lby);
				resolve_type[entity->GetIndex()] = 5;
			}
		}
	}


}

CResolver* resolver = new CResolver();