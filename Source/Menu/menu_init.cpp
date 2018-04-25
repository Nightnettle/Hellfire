#include "../includes.h"
#include "../Utils/Interfaces.h"
#include "../SDK/IEngine.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/CInputSystem.h"
#include "../Utils/render.h"
#include "Components.h"

#include "menu_framework.h"

namespace MENU
{
	void InitColors()
	{
		using namespace PPGUI_PP_GUI;
		colors[WINDOW_BODY] = CColor(40, 40, 40);
		colors[WINDOW_TITLE_BAR] = CColor(60, 60, 60); //255, 75, 0
		colors[WINDOW_TEXT] = WHITE;

		colors[GROUPBOX_BODY] = CColor(40, 40, 40, 0);
		colors[GROUPBOX_OUTLINE] = CColor(50, 50, 50);
		colors[GROUPBOX_TEXT] = WHITE;

		colors[SCROLLBAR_BODY] = SETTINGS::settings.menu_col;

		colors[SEPARATOR_TEXT] = WHITE;
		colors[SEPARATOR_LINE] = CColor(60, 60, 60, 255);

		colors[CHECKBOX_CLICKED] = SETTINGS::settings.menu_col; //HOTPINK
		colors[CHECKBOX_NOT_CLICKED] = CColor(60, 60, 60, 255);
		colors[CHECKBOX_TEXT] = WHITE;

		colors[BUTTON_BODY] = CColor(40, 40, 40, 255);
		colors[BUTTON_TEXT] = WHITE;

		colors[COMBOBOX_TEXT] = LIGHTGREY;
		colors[COMBOBOX_SELECTED] = CColor(40, 40, 40, 255);
		colors[COMBOBOX_SELECTED_TEXT] = WHITE;
		colors[COMBOBOX_ITEM] = CColor(20, 20, 20, 255);
		colors[COMBOBOX_ITEM_TEXT] = LIGHTGREY;

		colors[SLIDER_BODY] = CColor(40, 40, 40, 255);
		colors[SLIDER_VALUE] = SETTINGS::settings.menu_col; //HOTPINK
		colors[SLIDER_TEXT] = WHITE;

		colors[TAB_BODY] = CColor(20, 20, 20, 120);
		colors[TAB_TEXT] = LIGHTGREY;
		colors[TAB_BODY_SELECTED] = CColor(40, 40, 40, 150); //HOTPINK
		colors[TAB_TEXT_SELECTED] = WHITE;

		colors[VERTICAL_TAB_BODY] = CColor(255, 40, 40, 255);
		colors[VERTICAL_TAB_TEXT] = WHITE;
		colors[VERTICAL_TAB_OUTLINE] = CColor(0, 0, 0, 100);
		colors[VERTICAL_TAB_BODY_SELECTED] = CColor(255, 20, 20, 255);

		colors[COLOR_PICKER_BODY] = CColor(50, 50, 50, 0);
		colors[COLOR_PICKER_TEXT] = WHITE;
		colors[COLOR_PICKER_OUTLINE] = CColor(0, 0, 0, 0);
	}

