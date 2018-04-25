#include "Createmove.h"
#include "../Utils/Interfaces.h"
#include "../Utils/offsets.h"
#include "../SDK/CInput.h"
#include "../SDK/IClient.h"
#include "../SDK/CPanel.h"
#include "../Utils/render.h"
#include "../SDK/ConVar.h"
#include "../SDK/CGlowObjectManager.h"
#include "../SDK/IEngine.h"
#include "../SDK/CTrace.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/RecvData.h"
#include "../Utils/NetvarHookManager.h"
#include "../SDK/ModelInfo.h"
#include "../SDK/ModelRender.h"
#include "../SDK/RenderView.h"
#include "../SDK/CTrace.h"
#include "../SDK/CViewSetup.h"
#include "../SDK/CGlobalVars.h"
#include "../FEATURES/Movement.h"
#include "../FEATURES/Visuals.h"
#include "../FEATURES/Chams.h"
#include "../FEATURES/AntiAim.h"
#include "../FEATURES/Aimbot.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/FakeWalk.h"
#include "../FEATURES/FakeLag.h"
#include "../Features/Aimbot.h"


typedef bool(__stdcall* CreateMoveFn)(float, SDK::CUserCmd*);

CreateMoveFn original_create_move;

char* const chatspam[] =
{
	"Hellfire > ALL",
	"Hellfire dominates all",
	"Keep getting tapped? Get HELLFIRE",
	"Hellfire > ALL",
	"Hellfire > ALL",
	"Hellfire > ALL"
};

void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	Interfaces::Engine->ClientCmd_Unrestricted(buffer);
}

inline void SetName(const char* new_name)
{
	static auto name = Interfaces::cvar->FindVar("name");

	name->SetValue(new_name);
	*reinterpret_cast<int*>(uintptr_t(&name->fnChangeCallback) + 0xC) = 0;
}



