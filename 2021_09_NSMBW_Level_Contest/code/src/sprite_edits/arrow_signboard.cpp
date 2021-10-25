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


// Arrow Signboard (spr 310) hacks


void ArrowSignboard_SetRotation(dStageActor_c *this_) {
    // settings & 0xffff0000: rotation angle
    this_->rot.z = this_->settings >> 16;
};

kmBranchDefAsm(0x8081c950, 0x8081c954) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r31
    bl ArrowSignboard_SetRotation
    ASM_RESTORE_CONTEXT_FROM_STACK
    lwz r31, 0x1c(sp)  // (original instruction)
    blr  // needed because we used nofralloc at the start
};
