#pragma once
#include "gui.h"
#include "interfaces.h"

namespace hooks {
	void Setup();
	void Destroy() noexcept;

	// CreateMove
	using CreateMoveFunction = bool(__thiscall*)(void*, float, CUserCmd*) noexcept;
	inline CreateMoveFunction CreateMoveOriginal = nullptr;
	bool __stdcall CreateMove(float frameTime, CUserCmd* cmd) noexcept;

	constexpr void* VirtualFunction(void* thisptr, size_t index) noexcept {
		return (*static_cast<void***>(thisptr))[index];
	}

	// Bypass Return Address Checking
	using AllocKeyValuesMemoryFn = void* (__thiscall*)(void*, const std::int32_t) noexcept;
	inline AllocKeyValuesMemoryFn AllocKeyValuesMemoryOriginal;
	void* __stdcall AllocKeyValuesMemory(const std::int32_t size) noexcept;

	// ImGui
	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	// DrawModel
	using DrawModelFn = void(__thiscall*)(
		void*,
		void*,
		const CDrawModelInfo&,
		CMatrix3x4*,
		float*,
		float*,
		const CVector&,
		const std::int32_t
		) noexcept;

	// original function pointer
	inline DrawModelFn DrawModelOriginal = nullptr;
	
	// hooked function definition
	void __stdcall DrawModel(
		void* results,
		const CDrawModelInfo& info,
		CMatrix3x4* bones,
		float* flexWeights,
		float* flexDelayedWeights,
		const CVector& modelOrigin,
		const std::int32_t flags
	) noexcept;

	// PaintTraverse
	using PaintTraverseFn = void(__thiscall*)(IVPanel*, std::uintptr_t, bool, bool) noexcept;
	inline PaintTraverseFn PaintTraverseOriginal = nullptr;
	void __stdcall PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;
}