bool __stdcall HookedCreateMove(float sample_input_frametime, SDK::CUserCmd* cmd)
{
	auto local_player = Interfaces::ClientEntityList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	auto weapon = Interfaces::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex());

	if (!cmd || cmd->command_number == 0)
		return false;

	uintptr_t* FPointer; __asm { MOV FPointer, EBP }
	byte* SendPacket = (byte*)(*FPointer - 0x1C);

	if (!SendPacket)
		return false;

	GLOBAL::should_send_packet = *SendPacket;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
	

		auto mat = Interfaces::cvar->FindVar("mat_showlowresimage");
		auto mat1 = Interfaces::cvar->FindVar("mat_fullbright");
		auto mat2 = Interfaces::cvar->FindVar("mat_showmiplevels");
		auto mat3 = Interfaces::cvar->FindVar("mat_drawgray");
		

		switch (SETTINGS::settings.render_mode % 4)
		{
		case 0:
			mat->SetValue(0);
			mat1->SetValue(0);
			mat2->SetValue(0);
			mat3->SetValue(0);
			
			break;
		case 1:
			mat->SetValue(1);
			mat1->SetValue(0);
			mat2->SetValue(0);
			mat3->SetValue(0);
			
			break;
		case 2:
			mat->SetValue(0);
			mat1->SetValue(0);
			mat2->SetValue(1);
			mat3->SetValue(0);
			
			break;
		case 3:
			mat->SetValue(0);
			mat1->SetValue(1);
			mat2->SetValue(0);
			mat3->SetValue(0);
			
			break;
		case 4:
			mat->SetValue(0);
			mat1->SetValue(0);
			mat2->SetValue(0);
			mat3->SetValue(1);
			
			break;
		}

		if (SETTINGS::settings.astrowalk)
			aimbot->AstroWalk(cmd);

		if (SETTINGS::settings.ragdoll)
		{
			auto mat4 = Interfaces::cvar->FindVar("cl_ragdoll_gravity");
			mat4->SetValue(0);
		}
		else {
			auto mat4 = Interfaces::cvar->FindVar("cl_ragdoll_gravity");
			mat4->SetValue(0);
		}

		auto weapon = reinterpret_cast<SDK::CBaseWeapon*>(Interfaces::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

		// KNIFEBOT 
		

		if (SETTINGS::settings.namespam)
		{
			char name[151];
			char fucked_char = static_cast<char>(-1);
			for (auto i = 0; i <= 150; i++)
				name[i] = fucked_char;

			const char nick[12] = "PerfectHook";
			memcpy(name, nick, 11);

			SetName(name);
		}

		if (SETTINGS::settings.aim_type == 0)
			slidebitch->do_fakewalk(cmd);

		if (SETTINGS::settings.bhop_bool)
			movement->bunnyhop(cmd);

		if (SETTINGS::settings.strafe_bool)
			movement->autostrafer(cmd);

		if (SETTINGS::settings.aim_type == 0)
		{
			aimbot->ShootEnemy(cmd);
			aimbot->RemoveRecoil(cmd);
			backtracking->backtrack_player(cmd);
			fakelag->do_fakelag();
		}

		if (SETTINGS::settings.chatspam)
		{
			static size_t lastTime = 0;
			static int counter = 0;

			if (GetTickCount() > lastTime)
			{
				counter = counter + 1;
				if (counter > 5)
					counter = 0;

				SayInChat(chatspam[counter]);

				lastTime = GetTickCount() + 850;
			}
		}

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

		if (SETTINGS::settings.moonwalk)
		{
			enum MoveType_t;
			if (enum MoveType_t() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER | FL_ONGROUND))
				return;
			{
				if (cmd->buttons & IN_FORWARD)
				{
					cmd->forwardmove = 450;
					cmd->buttons &= ~IN_FORWARD;
					cmd->buttons |= IN_BACK;
				}
				else if (cmd->buttons & IN_BACK)
				{
					cmd->forwardmove = -450;
					cmd->buttons &= ~IN_BACK;
					cmd->buttons |= IN_FORWARD;
				}

				if (cmd->buttons & IN_MOVELEFT)
				{
					cmd->sidemove = -450;
					cmd->buttons &= ~IN_MOVELEFT;
					cmd->buttons |= IN_MOVERIGHT;
				}
				else if (cmd->buttons & IN_MOVERIGHT)
				{
					cmd->sidemove = 450;
					cmd->buttons &= ~IN_MOVERIGHT;
					cmd->buttons |= IN_MOVELEFT;
				}
			}
		}

		if (SETTINGS::settings.nosky)
		{
			auto skybox = Interfaces::cvar->FindVar("sv_skyname");
			if (skybox) skybox->SetValue("sky_l4d_rural02_ldr");
		}
		else {
			static SDK::ConVar* sv_skyname = Interfaces::cvar->FindVar("sv_skyname");
			sv_skyname->nFlags &= ~FCVAR_CHEAT;
			sv_skyname->SetValue("vertigoblue_hdr");
		}



		

		// No Post Processing
		static SDK::ConVar* postprocess = Interfaces::cvar->FindVar("mat_postprocess_enable");
		if (SETTINGS::settings.nopostprocess)
		{
			if (Interfaces::Engine->IsInGame())
			{
				postprocess->SetValue(0);
			}
			else 
			{
				if (Interfaces::Engine->IsInGame())
				{
					postprocess->SetValue(1);
				}
			}
		}

		if (SETTINGS::settings.clantag == 1)
		{
			int iLastTime;

			if (int(Interfaces::Globals->curtime) != iLastTime)
			{
			#define XorStr( s ) ( s )
				switch (int(Interfaces::Globals->curtime * 4) % 13)
				{
				/* removed clantag, it is considered ads on UC. - quantiom */

				

				
				}
			}

			iLastTime = int(Interfaces::Globals->curtime);
		}
		else if (SETTINGS::settings.clantag == 2)
		{
			
				int iLastTime;

				if (int(Interfaces::Globals->curtime) != iLastTime)
				{
					// gamesense clantag

					#define XorStr( s ) ( s )
					switch (int(Interfaces::Globals->curtime * 2.4) % 28)
					{
					case 0: setclantag(XorStr("                  ")); break;
					case 1: setclantag(XorStr("                ga")); break;
					case 2: setclantag(XorStr("               gam")); break;
					case 3: setclantag(XorStr("              game")); break;
					case 4: setclantag(XorStr("             games")); break;
					case 5: setclantag(XorStr("            gamese")); break;
					case 6: setclantag(XorStr("           gamesen")); break;
					case 7: setclantag(XorStr("          gamesens")); break;
					case 8: setclantag(XorStr("         gamesense")); break;
					case 9:setclantag(XorStr("        gamesense ")); break;
					case 10:setclantag(XorStr("       gamesense  ")); break;
					case 11:setclantag(XorStr("      gamesense   ")); break;
					case 12:setclantag(XorStr("     gamesense    ")); break;
					case 13:setclantag(XorStr("    gamesense     ")); break;
					case 14:setclantag(XorStr("    gamesense     ")); break;
					case 15:setclantag(XorStr("   gamesense      ")); break;
					case 16:setclantag(XorStr("  gamesense       ")); break;
					case 17:setclantag(XorStr(" gamesense        ")); break;
					case 18:setclantag(XorStr("gamesense         ")); break;
					case 19:setclantag(XorStr("amesense          ")); break;
					case 20:setclantag(XorStr("mesense           ")); break;
					case 22:setclantag(XorStr("esense            ")); break;
					case 23:setclantag(XorStr("sense             ")); break;
					case 24:setclantag(XorStr("sens              ")); break;
					case 25:setclantag(XorStr("sen               ")); break;
					case 26:setclantag(XorStr("se                ")); break;

					
					}
				}

				iLastTime = int(Interfaces::Globals->curtime);
			
		}

		if (SETTINGS::settings.aim_type == 1 && SETTINGS::settings.back_bool)
			backtracking->run_legit(cmd);

		if (SETTINGS::settings.aa_bool || SETTINGS::settings.aa_type > 0 || SETTINGS::settings.aim_type == 0)
		{
			antiaim->do_antiaim(cmd);
			antiaim->fix_movement(cmd);
		}

		if (!GLOBAL::should_send_packet)
			GLOBAL::real_angles = cmd->viewangles;
	}

	*SendPacket = GLOBAL::should_send_packet;

	Utils::ClampLemon(cmd->viewangles);

	return false;
}