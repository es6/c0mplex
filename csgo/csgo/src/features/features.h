#pragma once

class CUserCmd;
namespace features {
	// Aim
	void trigger(float frameTime, CUserCmd* cmd) noexcept;


	// Misc
	void bhop(CUserCmd* c) noexcept;
	void radar() noexcept;
}