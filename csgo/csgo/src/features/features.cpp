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
				cmd->buttons |= ~CUserCmd::IN_ATTACK;
			}
		}
	}
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
	for (auto i = 1; i <= 64, ++i;) {
		const auto entity = interfaces::entityList->GetEntityFromIndex(i);

		if (!entity)
			continue;
		// check if alive, if enemy, if dormant
		if (!entity->IsAlive() || entity->GetTeam() == globals::localPlayer->GetTeam() || entity->IsDormant())
			continue;

		entity->Spotted() = true;
	}
}