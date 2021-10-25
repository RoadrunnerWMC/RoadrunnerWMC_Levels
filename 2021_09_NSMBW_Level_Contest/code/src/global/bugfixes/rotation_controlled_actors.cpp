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


// (Copypasted from NSMBW-Updated: https://github.com/RoadrunnerWMC/NSMBW-Updated)


// daEnLiftRotHalf_c (EN_LIFT_ROTATION_HALF: profile 481, sprite 107)
kmWrite32(0x80a5d980, 0x38a0c000);  // li r5, -0x4000

// daEnObjRotationBlock_c (EN_ROTATION_BLOCK: profile 529, sprite 252)
kmWrite32(0x80a7b558, 0x3860c000);  // li r3, -0x4000
kmWrite32(0x80a7b570, 0x3860c000);  // li r3, -0x4000

// daEnCoinAngle_c (EN_COIN_ANGLE: profile 530, sprite 253)
kmWrite32(0x809e55f4, 0x3860c000);  // li r3, -0x4000
kmWrite32(0x809e560c, 0x3860c000);  // li r3, -0x4000

// daEnBlockAngle_c (EN_BLOCK_HATENA_ANGLE: profile 532, sprite 255 & EN_BLOCK_RENGA_ANGLE: profile 533, sprite 256)
kmWrite32(0x809c15c4, 0x3860c000);  // li r3, -0x4000
kmWrite32(0x809c15dc, 0x3860c000);  // li r3, -0x4000
