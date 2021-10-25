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


// LIFT_ROTATION_FULL_COLOR (spr 362) hacks


// If spritedata & 0x20000000, remove draw() function
kmBranchDefAsm(0x8089a600, 0x8089a604) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lwz r0, 0x4(r3)
    rlwinm. r0, r0, 3, 31, 31  // r0 = (r0 <<< 3) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    BACKDOOR_BLR
_OptionNotEnabled:
    // (Original instruction)
    stwu sp, -0x10(sp)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// If spritedata & 0x10000000, don't add 3 to width/height nybbles
// For visuals:
kmBranchDefAsm(0x8089a6b8, 0x8089a6c0) {
    // Check spritedata
    lwz r4, 0x4(r28)
    rlwinm. r4, r4, 4, 31, 31  // r4 = (r4 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Don't add 3 when moving r29/r30 to r4/r5)
    mr r4, r29
    mr r5, r30
    b _End
_OptionNotEnabled:
    // (Original code)
    addi r4, r29, 0x3
    addi r5, r30, 0x3
_End:
};
// For collision:
kmBranchDefAsm(0x8089a6cc, 0x8089a6d8) {
    // Check spritedata
    lwz r4, 0x4(r28)
    rlwinm. r4, r4, 4, 31, 31  // r4 = (r4 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Don't add 3 when moving r29/r30 to r4/r0)
    mr r4, r29
    mr r0, r30
    b _End
_OptionNotEnabled:
    // (Original code)
    addi r4, r29, 0x3
    addi r0, r30, 0x3
_End:
    // (Was originally between the two addi instructions)
    stw  r4, 0x14(sp)
};
