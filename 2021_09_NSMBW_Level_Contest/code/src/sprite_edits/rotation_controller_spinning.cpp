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


// Rotation controller (spinning) (spr 149) hacks


// This is the worst workaround

bool RotationController_IsActive(dStageActor_c *this_) {
    int disablingEventId = this_->currentLayerID;  // nybbles 15-16

    if (disablingEventId && dFlagMgr_c::instance->active(disablingEventId - 1)) {
        *(u16*)((u32)this_ + 0x104) = 0xC000;  // (this is actually 0, trust me)
        // force rotation to 0
        return false;
    }
    return true;
};

kmBranchDefAsm(0x80874968, 0x8087496c) {
    nofralloc  // don't auto-generate a prologue
    kmSaveContext
    mr r3, r31
    bl RotationController_IsActive
    cmpwi r3, 1
    bne _IsntActive
_IsActive:
    kmRestoreContext
    bctrl  // (original instruction)
    b _End
_IsntActive:
    kmRestoreContext
_End:
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
