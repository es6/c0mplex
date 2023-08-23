#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <thread>
#include "../../dependencies/minhook/minhook.h"
#include "interfaces.h"
#include "hooks.h"
#include "usercmd.h"
#include "interfaces.h"

using CreateMove = bool(__thiscall*)(void*, float, UserCmd*);
static CreateMove CreateMoveOriginal = nullptr;

constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA98C;
constexpr ::std::ptrdiff_t m_fFlags = 0x104;

bool __stdcall CreateMove_Bhop(float frameTime, UserCmd* cmd) {
	const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	if (cmd->commandNumber == 0) {
		return false;
	}

	if (!cmd || !cmd->commandNumber)
		return result;

	static unsigned int client = reinterpret_cast<unsigned int>(GetModuleHandle("client.dll"));

	const unsigned int localPlayer = *reinterpret_cast<unsigned int*>(client + dwLocalPlayer);

	if (localPlayer) {
		if (!(*reinterpret_cast<int*>(localPlayer + m_fFlags) & 1)) {
			cmd->buttons &= ~IN_JUMP;
		}
	}

	return false;
}

/*
	// We first initialize MinHook, create a hook to CreateMove in CSGO - complete our CreateMoveHook function - then repass in the original CreateMove to return code flow
	Setup2(LPVOID instance) is an example thread function to initiate our hook to CreateMove and execute 

DWORD WINAPI Setup2(LPVOID instance) {
	g_Client = GetInterface<void>("VClient018", "client.dll");
	g_ClientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(g_Client))[10] + 5);

	MH_Initialize();
	MH_CreateHook((*static_cast<void***>(g_ClientMode))[24], &CreateMove_Bhop, reinterpret_cast<void**>(&CreateMoveOriginal));
	MH_EnableHook(MH_ALL_HOOKS);

	while (!GetAsyncKeyState(VK_END)) {
		Sleep(200);
	}
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}
*/