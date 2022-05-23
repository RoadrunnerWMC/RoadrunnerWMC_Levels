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


// New setting in nybbles 5-8: initial angle
kmSafeBranchDefCpp(0x809e5828, lwz r29, 0x44(sp)) {
    kmUseReg(r28, this_, dStageActor_c *);

    s16 initialAngle = (this_->settings >> 16) & 0xffff;
    this_->rot.z += initialAngle;
};
