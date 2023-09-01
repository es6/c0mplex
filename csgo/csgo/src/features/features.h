#pragma once

#include <stdexcept>
#include "../main/interfaces.h"

class CUserCmd;
namespace features {
	// Aim
	void trigger(float frameTime, CUserCmd* cmd) noexcept;

	// ESP
	void esp(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;

	// Chams
	void chams(void* results,
		const CDrawModelInfo& info,
		CMatrix3x4* bones,
		float* flexWeights,
		float* flexDelayedWeights,
		const CVector& modelOrigin,
		const std::int32_t flags,
		bool ignorezValue,
		const float* color) noexcept;

	// Misc
	void bhop(CUserCmd* c) noexcept;
	void radar() noexcept;
}