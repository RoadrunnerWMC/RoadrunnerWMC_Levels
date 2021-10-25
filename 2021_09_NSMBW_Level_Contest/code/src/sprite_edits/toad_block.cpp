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
#include <sfx.h>

#include "asm_macros.h"
#include "extra_externals.h"
#include "util/play_sound.h"


// Toad block (spr 422) hacks


// If settings & 0x20000000, block will always contain Toad no matter what
kmBranchDefAsm(0x809c3030, 0x809c3034) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lwz r3, 0x4(r30)
    rlwinm. r3, r3, 3, 31, 31  // r3 = (r3 <<< 3) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Just return true)
    li r3, 1
    b _End
_OptionNotEnabled:
    // (Original code)
    bl FUN_P1_800fd140
_End:
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};


void ToadBlock_DisappearWithBooLaugh(dStageActor_c *this_) {
    PlaySound(this_, SE_EMY_CS_TERESA_BEAT_YOU);
    S16Vec nullRot = {0, 0, 0};
    Vec oneVec = {1.0f, 1.0f, 1.0f};
    SpawnEffect("Wm_en_obakedoor", 0, &this_->pos, &nullRot, &oneVec);

    // event_nybbles & 0xff: optional event ID to trigger when the fake block is opened
    int event_id = this_->spriteFlagNum & 0xff;
    if (event_id) {
        dFlagMgr_c::instance->set(event_id - 1, 0, true, false, false);
    }

    this_->Delete(1);
};

// If settings & 0x10000000, instead of giving Toad, make the block disappear with boo laugh
kmCallDefAsm(0x809c39e8) {
    // Check spritedata
    lwz r0, 0x4(r31)
    rlwinm. r0, r0, 4, 31, 31  // r0 = (r0 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    b ToadBlock_DisappearWithBooLaugh
_OptionNotEnabled:
    // (Original code)
    b player_set_15daEnBlockMain_cFii
};
