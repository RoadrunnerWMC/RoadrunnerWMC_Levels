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


// C++ version of PlaySound() from Newer


extern "C" void PlaySound(dActor_c *this_, int id) {
    // Actor_GetVec3WithValuesAdded adds pos_delta which is dumb and bad, so, skipping that
    Vec vec1 = {this_->pos.x, this_->pos.y, this_->pos.z};

    Vec vec2;
    ConvertStagePositionIntoScreenPosition__Maybe(&vec1, &vec2);

    if (!CheckIfPlayingSound(BufferToCheckIfPlaying, id))
        YetAnotherSoundPlayer(SoundClassRelated, id, &vec2, 0);
};


// Decompiled from daRemoDoor_c::initializeState_Open(void). Seems to work better than the other one.

extern "C" void PlaySound_Better(dStageActor_c *this_, int id) {
    Vec3 temp1 = {0.0f, 0.0f, 0.0f};
    ConvertStagePositionToScreenPosition((Vec2*)(&temp1), &this_->pos);
    SoundPlayingClass_PlaySoundAtPosition(SoundPlayingClass::instance2, id, &temp1, 0);
};
