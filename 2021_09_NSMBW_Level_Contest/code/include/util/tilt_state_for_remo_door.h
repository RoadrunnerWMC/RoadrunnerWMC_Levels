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

#ifndef UTIL_TILT_STATE_FOR_REMO_DOOR_H
#define UTIL_TILT_STATE_FOR_REMO_DOOR_H


// You can use < and > to compare these from leftmost to rightmost
#define TILT_STATE_LEFT -2
#define TILT_STATE_NEUTRAL_LEFT -1
#define TILT_STATE_NEUTRAL 0
#define TILT_STATE_NEUTRAL_RIGHT 1
#define TILT_STATE_RIGHT 2

int GetTiltStateForRemoDoor();


#endif  // UTIL_TILT_STATE_FOR_REMO_DOOR_H
