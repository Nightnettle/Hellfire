#include "PaintTraverse.h"

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

#include "../FEATURES/Visuals.h"

#include "../MENU/menu_framework.h"

typedef void(__thiscall* PaintTraverseFn)(void*, unsigned int, bool, bool);

PaintTraverseFn original_paint_traverse;

void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
{
	std::string panel_name = Interfaces::Panel->GetName(VGUIPanel);

	if (panel_name == "HudZoom")
		return;

	if (panel_name == "MatSystemTopPanel")
	{
		if (FONTS::ShouldReloadFonts())
			FONTS::InitFonts();

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		{
			if (SETTINGS::settings.esp_bool)
			{
				visuals->Draw();
				visuals->ClientDraw();
			}
		}

		MENU::PPGUI_PP_GUI::Begin();
		MENU::Do();
		MENU::PPGUI_PP_GUI::End();

		Utils::INPUT::input_handler.Update();

		RENDER::DrawSomething();
	}

	original_paint_traverse(Interfaces::Panel, VGUIPanel, ForceRepaint, AllowForce);
}