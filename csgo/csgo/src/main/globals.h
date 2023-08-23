#pragma once

class CEntity;
namespace globals 
{
	// global local player pointer
	inline CEntity* localPlayer = nullptr;


	// float arrays to hold cham colors
	// TODO: need to work on implementing menu customiation with color
	constexpr float hidden[3] = { 0.f, 1.f, 1.f };
	constexpr float visible[3] = { 1.f, 1.f, 0.f };

	// update the local player pointer
	void UpdateLocalPlayer() noexcept;
}