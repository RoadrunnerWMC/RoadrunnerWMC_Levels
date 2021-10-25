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


// Boo circle (spr 323) hacks


void BooCircle_SetInitialRotationAngle(dStageActor_c *this_) {
    // settings & 0xf0000000: initial rotation angle
    *((s16*)((u32)this_ + 0x448)) = (this_->settings >> 28) << 12;
};

kmCallDefAsm(0x8089ff60) {
    nofralloc  // don't auto-generate a prologue
    stw r8, 0x3d0(r3)  // (original instruction)
    ASM_DUMP_CONTEXT_TO_STACK
    bl BooCircle_SetInitialRotationAngle
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start
};
