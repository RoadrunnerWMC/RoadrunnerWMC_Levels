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


// Edit the fourth zoom level in the "normal" camera mode zoom-levels list to
// be 24 tiles tall instead of 20.
// This is needed because:
// - the horizontal camera limiter (248/249) doesn't work properly at lower zoom levels, and
// - 20 is the maximum size normally available in the "normal" mode zoom-levels list, and
// - the "X Tracking Only" mode (which we need to prevent Y scrolling) uses that list, and
// - I wanted a 24-tile-tall camera there anyway for aesthetics
kmWrite32(0x802ef964, 0x3fdb6db7);  // float: 24/14 (originally 20/14)


// Prevent initial zoom-out
/* ---- Not used anymore
kmWrite32(0x80155d40, 0x28000000);  // cmplwi r0, 0x0
*/
