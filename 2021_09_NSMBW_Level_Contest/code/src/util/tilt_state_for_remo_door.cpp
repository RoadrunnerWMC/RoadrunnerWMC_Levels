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


#include <game.h>

#include "extra_externals.h"
#include "util/tilt_state_for_remo_door.h"


// Custom function for checking wiimote tilt state


int GetTiltStateForRemoDoor() {
    if (SomethingPlayerRelated_80429f84 >= 4)
        return TILT_STATE_NEUTRAL;

    s16 sta = shiftedTiltAmount__7RemoconFv(GetRemoconMng()->controllers[SomethingPlayerRelated_80429f84]) >> 16;

    if (sta <= -0x2000)
        return TILT_STATE_LEFT;
    else if (sta <= -0x1000)
        return TILT_STATE_NEUTRAL_LEFT;
    else if (sta < 0x1000)
        return TILT_STATE_NEUTRAL;
    else if (sta < 0x2000)
        return TILT_STATE_NEUTRAL_RIGHT;
    else
        return TILT_STATE_RIGHT;
};
