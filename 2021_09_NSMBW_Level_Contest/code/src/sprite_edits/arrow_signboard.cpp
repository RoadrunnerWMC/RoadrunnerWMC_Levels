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


// New setting in nybbles 5-8: rotation angle
kmSafeBranchDefCpp(0x8081c950, lwz r31, 0x1c(sp)) {
    kmUseReg(r31, this_, dStageActor_c *);

    this_->rot.z = this_->settings >> 16;
};
