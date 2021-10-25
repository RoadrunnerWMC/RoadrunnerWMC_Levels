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


// Rotation-controlled coin (253) hacks


void RotationControlledCoin_ApplyDisplayAngleChange(dStageActor_c *this_) {
    s16 initialAngle = (this_->settings >> 16) & 0xffff;
    this_->rot.z += initialAngle;
};


kmBranchDefAsm(0x809e5828, 0x809e582c) {
    nofralloc  // don't auto-generate a prologue
    lwz r29, 0x44(sp)  // (original instruction)
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r28
    bl RotationControlledCoin_ApplyDisplayAngleChange
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
