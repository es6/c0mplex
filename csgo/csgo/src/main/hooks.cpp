#include "hooks.h"

#include <stdexcept>
#include <intrin.h>

#include "../../dependencies/minhook/minhook.h"

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../features/features.h"

void hooks::Setup() {

	// If MinHook intializes successfully, it returns 0, else it was unsuccessful
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	if (MH_CreateHook(VirtualFunction(gui::device, 42), &EndScene, (void**)(&EndSceneOriginal)))
		throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(VirtualFunction(gui::device, 16), &Reset, (void**)(&ResetOriginal)))
		throw std::runtime_error("Unable to hook Reset()");

	if (MH_CreateHook(memory::Get(interfaces::keyValuesSystem, 1), &AllocKeyValuesMemory, reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)))
		throw std::runtime_error("Unable to hook AllocKeyValuesMemory()");

	if (MH_CreateHook(memory::Get(interfaces::clientMode, 24), &CreateMove, reinterpret_cast<void**>(&CreateMoveOriginal)))
		throw std::runtime_error("Unable to hook CreateMove()");

	if (MH_CreateHook(memory::Get(interfaces::studioRender, 29), &DrawModel, reinterpret_cast<void**>(&DrawModelOriginal)))
		throw std::runtime_error("Unable to hook DrawModel()");

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

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
	// if function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesEngine) ||
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesClient))
		return nullptr;

	// return original
	return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd) noexcept {
	if (cmd->commandNumber == 0)
		return false;

	// dont run logic if not called by CInput::CreateMove
	if (!cmd->commandNumber)
		return CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	// set view angles if return value is true to avoid stuttering
	if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
		interfaces::engine->SetViewAngles(cmd->viewAngles);

	// get local player
	globals::UpdateLocalPlayer();

	if (globals::localPlayer && globals::localPlayer->IsAlive()) {
		// Run hacks
		features::trigger(frameTime, cmd);
		features::bhop(cmd);
		features::radar();
	}

	return false;
}

void __stdcall hooks::DrawModel(
	void* results,
	const CDrawModelInfo& info,
	CMatrix3x4* bones,
	float* flexWeights,
	float* flexDelayedWeights,
	const CVector& modelOrigin,
	const std::int32_t flags
) noexcept {
	// make sure local player and renderable pointer != nullptr or else crashes
	if (globals::localPlayer && info.renderable) {
		// get base entity poitner from IClientUnknown
		CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

		// make sure entity is a valid enemy player
		if (entity && entity->IsPlayer() && entity->GetTeam() != globals::localPlayer->GetTeam()) {
			// gets material to override
			static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

			// modulate alpha
			interfaces::studioRender->SetAlphaModulation(1.f);

			// show through walls
			material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(globals::hidden);
			interfaces::studioRender->ForcedMaterialOverride(material);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			// show only visible
			material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
			interfaces::studioRender->SetColorModulation(globals::visible);
			interfaces::studioRender->ForcedMaterialOverride(material);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			// reset material override and return from hook
			return interfaces::studioRender->ForcedMaterialOverride(nullptr); 
		}
	}
	DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}