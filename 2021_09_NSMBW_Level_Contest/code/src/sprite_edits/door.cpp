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
#include "extra_externals.h"


// Doors (sprs 259, 276) hacks


// If settings & 0x10000000, door is non-enterable
kmCallDefCpp(0x8002b308, bool, dStageActor_c *this_) {
    if ((this_->settings >> 28) & 1)
        return false;
    else
        return daEnDoor_c_checkOpenOk(this_);
};

// settings & 0x0f000000: z-order (higher values = farther behind)
void Door_SetZOrder(dStageActor_c *this_) {
    int zorder_int = (this_->settings >> 24) & 0xf;
    this_->pos.z = 32.0f - 100.0f * zorder_int;
};
kmBranchDefAsm(0x8002af10, 0x8002af14) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r31
    bl Door_SetZOrder
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
