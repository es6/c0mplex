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

		const HANDLE thread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)Setup, instance, NULL, nullptr);
		if (thread)
			CloseHandle(thread);

	}

	return TRUE;
}