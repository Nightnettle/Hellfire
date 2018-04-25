#include "../includes.h"
#include "../Utils/Interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CBaseWeapon.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/AntiAim.h"

float randnum(float Min, float Max)
{
	return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}
int lastms;

float get_curtime(SDK::CUserCmd* ucmd)
{
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

float anim_velocity(SDK::CBaseEntity* LocalPlayer)
{
	if (LocalPlayer->GetAnimState() == nullptr)
		return false;

	int vel = LocalPlayer->GetAnimState()->m_velocity;
	return vel;
}

bool first_supress(const float yaw_to_break, SDK::CUserCmd* cmd)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->GetAnimState();

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (SETTINGS::settings.LBYdelta < 120)
		return false;

	if (animstate->m_velocity > 0.1)
		next_lby_update_time = curtime + 0.22 - TICKS_TO_TIME(1);

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1;
		return true;
	}

	return false;
}

bool second_supress(const float yaw_to_break, SDK::CUserCmd* cmd)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	auto animstate = local_player->GetAnimState();
	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (SETTINGS::settings.LBYdelta < 120)
		return false;

	if (animstate->m_velocity > 0.1)
		next_lby_update_time = curtime + 0.22 + TICKS_TO_TIME(1);


	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1;
		return true;
	}

	return false;
}

bool next_lby_update(const float yaw_to_break, SDK::CUserCmd* cmd)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return false;

	auto animstate = local_player->GetAnimState();

	static float next_lby_update_time = 0;
	float curtime = get_curtime(cmd);

	if (!animstate)
		return false;

	if (!(local_player->GetFlags() & FL_ONGROUND))
		return false;

	if (animstate->m_velocity > 0.1)
		next_lby_update_time = curtime + 0.22f;

	if (next_lby_update_time < curtime)
	{
		next_lby_update_time = curtime + 1.1f;
		return true;
	}

	return false;
}

