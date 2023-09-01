#include "features.h"

#include "../main/globals.h"
#include "../main/interfaces.h"
#include <Windows.h>

// Aim
void features::trigger(float frameTime, CUserCmd* cmd) noexcept {
	// make sure we are pressing our triggerbot key: mouse5
	if (GetAsyncKeyState(VK_XBUTTON2)) {
		// make sure our localplayer exists and is alive
		CVector eyePosition;
		globals::localPlayer->GetEyePosition(eyePosition);

		CVector aimPunch;
		globals::localPlayer->GetAimPunch(aimPunch);

		// calculate destination of ray
		const CVector dest = eyePosition + ((CVector{ cmd->viewAngles + aimPunch }).ToVector() * 10000.f);

		// trace the ray from eyes -> dest
		CTrace trace;
		interfaces::trace->TraceRay({ eyePosition, dest }, 0x46004009, globals::localPlayer, trace);

		// make sure player is alive & is an enemy
		if (trace.entity && trace.entity->IsPlayer()) {
			if (trace.entity->IsAlive() && trace.entity->GetTeam() != globals::localPlayer->GetTeam()) {
				// make our local player shoot
				cmd->buttons |= CUserCmd::IN_ATTACK;
			}
		}
	}
}

// ESP
void features::esp(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept {
	// loop through players
	for (int i = 1; i <= interfaces::globals->maxClients; ++i)
	{
		// get the player
		CEntity* player = interfaces::entityList->GetEntityFromIndex(i);

		// make sure player is valid, is not dormant, is alive, is not on same team
		if (!player || player->IsDormant() || !player->IsAlive() || player->GetTeam() == globals::localPlayer->GetTeam())
			continue;

		// dont do esp on who we are spectating
		if (!globals::localPlayer->IsAlive())
			if (globals::localPlayer->GetObserverTarget() == player)
				continue;

		// player's bone matrix
		CMatrix3x4 bones[128];
		if (!player->SetupBones(bones, 128, 0x7FF00, interfaces::globals->currentTime))
			continue;

		// screen position of head
		// we add 11.f here because we want the box ABOVE their head
		CVector top;
		if (interfaces::debugOverlay->ScreenPosition(bones[8].Origin() + CVector{ 0.f, 0.f, 11.f }, top))
			continue;

		// screen position of feet
		// we subtract 9.f here because we want the box BELOW their feet
		CVector bottom;
		if (interfaces::debugOverlay->ScreenPosition(player->GetAbsOrigin() - CVector{ 0.f, 0.f, 9.f }, bottom))
			continue;

		// the height of the box is the difference between
		// the bottom (larger number) and the top
		const float h = bottom.y - top.y;

		// we can use the height to determine a width
		const float w = h * 0.3f;

		const auto left = static_cast<int>(top.x - w);
		const auto right = static_cast<int>(top.x + w);

		// set the color to white
		interfaces::surface->DrawSetColor(255, 255, 255, 255);

		// draw the normal box
		interfaces::surface->DrawOutlinedRect(left, top.y, right, bottom.y);

		// set the color to black for outlines
		interfaces::surface->DrawSetColor(0, 0, 0, 255);

		// normal box outline
		interfaces::surface->DrawOutlinedRect(left - 1, top.y - 1, right + 1, bottom.y + 1);
		interfaces::surface->DrawOutlinedRect(left + 1, top.y + 1, right - 1, bottom.y - 1);

		// health bar outline (use the black color here)
		interfaces::surface->DrawOutlinedRect(left - 6, top.y - 1, left - 3, bottom.y + 1);

		// health is an integer from 0 to 100 so convert to percentage
		const float healthFrac = player->GetHealth() * 0.01f;

		// set the color of the health bar to a split between red / green
		interfaces::surface->DrawSetColor((1.f - healthFrac) * 255, 255 * healthFrac, 0, 255);

		// draw it
		interfaces::surface->DrawFilledRect(left - 5, bottom.y - (h * healthFrac), left - 4, bottom.y);
	}
}

// Chams
void features::chams(void* results,
	const CDrawModelInfo& info,
	CMatrix3x4* bones,
	float* flexWeights,
	float* flexDelayedWeights,
	const CVector& modelOrigin,
	const std::int32_t flags,
	bool ignorezValue,
	const float* color) noexcept {
	// gets material to override
	static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

	// modulate alpha
	interfaces::studioRender->SetAlphaModulation(1.f);

	// set show through walls + color based on input
	material->SetMaterialVarFlag(IMaterial::IGNOREZ, ignorezValue);
	interfaces::studioRender->SetColorModulation(color);
	interfaces::studioRender->ForcedMaterialOverride(material);
}

// Misc
void features::bhop(CUserCmd* cmd) noexcept {
	if (globals::localPlayer) {
		if (!(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND)) {
			cmd->buttons &= ~CUserCmd::IN_JUMP;
		}
	}
}

void features::radar() noexcept {
	for (auto i = 1; i <= 64; ++i) {
		const auto entity = interfaces::entityList->GetEntityFromIndex(i);

		if (!entity)
			continue;
		// check if alive, if enemy, if dormant
		if (!entity->IsAlive() || entity->GetTeam() == globals::localPlayer->GetTeam() || entity->IsDormant())
			continue;

		entity->Spotted() = true;
	}
}