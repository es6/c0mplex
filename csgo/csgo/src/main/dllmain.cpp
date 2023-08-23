#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <cstdint>
#include "hooks.h"

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


DWORD WINAPI Setup(LPVOID instance) {
	// Initialize our cheat
	memory::Setup();		// finds sig
	interfaces::Setup();	// gets interfaces
	netvars::Setup();		// dumps offsets
	gui::Setup();			// launches menu
	hooks::Setup();			// places hooks

	while (!GetAsyncKeyState(VK_END)) {
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	hooks::Destroy();
	gui::Destroy();

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		const HANDLE thread = CreateThread(nullptr, 0, Setup, instance, 0, nullptr);

		if (thread)
			CloseHandle(thread);

	}

	return TRUE;
}