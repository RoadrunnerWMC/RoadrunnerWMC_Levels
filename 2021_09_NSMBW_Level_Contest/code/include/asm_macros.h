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

#ifndef ASM_MACROS_H
#define ASM_MACROS_H


// Kamek *HELPFULLY* errors out ("Function contains a return partway through")
// if there's a blr within a kmBranchDefAsm(x, y) hook.
// So I substitute a useless instruction that codewarrior would never generate
// naturally for blr, and the build script later does find-replace on it in the
// output kamekfiles.
#define BACKDOOR_BLR or r31, r31, r31  // 0x7ffffb78


#endif  // ASM_MACROS_H
