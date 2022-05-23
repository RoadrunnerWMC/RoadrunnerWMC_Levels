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


// New setting in nybble 5: initial rotation angle
kmSafeBranchDefCpp(0x8089ff60, stw r8, 0x3d0(r3)) {
    kmUseReg(r3, this_, dStageActor_c *);

    *((s16*)((u32)this_ + 0x448)) = (this_->settings >> 28) << 12;
};
