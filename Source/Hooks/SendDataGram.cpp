#include "SendDataGram.h"

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

typedef int(__thiscall* SendDatagramFn)(void*, void*);

SendDatagramFn original_send_datagram;



void __fastcall HookedSendDatagram(void* ecx, void* data)
{
	original_send_datagram(ecx, data);
}