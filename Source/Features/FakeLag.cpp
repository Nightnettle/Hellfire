#include "../includes.h"
#include "../Utils/Interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CUserCmd.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CTrace.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/NetChannel.h"
#include "../SDK/CBaseAnimState.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/AutoWall.h"
#include "../FEATURES/FakeLag.h"

void CFakeLag::do_fakelag()
{
	GLOBAL::should_send_packet = true;
	int choke_amount;

	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!local_player || local_player->GetHealth() <= 0)
		return;

	auto net_channel = Interfaces::Engine->GetNetChannel();
	if (!net_channel)
		return;

	if (SETTINGS::settings.aa_type == 0 || SETTINGS::settings.aa_type == 5)
		return;

	SDK::CUserCmd* cmd;

	//anzahl ticks * velocity


	//lagcomp breaker, breaking lby breaker :/
/*
	if (SETTINGS::settings.lag_bool)
	{
		auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		float velocity = local_player->GetVelocity().Length2D() * Interfaces::Globals->interval_per_tick;

		int chokeAmmount = 0;

		int amountToChoke = SETTINGS::settings.lag_choke;

		int maxChoke = 15;

		if (local_player->GetVelocity().Length2D() < 0.1)
			return;

		if (velocity < 1)//should be 66/maxChoke instead of 1 but meh
		{
			amountToChoke = maxChoke;
		}
		else
		{
			while (velocity * chokeAmmount <= 70)
			{
				//While they aren't breaking lag comp
				chokeAmmount++;
			}
			if (chokeAmmount > maxChoke)
			{
				amountToChoke = maxChoke;
			}
			else

			{
				int minChoke = chokeAmmount;
				int fromMenu = SETTINGS::settings.lag_choke;
				int stepVal = minChoke;

				if (minChoke < fromMenu)
				{
					stepVal = fromMenu;
				}

				static bool switcher = false;
				static int counter = minChoke;
				if (counter < minChoke)
					counter = minChoke;

				if (!net_channel->m_nChokedPackets)
				{
					switcher = !switcher;
					counter++;
					if (counter > maxChoke)
						counter = minChoke;
				}

				if (switcher)
					amountToChoke = stepVal;
				else
					amountToChoke = counter;

			}
		}
	}
	else
	{
		int chokeAmmount = 1;

		int amountToChoke = SETTINGS::settings.lag_choke;

		if (net_channel->m_nChokedPackets >= min(15, amountToChoke))
			GLOBAL::should_send_packet = true;
		else
			GLOBAL::should_send_packet = false;
	}
	*/
	if (SETTINGS::settings.lag_bool)
	{
		if (fake_walk)
		{
			choke_amount = 15;	
		}
		else if (local_player->GetVelocity().Length2D() < 0.1)
		{
			choke_amount = 1; // standing flag (1)
		}
		else if (local_player->GetVelocity().Length2D() > 0.1 || !(local_player->GetFlags() & FL_ONGROUND))
		{
				choke_amount = SETTINGS::settings.lag_choke; // moving flag (3)
		}
		
	}
	else
		choke_amount = 1;

	if (net_channel->m_nChokedPackets >= min(15, choke_amount))
		GLOBAL::should_send_packet = true;
	else
		GLOBAL::should_send_packet = false;
		
}

CFakeLag* fakelag = new CFakeLag();