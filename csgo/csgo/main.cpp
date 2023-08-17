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
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!GetAsyncKeyState(VK_SPACE)) continue;

		// Get LocalPlayer
		const auto localPlayer = *(uintptr_t*)(client + offset::dwLocalPlayer);
		if (!localPlayer) continue;

		const auto health = *(int32_t*)(localPlayer + offset::m_iHealth);

		// Check if alive
		if (!health) continue;

		const auto flags = *(std::int32_t*)(localPlayer + offset::m_fFlags);

		// Check if on ground
		if (flags && (1 << 0)) {
			*(std::uintptr_t*)(client + offset::dwForceJump) = 6; // forces jump if on ground
		} else {
			*(std::uintptr_t*)(client + offset::dwForceJump) = 4; // resets
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
