#pragma once
#include "..\SDK\CUserCmd.h"

typedef int(__thiscall* SendDatagramFn)(void*, void*);

extern SendDatagramFn original_send_datagram;

void __fastcall HookedSendDatagram(void* ecx, void* data);