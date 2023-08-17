#include "hooks.h"

#include <stdexcept>
#include <intrin.h>

#include "../dependencies/minhook/minhook.h"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "../dependencies/imgui/imgui_impl_dx9.h"

void hooks::Setup() {

	// If MinHook intializes successfully, it returns 0, else it was unsuccessful
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	if (MH_CreateHook(VirtualFunction(gui::device, 42), &EndScene, (void**)(&EndSceneOriginal)))
		throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(VirtualFunction(gui::device, 16), &Reset, (void**)(&ResetOriginal)))
		throw std::runtime_error("Unable to hook Reset()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept {
	// We declare as static so returnAddress only calls once and store the first return address of EndScene
	static const auto returnAddress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	// If the current return address is equal to the one we saved
	// We return out with our result 
	if (_ReturnAddress() == returnAddress)
		return result;

	if (!gui::setup)
		gui::SetupMenu(device);

	if (gui::open)
		gui::Render();

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}
