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
#include "game_constants.h"


// Rolling Hill (spr 212) hacks


// If event_nybbles & 0x0001, remove draw() function
kmBranchDefAsm(0x807ef060, 0x807ef064) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lhz r0, 0x354(r3)
    andi. r0, r0, 0x1
    beq _OptionNotEnabled
_OptionEnabled:
    BACKDOOR_BLR
_OptionNotEnabled:
    // (Original instruction)
    stwu sp, -0x40(sp)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// If event_nybbles & 0x0002, remove collision
// (onCreate: start of function, just after the prologue)
kmBranchDefAsm(0x807eecfc, 0x807eed00) {
    nofralloc  // don't auto-generate a prologue
    // Check if this is the first or second time running onCreate
    lhz r0, 0x356(r3)
    cmpwi r0, 1
    bne _FirstTimeRunning

_SecondTimeRunning:
    // Check spritedata
    lhz r0, 0x354(r3)
    andi. r0, r0, 0x2
    bne _OptionEnabled
_OptionNotEnabled:
    // Call the collider-add function
    addi r3, r3, 0x3e4
    bl addToList__13BasicColliderFv
_OptionEnabled:
    // Epilogue and return 1
    lwz r0, 0x34(sp)
    mtspr lr, r0
    addi sp, sp, 0x30
    li r3, ONCREATE_RESULT_SUCCESS
    BACKDOOR_BLR

_FirstTimeRunning:
    // Write a "1" to a pad area so we know we ran once
    li r0, 1
    sth r0, 0x356(r3)
    // (Original instruction)
    stw r31, 0x2c(sp)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
// (onCreate: original call to addToList__13BasicColliderFv)
kmWrite32(0x807eeeac, 0x60000000);  // nop
// (onCreate: original li r3, 1 before the blr)
kmWrite32(0x807eef04, 0x38600000);  // li r3, 0 (ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME)
// (onDelete)
kmBranchDefAsm(0x807ef1d8, 0x807ef1dc) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lhz r0, 0x354(r31)
    andi. r0, r0, 0x2
    bne _OptionEnabled
_OptionNotEnabled:
    // (Original instruction)
    bl removeFromList__13BasicColliderFv
_OptionEnabled:
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
