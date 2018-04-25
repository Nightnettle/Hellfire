#include "../includes.h"

#include "../Utils/Interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/ISurface.h"

#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Visuals.h"
#include "../Hooks/Sounds.h"
#include "../Utils/render.h"
#include "EventListener.h"
#include <playsoundapi.h>


#pragma comment(lib, "Winmm.lib")

CGameEvents::PlayerHurtListener player_hurt_listener;
CGameEvents::BulletImpactListener bullet_impact_listener;
CGameEvents::RoundStartListener round_start_listener;
CGameEvents::PlayerDeathListener player_death_listener;

void CGameEvents::InitializeEventListeners()
{
	//Interfaces::GameEventManager->AddListener(&item_purchase_listener, "item_purchase", false);
	Interfaces::GameEventManager->AddListener(&player_hurt_listener, "player_hurt", false);
	Interfaces::GameEventManager->AddListener(&bullet_impact_listener, "bullet_impact", false);
	Interfaces::GameEventManager->AddListener(&round_start_listener, "round_start", false);
	Interfaces::GameEventManager->AddListener(&player_death_listener, "player_death", false);
}

void CGameEvents::PlayerDeathListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	SDK::player_info_t player_info;
	if (!Interfaces::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;

	if (SETTINGS::settings.headshotonly && entity_attacker == local_player)
	{
		if (!game_event->GetBool("headshot"))
			game_event->SetInt("headshot", 1);
	}
}
void DrawHitmarker()
{
	if (!SETTINGS::settings.hitmark_bool) return;

	int W, H;
	Interfaces::Engine->GetScreenSize(W, H);

	float r = 255.f;
	float g = 0.f;
	float b = 0.f;
	for (int i = 0; i < 1000; i++)
	{
		if (SETTINGS::settings.hitmark_bool)
		{
			RENDER::DrawLine(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, CColor(r, g, b, 255.f));
			RENDER::DrawLine(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, CColor(r, g, b, 255.f));
			RENDER::DrawLine(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, CColor(r, g, b, 255.f));
			RENDER::DrawLine(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, CColor(r, g, b, 255.f));

		}
		i++;
	}
}
void CGameEvents::PlayerHurtListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	SDK::player_info_t player_info;
	if (!Interfaces::Engine->GetPlayerInfo(entity->GetIndex(), &player_info))
		return;
	if (!SETTINGS::settings.hitmarker_bool) return;

	if (entity_attacker == local_player)
	{
		if (!SETTINGS::settings.hitmark_bool) return;
		DrawHitmarker();
		hitmarker_time = 255;
		if (SETTINGS::settings.hitmarker_bool == 1)
		{ 
			Interfaces::Surface->IPlaySound("C:\\Pasteware\\Hitmarker.wav");
			//Interfaces::Surface->IPlaySound("buttons\\arena_switch_press_02.wav");
		} 
		else if (SETTINGS::settings.hitmarker_bool == 2)
		{
			PlaySoundA(NIGGERK::roblox, NULL, SND_ASYNC | SND_MEMORY);
		}

		shots_hit[entity->GetIndex()]++;
	}
}

void CGameEvents::RoundStartListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!local_player)
		return;

	auto entity = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("userid")));
	if (!entity)
		return;

	auto entity_attacker = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	if (!entity_attacker)
		return;

	if (entity->GetTeam() == local_player->GetTeam())
		return;

	if (SETTINGS::settings.buybot == 0)
	{
		// disabled
		return;
	}
	else if (SETTINGS::settings.buybot == 1)
	{
		Interfaces::Engine->ClientCmd_Unrestricted("buy g3sg1; buy scar20; buy elite; buy vest; buy vesthelm; buy defuser; buy molotov; buy incgrenade; buy hegrenade; buy smokegrenade;");
		// auto
	}
	else if (SETTINGS::settings.buybot == 2)
	{
		Interfaces::Engine->ClientCmd_Unrestricted("buy ssg08; buy elite; buy vest; buy vesthelm; buy defuser; buy molotov; buy incgrenade; buy hegrenade; buy smokegrenade;");
		// scout
	}
}

void CGameEvents::BulletImpactListener::FireGameEvent(SDK::IGameEvent* game_event)
{
	if (!game_event)
		return;

	int iUser = Interfaces::Engine->GetPlayerForUserID(game_event->GetInt("userid"));

	auto entity = reinterpret_cast<SDK::CBaseEntity*>(Interfaces::ClientEntityList->GetClientEntity(iUser));

	if (!entity)
		return;

	if (entity->GetIsDormant())
		return;

	float x, y, z;
	x = game_event->GetFloat("x");
	y = game_event->GetFloat("y");
	z = game_event->GetFloat("z");

	
	Utils::BulletImpact_t impact(entity, Vector(x, y, z), Interfaces::Globals->curtime, iUser == Interfaces::Engine->GetLocalPlayer() ? GREEN : RED);

	visuals->Impacts.push_back(impact);
}