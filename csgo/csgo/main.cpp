#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdint>
#include <thread>

namespace offset {
	// client
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDEA98C;
	constexpr ::std::ptrdiff_t dwForceJump = 0x52BBCD8;

	// player
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_fFlags = 0x104;
}

void bhop(const HMODULE instance) noexcept {
	const auto client = (uintptr_t)(GetModuleHandle("client.dll"));

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {
		// complete Bhop functionality
	}

	// Uninject
	FreeLibraryAndExitThread(instance, 0);
}

// entry point
int __stdcall DllMain(const HMODULE instance, const std::uintptr_t reason, const void* reserved) {
	
	// DLL_PROCESS_ATTACH
	if (reason == 1) {
		DisableThreadLibraryCalls(instance);

		// create hack thread
		const auto thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)(bhop), instance, 0, nullptr);
		
		if (thread) CloseHandle(thread);
	}
	return 1;
}
