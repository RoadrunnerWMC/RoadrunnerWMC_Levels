// Copyright (C) 2021 RoadrunnerWMC

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <kamek.h>
#include <game.h>

#include "asm_macros.h"
#include "game_constants.h"


// P-switch (41) hacks


// This is so stupid

// settings & 0x000000f0: event ID that prevents it from spawning
bool PSwitch_ShouldSpawn(dStageActor_c *this_) {
    int spawnPreventingEventId = (this_->settings >> 4) & 0xf;

    if (spawnPreventingEventId && dFlagMgr_c::instance->active(spawnPreventingEventId - 1)) {
        return false;
    }

    return true;
};

kmBranchDefAsm(0x80a197b4, 0x80a197b8) {
    nofralloc  // don't auto-generate a prologue
    mfspr r0, lr      // (original instructions)
    stw r0, 0x24(sp)  // (^)

    kmSaveContext
    bl PSwitch_ShouldSpawn
    cmpwi r3, 1
    beq _ShouldSpawn
_ShouldntSpawn:
    kmRestoreContext
    // return 2 ("delete me pls")
    li r3, ONCREATE_RESULT_PLEASE_DELETE_ME
    // epilogue and blr
    lwz r0, 0x24(sp)
    mtspr lr, r0
    addi sp, sp, 0x20
    BACKDOOR_BLR
_ShouldSpawn:
    kmRestoreContext
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
