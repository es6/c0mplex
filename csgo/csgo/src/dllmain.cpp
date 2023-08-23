#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <cstdint>
#include "interfaces.h"
#include "hooks.h"
#include "usercmd.h"
#include "../dependencies/minhook/minhook.h"

/*
* Breakdown of Execution
* 
* We first inject our DLL (calls DllMain), then check if the reason is DLL_PROCESS_ATTACH
* We then disable thread library calls because we aren't interested in thread notifications
* 
* Then we create a thread which runs Setup(const HMODULE instance) and if thread creation is successful
* We then close our handle
* 
* 
* If everything is fine, Setup(...) will be run in a separate thread which the function will try to setup gui
*	- this attempts to create a temp window, getting the device, and saving the device.
* 
* We use this device created from our gui::Setup to then hook the 2 virtual functions: EndScene and Reset
*	- if everything works, we sit in the while loop until we close our menu via pressing DELETE
*	- if DELETE is pressed, we go to UNLOAD which destroys our hooks and gui then frees the library and exits the thread
*/

template <typename T>
T* GetInterface(const char* name, const char* library) {
	const auto handle = GetModuleHandle(library);

	if (!handle)
		return nullptr;

	const auto functionAddress = GetProcAddress(handle, "CreateInterface");

	if (!functionAddress)
		return nullptr;

	/*
	Gets the address of CreateInterface and cast it to our own "Fn" function pointer

	*/

	using Fn = T * (*)(const char*, int*);
	const auto CreateInterface = (Fn)functionAddress;

	return CreateInterface(name, nullptr);
}

using CreateMove = bool(__thiscall*)(void*, float, UserCmd*);
static CreateMove CreateMoveOriginal = nullptr;

constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA98C;
constexpr ::std::ptrdiff_t m_fFlags = 0x104;

bool __stdcall CreateMoveHook(float frameTime, UserCmd* cmd) {
	const bool result = CreateMoveOriginal(g_ClientMode, frameTime, cmd);

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
	We first initialize MinHook, create a hook to CreateMove in CSGO - complete our CreateMoveHook function - then repass in the original CreateMove to return code flow
*/

DWORD WINAPI Setup2(LPVOID instance) {
	g_Client = GetInterface<void>("VClient018", "client.dll");
	g_ClientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(g_Client))[10] + 5);

	MH_Initialize();
	MH_CreateHook((*static_cast<void***>(g_ClientMode))[24], &CreateMoveHook, reinterpret_cast<void**>(&CreateMoveOriginal));
	MH_EnableHook(MH_ALL_HOOKS);

	while (!GetAsyncKeyState(VK_END)) {
		Sleep(200);
	}
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}


void Setup(const HMODULE instance) {
	try {
		// Initialize our cheat
		gui::Setup();
		hooks::Setup(); 
	}
	catch (const std::exception& error) {
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "c0mplex - ERROR", MB_OK | MB_ICONEXCLAMATION);

		goto UNLOAD;
	} 

	while (!GetAsyncKeyState(VK_DELETE)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

UNLOAD:
	hooks::Destroy();
	gui::Destroy();

	FreeLibraryAndExitThread(instance, 0);
}

BOOL WINAPI DllMain(const HMODULE instance, const std::uintptr_t reason, const void* reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		const HANDLE thread = CreateThread(nullptr, NULL, Setup2, instance, NULL, nullptr);
		if (thread)
			CloseHandle(thread);

	}

	return TRUE;
}