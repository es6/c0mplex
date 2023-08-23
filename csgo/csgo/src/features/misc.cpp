#include "misc.h"

#include "../main/globals.h"
#include "../main/interfaces.h"

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
		// check if alive, if enemy
		if (!entity->IsAlive() || entity->GetTeam() == globals::localPlayer->GetTeam())
			continue;

		entity->Spotted() = true;
	}
}