void CAntiAim::do_antiaim(SDK::CUserCmd* cmd)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (local_player->GetHealth() <= 0)
		return;

	auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(Interfaces::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!weapon)
		return;

	if (cmd->buttons & IN_USE)
		return;

	if (cmd->buttons & IN_ATTACK && aimbot->CanShoot())
		return;

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,	// never moves
		MOVETYPE_ISOMETRIC,			// For players -- in TF2 commander view, etc.
		MOVETYPE_WALK,				// Player only - moving on the ground
		MOVETYPE_STEP,				// gravity, special edge handling -- monsters use this
		MOVETYPE_FLY,				// No gravity, but still collides with stuff
		MOVETYPE_FLYGRAVITY,		// flies through the air + is affected by gravity
		MOVETYPE_VPHYSICS,			// uses VPHYSICS for simulation
		MOVETYPE_PUSH,				// no clip to world, push and crush
		MOVETYPE_NOCLIP,			// No gravity, no collisions, still do velocity/avelocity
		MOVETYPE_LADDER,			// Used by players only when going onto a ladder
		MOVETYPE_OBSERVER,			// Observer movement, depends on player's observer mode
		MOVETYPE_CUSTOM,			// Allows the entity to describe its own physics

									// should always be defined as the last item in the list
		MOVETYPE_LAST = MOVETYPE_CUSTOM,
		MOVETYPE_MAX_BITS = 4
	};

	

	if (weapon->get_full_info()->WeaponType == 9)
		return;

	if (!SETTINGS::settings.aa_bool)
		return;

	//float delta = abs(MATH::NormalizeYaw(GLOBAL::real_angles.y - local_player->GetLowerBodyYaw()));
	//std::cout << std::to_string(delta) << std::endl;
	if (local_player->GetVelocity().Length2D() < 80)
	{


		switch (SETTINGS::settings.pitch_type % 6)
		{
		case 0:
			break;
		case 1:
			cmd->viewangles.x = 89.000000;
			break;
		case 2:
			cmd->viewangles.x = -89.000000;
			break;
		case 3:
			cmd->viewangles.x = 0.000000;
			break;
		case 4:
			cmd->viewangles.x = SETTINGS::settings.CustomPitch;
			break;

		}

		//Note by LaMe, please don't make the code ugly, you see void CAntiAim::fix_movement(SDK::CUserCmd* cmd) at the bottom of this file, why do you've deleted all the functions and made it worse???

		switch (SETTINGS::settings.aa_type % 6)
		{
		case 0: //Off
			break;
		case 1: //Static
			if (SETTINGS::settings.flip_bool)
			{
				cmd->viewangles.y -= SETTINGS::settings.AddReal;
			}
			else {
				cmd->viewangles.y += SETTINGS::settings.AddReal;
			}
			break;
		case 2: //Spin
			if (!GLOBAL::should_send_packet)
			{
				static int y21 = -179;
				int spinBotSpeedFast1 = SETTINGS::settings.SpinSpeed;

				y21 += spinBotSpeedFast1;

				if (y21 >= 179)
					y21 = -179;

				cmd->viewangles.y = y21;
			}
			break;
		case 3: //Jitter
			if (!GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= SETTINGS::settings.AddReal + ((rand() % (int)SETTINGS::settings.JitterRange) - ((int)SETTINGS::settings.JitterRange * 0.5f));
				}
				else {
					cmd->viewangles.y += SETTINGS::settings.AddReal + ((rand() % (int)SETTINGS::settings.JitterRange) - ((int)SETTINGS::settings.JitterRange * 0.5f));
				}
			}
			break;
		case 4: //LBY
			static float last_real;
			if (!GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.LBYdelta, cmd))
						cmd->viewangles.y = last_real + SETTINGS::settings.LBYdelta;
					else
					{
						cmd->viewangles.y -= SETTINGS::settings.AddReal;
						last_real = cmd->viewangles.y;
					}
				}
				else
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.LBYdelta, cmd))
						cmd->viewangles.y = last_real + SETTINGS::settings.LBYdelta;
					else
					{
						cmd->viewangles.y += SETTINGS::settings.AddReal;
						last_real = cmd->viewangles.y;
					}
				}
			}
			break;
	   //No AA Yet
		case 5:

			break;
		}




		//Fake Angles
		switch (SETTINGS::settings.fakeaa_type % 5)
		{
			//Off
		case 0:
			break;
			//Static
		case 1:
			if (GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= SETTINGS::settings.AddFake;
				}
				else {
					cmd->viewangles.y += SETTINGS::settings.AddFake;
				}
			}
			break;
			//Spin
		case 2:
		{
			if (GLOBAL::should_send_packet)
			{
				static int y2 = -179;

				int spinBotSpeedFast = SETTINGS::settings.SpinSpeed;

				y2 += spinBotSpeedFast;

				if (y2 >= 179)
					y2 = -179;

				cmd->viewangles.y = y2;
			}
		}
		break;	//Jitter
		case 3:
			if (GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= SETTINGS::settings.AddFake + ((rand() % (int)SETTINGS::settings.JitterRange) - ((int)SETTINGS::settings.JitterRange * 0.5f));
				}
				else {
					cmd->viewangles.y += SETTINGS::settings.AddFake + ((rand() % (int)SETTINGS::settings.JitterRange) - ((int)SETTINGS::settings.JitterRange * 0.5f));
				}
			}
			break;
			//EMPTY
		case 4:
			if (GLOBAL::should_send_packet)
			{

			}
			break;
		}
	}
	if (local_player->GetVelocity().Length2D() > 80)
	{
		switch (SETTINGS::settings.movingpitch_type % 6)
		{
		case 0:
			break;
		case 1:
			cmd->viewangles.x = 89.000000;
			break;
		case 2:
			cmd->viewangles.x = -89.000000;
			break;
		case 3:
			cmd->viewangles.x = 0.000000;
			break;
		case 4:
			cmd->viewangles.x = SETTINGS::settings.CustomMovingPitch;
			break;
		case 5:
			if (FL_ONGROUND)
			{
				cmd->viewangles.x = 89.000000;
			}
			else
			{
				cmd->viewangles.x = 0.000000;
			}
		}

		//Note by LaMe, please don't make the code ugly, you see void CAntiAim::fix_movement(SDK::CUserCmd* cmd) at the bottom of this file, why do you've deleted all the functions and made it worse???

		switch (SETTINGS::settings.realmove_type % 6)
		{
		case 0: //Off
			break;
		case 1: //Static
			if (SETTINGS::settings.flip_bool)
			{
				cmd->viewangles.y -= SETTINGS::settings.AddMovingReal;
			}
			else {
				cmd->viewangles.y += SETTINGS::settings.AddMovingReal;
			}
			break;
		case 2: //Spin
			if (!GLOBAL::should_send_packet)
			{
				static int y21 = -179;
				int spinBotSpeedFast1 = SETTINGS::settings.MovingSpinSpeed;

				y21 += spinBotSpeedFast1;

				if (y21 >= 179)
					y21 = -179;

				cmd->viewangles.y = y21;
			}
			break;
		case 3: //Jitter
			if (!GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= SETTINGS::settings.AddMovingReal + ((rand() % (int)SETTINGS::settings.MovingJitterRange) - ((int)SETTINGS::settings.MovingJitterRange * 0.5f));
				}
				else {
					cmd->viewangles.y += SETTINGS::settings.AddMovingReal + ((rand() % (int)SETTINGS::settings.MovingJitterRange) - ((int)SETTINGS::settings.MovingJitterRange * 0.5f));
				}
			}
			break;
		case 4: //LBY
			static float last_real;
			if (!GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.LBYdelta, cmd))
						cmd->viewangles.y = last_real + SETTINGS::settings.LBYdelta;
					else
					{
						cmd->viewangles.y -= SETTINGS::settings.AddMovingReal;
						last_real = cmd->viewangles.y;
					}
				}
				else
				{
					if (next_lby_update(cmd->viewangles.y + SETTINGS::settings.LBYdelta, cmd))
						cmd->viewangles.y = last_real + SETTINGS::settings.LBYdelta;
					else
					{
						cmd->viewangles.y += SETTINGS::settings.AddMovingReal;
						last_real = cmd->viewangles.y;
					}
				}
			}
			break;
			//Empty
		case 5:

			break;
		}

		// Flip

		//SETTINGS::settings.FakeAngles = cmd->viewangles.y + SETTINGS::settings.AddMovingFake;

		//Fake Angles
		switch (SETTINGS::settings.fakemove_type % 5)
		{
			//Off
		case 0:
			break;
			//Static
		case 1:
			if (GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= SETTINGS::settings.AddMovingFake;
				}
				else {
					cmd->viewangles.y += SETTINGS::settings.AddMovingFake;
				}
			}
			break;
			//Spin
		case 2:
		{
			if (GLOBAL::should_send_packet)
			{
				static int y2 = -179;

				int spinBotSpeedFast = SETTINGS::settings.MovingSpinSpeed;

				y2 += spinBotSpeedFast;

				if (y2 >= 179)
					y2 = -179;

				cmd->viewangles.y = y2;
			}
		}
		break;	//Jitter
		case 3:
			if (GLOBAL::should_send_packet)
			{
				if (SETTINGS::settings.flip_bool)
				{
					cmd->viewangles.y -= SETTINGS::settings.AddMovingFake + ((rand() % (int)SETTINGS::settings.MovingJitterRange) - ((int)SETTINGS::settings.MovingJitterRange * 0.5f));
				}
				else {
					cmd->viewangles.y += SETTINGS::settings.AddMovingFake + ((rand() % (int)SETTINGS::settings.MovingJitterRange) - ((int)SETTINGS::settings.MovingJitterRange * 0.5f));
				}
			}
			break;
			//EMPTY
		case 4:
			if (GLOBAL::should_send_packet)
			{

			}
			break;
		}
	}
}

void CAntiAim::fix_movement(SDK::CUserCmd* cmd)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	Vector real_viewangles;
	Interfaces::Engine->GetViewAngles(real_viewangles);

	Vector vecMove(cmd->forwardmove, cmd->sidemove, cmd->upmove);
	float speed = sqrt(vecMove.x * vecMove.x + vecMove.y * vecMove.y);

	Vector angMove;
	MATH::VectorAngles(vecMove, angMove);

	float yaw = DEG2RAD(cmd->viewangles.y - real_viewangles.y + angMove.y);

	cmd->forwardmove = cos(yaw) * speed;
	cmd->sidemove = sin(yaw) * speed;

	cmd->viewangles = MATH::NormalizeAngle(cmd->viewangles);
}

void CAntiAim::localviewfake(SDK::CUserCmd* cmd)
{
	if (GLOBAL::should_send_packet)
		cmd->viewangles.y += 0;
}

CAntiAim* antiaim = new CAntiAim();