	void Do()
	{
		//all of this cool gui stuff made by bolbi, but i decide to just render text and make binds for every feature. sorry bolbi!

		static bool menu_open = false;

		if (Utils::INPUT::input_handler.GetKeyState(VK_INSERT) & 1)
		{
			menu_open = !menu_open;
			Interfaces::Engine->ClientCmd(menu_open ? "cl_mouseenable 0" :
				"cl_mouseenable 1");
			Interfaces::InputSystem->EnableInput(!menu_open);
		}

		if (Utils::INPUT::input_handler.GetKeyState(VK_END) & 1)
		{
			menu_hide = !menu_hide;
		}

		//--- Flip Bool Stuff ---//
		if (Utils::INPUT::input_handler.GetKeyState(86) & 1)
			SETTINGS::settings.flip_bool = !SETTINGS::settings.flip_bool;

		//--- New Menu Rendering System --//
		if (menu_hide)
		{

		}
		else
		{

			if (menu_open)
			{
				using namespace PPGUI_PP_GUI;

				// if menu not open
				if (!menu_open)
					return;

				DrawMouse();

				SetFont(FONTS::menu_window_font);
				WindowBegin("Hellfire", Vector2D(200, 200), Vector2D(900, 650));

				std::vector<std::string> tabs = { "Aimbot", "Visuals", "Misc", "Anti-Aim", "Colors", "Config" };
				std::vector<std::string> aim_mode = { "Rage", "Legit" };
				std::vector<std::string> acc_mode = { "Head Only", "Body Only", "All" };
				std::vector<std::string> chams_mode = { "None", "Visible Only", "XQZ" };
				std::vector<std::string> smoke_type = { "Default", "Wireframe" };
				std::vector<std::string> pitch_mode = { "None", "Emotion", "Up", "Zero", "Custom", };
				std::vector<std::string> movingpitch_mode = { "None", "Emotion", "Up", "Zero", "Custom" };
				std::vector<std::string> buybot_mode = { "None", "Auto", "Scout" };

				std::vector<std::string> aa_realx = { "Off", "Static", "Rotate", "Jitter", "Lowerbody", "EMPTY**" };
				std::vector<std::string> aa_fakex = { "Off", "Static", "Rotate", "Jitter", "EMPTY**" };

				std::vector<std::string> movingaa_realx = { "Off", "Static", "Rotate", "Jitter", "Lowerbody", "EMPTY**" };
				std::vector<std::string> movingaa_fakex = { "Off", "Static", "Rotate", "Jitter", "EMPTY**" };

				std::vector<std::string> rendermode = { "None", "Minecraft mode", "Rainbow mode", "Fullbright mode", "Gray mode" };
				
				std::vector<std::string> configs = { "default", "legit", "autos", "scouts", "pistols" };
				std::vector<std::string> box_style = { "None", "2D", "Debug" };
				std::vector<std::string> tp_mode = { "Real Angle", "Fake Angle" };
				std::vector<std::string> hcweapon = { "Auto", "Scout", "Other" };
				std::vector<std::string> anti_aim_type = { "Standing", "Moving", "In air" };
				std::vector<std::string> clantags = { "None", "Hellfire", "gamesense" };
				std::vector<std::string> hitmarkers = { "None", "Gamesense", "Roblox"};
				std::vector<std::string> hands = { "Default", "No Hands", "Wireframe" };
				std::vector<std::string> materials = { "Glass", "Crystal", "Gold", "Dark Chrome", "Platic Glass", "Velvet", "Crystal Blue", "Detailed Gold" };
				//std::vector<std::string> crosshair_style = { "None", "X-Hair", "Spread Circle", "X-Hair + Spread Circle" };
				std::string config;
				
				switch (Tab("main", tabs, OBJECT_FLAGS::FLAG_NONE))
				{

				case 0:
				{
					Checkbox("Enable Aimbot", SETTINGS::settings.aim_bool);
					Combobox("Aimbot mode", aim_mode, SETTINGS::settings.aim_type);
					if (SETTINGS::settings.aim_type == 0)
					{
						GroupboxBegin("Main", 227.5, 1);
						Combobox("Weapon", hcweapon, SETTINGS::settings.weapon);
						if (SETTINGS::settings.weapon == 0)
						{
							Slider("Hitchance", 0, 100, SETTINGS::settings.auto_chance_val);
							Slider("Minimum damage", 1, 100, SETTINGS::settings.auto_damage_val);
						}
						else if (SETTINGS::settings.weapon == 1)
						{
							Slider("Hitchance", 0, 100, SETTINGS::settings.scout_chance_val);
							Slider("Minimum damage", 1, 100, SETTINGS::settings.scout_damage_val);
						}
						else if (SETTINGS::settings.weapon == 2)
						{
							Slider("Hitchance", 0, 100, SETTINGS::settings.other_chance_val);
							Slider("Minimum damage", 1, 100, SETTINGS::settings.other_damage_val);
						}
						Combobox("Hitbox", acc_mode, SETTINGS::settings.acc_type);
						Checkbox("Multipoint", SETTINGS::settings.multipoint_bool);
						if (SETTINGS::settings.multipoint_bool)
						{
							Slider("Head scale", 0, 1, SETTINGS::settings.point_val);
							Slider("Body scale", 0, 1, SETTINGS::settings.body_val);
						}
						GroupboxBegin("Misc", 40, 1);
						Checkbox("Fakelag fix", SETTINGS::settings.fakefix_bool);
						GroupboxEnd();
						//Checkbox("Automatic Scope", SETTINGS::settings.auto_scope);
						GroupboxEnd();
						
					}
					else
					{
						GroupboxBegin("Misc", 40, 1);
						Checkbox("Backtracking", SETTINGS::settings.back_bool);
						GroupboxEnd();

					}
				}
				break;
				case 1:
				{
					Checkbox("Enabled", SETTINGS::settings.esp_bool);
					
					GroupboxBegin("Flags", 65, 1);
					Checkbox("Local ESP", SETTINGS::settings.localesp);
					Checkbox("Team ESP", SETTINGS::settings.teamesp);
					
					GroupboxEnd();
					GroupboxBegin("Player", 210.5, 1);

					Checkbox("Box", SETTINGS::settings.box_bool);
					Checkbox("Name", SETTINGS::settings.name_bool);
				//	Checkbox("Skeleton", SETTINGS::settings.skeleton_bool);
					Checkbox("Weapon", SETTINGS::settings.weap_bool);
					Checkbox("Info", SETTINGS::settings.info_bool);
					Checkbox("Distance", SETTINGS::settings.distance);
					Checkbox("Health", SETTINGS::settings.health_bool);
					Combobox("Chams", chams_mode, SETTINGS::settings.chams_type);
					
					Checkbox("Wireframe Hands", SETTINGS::settings.wirehands);
					
					Slider("Field Of View", -179, 179, SETTINGS::settings.fov);
					GroupboxEnd();

					GroupboxBegin("World", 357.5, 1);
					Combobox("Smoke Type", smoke_type, SETTINGS::settings.SmokeType);
					Checkbox("Modulation", SETTINGS::settings.night_bool);
					if (SETTINGS::settings.night_bool)
					{
						Slider("Power", 0.0f, 1.0f, SETTINGS::settings.NightStrength);
					}
					Checkbox("Hitmarker", SETTINGS::settings.hitmark_bool);
					
					if (SETTINGS::settings.hitmark_bool)
					{
						Combobox("Sound", hitmarkers, SETTINGS::settings.hitmarker_bool);
					}
					Checkbox("Crosshair", SETTINGS::settings.xhair_bool);
					Checkbox("Spread Crosshair", SETTINGS::settings.recoil_circle);
					// DOES NOT WORK | INSTA CRASH // Checkbox("Remove Visual Recoil", SETTINGS::settings.novisualrecoil_bool);	
					Checkbox("Bullet Tracers", SETTINGS::settings.beam_bool);
					Combobox("Render Mode", rendermode, SETTINGS::settings.render_mode);
					Checkbox("No Flash", SETTINGS::settings.noflash);
					Checkbox("No Sky", SETTINGS::settings.nosky);
					Checkbox("Disable Postprocessing", SETTINGS::settings.nopostprocess);
					Checkbox("Third Person (mouse5)", SETTINGS::settings.tp_bool);
					

					GroupboxEnd();
				}
				break;
				case 2:
				{
					GroupboxBegin("Movement", 130, 1);
					Checkbox("Bunnyhop", SETTINGS::settings.bhop_bool);
					Checkbox("Auto Strafer", SETTINGS::settings.strafe_bool);
					Checkbox("Fakewalk", SETTINGS::settings.fakewalk);
					Checkbox("QuantiomWalk", SETTINGS::settings.moonwalk);
					GroupboxEnd();
					GroupboxBegin("Fakelag", 65, 1);
					Checkbox("Fakelag", SETTINGS::settings.lag_bool);
					if (SETTINGS::settings.lag_bool) Slider("Amount", 1, 15, SETTINGS::settings.lag_choke);
					GroupboxEnd();
					GroupboxBegin("Other", 200, 1);
					Combobox("Clantag", clantags, SETTINGS::settings.clantag);
					Checkbox("Chatspam", SETTINGS::settings.chatspam);
					Checkbox("Watermark", SETTINGS::settings.watermark);
					Checkbox("Fake Media", SETTINGS::settings.headshotonly);
					Checkbox("Knifebot", SETTINGS::settings.knf_Active);
					if (SETTINGS::settings.knf_Active)
					{

						static float flRainbow;
						static POINT curPos;
						float flSpeed = 0.0003f;
						int curWidth = 1;
						GetCursorPos(&curPos);
						GetWindowPos(&curWindowPos);
						curPos.x += curWindowPos.x;
						curPos.y += curWindowPos.y;
						int size1;
						int y;
						Interfaces::Engine->GetScreenSize(y, size1);
					}
					
					Combobox("Buybot", buybot_mode, SETTINGS::settings.buybot);
					Checkbox("Gravity Ragdoll", SETTINGS::settings.ragdoll);
					GroupboxEnd();
				}
				break;
				case 3:
				{
					Checkbox("Enable Anti-Aim", SETTINGS::settings.aa_bool);
					Combobox("Movement Type", anti_aim_type, SETTINGS::settings.antiaimtype);
					if (SETTINGS::settings.antiaimtype == 0)
					{
						GroupboxBegin("Static", 160, 0);
						Combobox("Standing Pitch", pitch_mode, SETTINGS::settings.pitch_type);
						if (SETTINGS::settings.pitch_type == 4)
							Slider("S.Angle", -89, 89, SETTINGS::settings.CustomPitch);
						Combobox("Standing Real", aa_realx, SETTINGS::settings.aa_type);
						Slider("Add S.Real", -180, 180, SETTINGS::settings.AddReal);
						Combobox("Standing Fake", aa_fakex, SETTINGS::settings.fakeaa_type);
						Slider("Add S.Fake", -180, 180, SETTINGS::settings.AddFake);
						GroupboxEnd();
					}
					else if (SETTINGS::settings.antiaimtype == 1)
					{
						GroupboxBegin("Moving", 150, 0);
						Combobox("Moving Pitch", movingpitch_mode, SETTINGS::settings.movingpitch_type);
						if (SETTINGS::settings.movingpitch_type == 4)
							Slider("M.Angle", -89, 89, SETTINGS::settings.CustomMovingPitch);
						Combobox("Moving Real", movingaa_realx, SETTINGS::settings.realmove_type);
						Slider("Add M.Real", -180, 180, SETTINGS::settings.AddMovingReal);
						Combobox("Moving Fake", movingaa_fakex, SETTINGS::settings.fakemove_type);
						Slider("Add M.Fake", -180, 180, SETTINGS::settings.AddMovingFake);
						GroupboxEnd();
					}
					else
					{
						//do nothing
					}
					Checkbox("Show Misc", SETTINGS::settings.showmisc);
					if (SETTINGS::settings.showmisc)
					{
						GroupboxBegin("Misc", 200, 0);
						Slider("LBY Delta", 0, 180, SETTINGS::settings.LBYdelta);
						Slider("Spin Speed", 0.0f, 100.0f, SETTINGS::settings.SpinSpeed);
						Slider("Jitter Range", -180.f, 180.f, SETTINGS::settings.JitterRange);
						Slider("Move Spin Speed", 0.0f, 100.0f, SETTINGS::settings.MovingSpinSpeed);
						Slider("Move Jitter Range", -180.f, 180.f, SETTINGS::settings.MovingJitterRange);
						Checkbox("Manual AA Indicator", SETTINGS::settings.indicator_bool);
						Checkbox("Show Fake Angles", SETTINGS::settings.drawfake_bool);
						GroupboxEnd();
					}
					}
				break;
				case 4:
				{
					
					Separator("Local");
					ColorPicker("Local Box", SETTINGS::settings.selfbox_col);
					ColorPicker("Local Visible Chams", SETTINGS::settings.selfvmodel_col);
					ColorPicker("Local Invisible Chams", SETTINGS::settings.selfimodel_col);
					ColorPicker("Spread Crosshair", SETTINGS::settings.spread_col);
					ColorPicker("Local Tracer", SETTINGS::settings.localbeam_col);
					
					Separator("Friendly");
					ColorPicker("Friendly Box", SETTINGS::settings.friendbox_col);
					ColorPicker("Friendly Visible Chams", SETTINGS::settings.teamvmodel_col);
					ColorPicker("Friendly Invisible Chams", SETTINGS::settings.teamimodel_col);
					ColorPicker("Friendly Tracer", SETTINGS::settings.friendlybeam_col);

					Separator("Enemy");
					ColorPicker("Enemy Box", SETTINGS::settings.box_col);
					ColorPicker("Enemy Visible Chams", SETTINGS::settings.vmodel_col);
					ColorPicker("Ememy Invisible Chams", SETTINGS::settings.imodel_col);
					ColorPicker("Enemy Tracer", SETTINGS::settings.enemybeam_col);

					//Separator("Color Modulation");
					//ColorPicker("World Modualtion", SETTINGS::settings.WorldMod_col);
					//ColorPicker("Prop Modulation", SETTINGS::settings.PropMod_col);

					Separator("Rage Features");
					ColorPicker("Fake Angle Chams", SETTINGS::settings.fake_angle_col);
					ColorPicker("Manual Arrow", SETTINGS::settings.manual_col);
				}
				break;
				case 5:
				{
					GroupboxBegin("Config", 40, 1);
					switch (Combobox("Configs", configs, SETTINGS::settings.config_sel))
					{
					case 0: config = "default"; break;
					case 1: config = "legit"; break;
					case 2: config = "auto_hvh"; break;
					case 3: config = "scout_hvh"; break;
					case 4: config = "pistol_hvh"; break;
					}
					GroupboxEnd();

					GroupboxBegin("Save/Load", 65, 1);
					if (Button("Load"))
						SETTINGS::settings.Load(config);

					if (Button("Save"))
						SETTINGS::settings.Save(config);
					GroupboxEnd();

					/*GroupboxBegin("Debug", 40, 1);
					Checkbox("debug", SETTINGS::settings.debug_bool);

					//ColorPicker("menu theme", SETTINGS::settings.menu_col);
					GroupboxEnd();*/
				}
				break;
				}

				WindowEnd();

			}
		}


	}
}