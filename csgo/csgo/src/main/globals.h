#pragma once

class CEntity;
namespace globals 
{
	// global local player pointer
	inline CEntity* localPlayer = nullptr;
	inline std::uintptr_t clientAddress = 0;

	inline bool bhop = false;


	// float arrays to hold cham colors
	// TODO: need to work on implementing menu customiation with color
	constexpr float hidden[3] = { 0.f, 1.f, 1.f };
	constexpr float visible[3] = { 1.f, 1.f, 0.f };
}

namespace offsets {

}