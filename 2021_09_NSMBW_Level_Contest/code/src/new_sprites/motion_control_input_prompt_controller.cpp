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
#include <dCourse.h>

#include "extra_externals.h"
#include "game_constants.h"
#include "new_sprites/ids.h"
#include "new_sprites/macros.h"


// NOTE: "id" is a unique class identifier, allowing the fukidashi manager to intelligently handle conflicts
// when different classes both want to display fukidashi prompts

// ID | Used by                                 | Animation
// =========================================================================
// 0  | daLiftRemoconSeesaw_c (sprites 51, 190) | Tilt
// 1  | dAcPy_c (for propeller suit) [1]        | (just blinks like crazy?)
// 2  | daJrClownForPlayer_c                    | Shake
// 3  | daPropBlock_c (sprite 393) [1]          | "(1) + [shake]"
// 4  | daLightBlock_c (sprite 391)             | "(1) + [shake]"
// 5  | daPowBlock_c (sprite 386)               | "(1) + [shake]"
// 6  | daBlockTaru_c (sprite 388)              | "(1) + [shake]"
// 7  | daSlideBlock_c (sprite 385)             | "(1) + [shake]"
// 8  | dAcPy_c (for propeller suit) [2]        | (nothing at all?)
// 9  | daLiftSpinRotation_c (sprites 172, 382) | Shake
// 10 | OBJ_SPIN_PARENT (sprite 315) (bolt)     | Shake
// 11 | dActorMng_c (huh??)                     | Tilt
// 12 | daRemoconBalance_c (sprite 56)          | Tilt
// 13 | daLiftRemoconXline_c (sprite 59)        | Tilt
// 14 | daWireRemocon_c (sprite 267)            | Tilt
// 15 | daEnRemoconCannon_c (sprite 202)        | Tilt
// 16 | daLineSpinLift_c (sprite 327)           | Shake
// 17 | daIce_c                                 | "(1) + [shake]"
// 18 | daEnJumpdai_c (sprite 148)              | "(1) + [shake]"
// 19 | daPropBlock_c (sprite 393) [2]          | Shake
// 20 | daEnHatenaBalloon_c (sprite 171)        | Shake
// 21 | daNiceBoat_c (sprite 368)               | Tilt
// (21 is the highest valid ID, according to multiple pieces of evidence)
// TODO: one of these is Rescuable Toad but I'm not sure which one

// Barring the possibility of adding new IDs, the safest way to use fukidashi is to pick the ID of a class
// that you're pretty sure won't be in the same level as you, and which has the animation you want.

// Show fukidashi
// 800b3600: void hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi(int player, int id)
// Permanently end fukidashi -- will not be shown again for this player + ID for the rest of the level.
// Use this when the player has demonstrated they definitely know how to use the motion mechanic.
// For example, the tilt platforms in 1-2 call this when the player has tilted the platform to a maximum
// angle eight times.
// 800b3720: void hashname_5edaa1df_bf88549a_gameCom_setFukidashi234(int player, int id, int param3)
// Temporarily end fukidashi; it can be re-shown again later.
// For example, the tilt platforms in 1-2 call this when the player leaves the platform.
// 800b3750: void hashname_9723880b_dea8588e_gameCom_setFukidashiAll234(int player, int id, int param3)

// Main thing still yet undocumented is how exactly "param3" works.
// Then I can properly design my fukidashi sprite.
// Sadly, it's just a value that gets stuffed into one of any number of opaque fields...

// 80157450 checks a param for vals 8 and < 0x15... Checking xrefs, that's clearly an "id" param, though.
// Though that does mean 21 is the highest. ...though it has extra logic for >= 0x15, so ...???


class daMotionControlPromptMng_c : public dStageActor_c {
public:

    u8 typeId;  // second param to the fukidashi functions
    u8 cancelParam3;
    u8 players;
    bool isSingleUse;

    bool isActive;

    static daMotionControlPromptMng_c *build();

    int onCreate();
    int onExecute();
};

daMotionControlPromptMng_c *daMotionControlPromptMng_c::build() {
    return new(AllocFromGameHeap1(sizeof(daMotionControlPromptMng_c))) daMotionControlPromptMng_c;
};

int daMotionControlPromptMng_c::onCreate() {
    typeId = settings & 0xff;
    cancelParam3 = (settings >> 8) & 0xff;
    players = (settings >> 16) & 0xf;
    isSingleUse = (settings >> 20) & 1;
    isActive = false;

    return ONCREATE_RESULT_SUCCESS;
};

int daMotionControlPromptMng_c::onExecute() {
    int eventId = spriteFlagNum & 0xff;  // (nybbles 3-4)

    bool nowActive = dFlagMgr_c::instance->active(eventId - 1);

    for (int player = 0; player < 3; player++) {
        if (!((players >> player) & 1))
            continue;

        if (!isActive && nowActive) {
            // Just activated
            hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi(player, typeId);
        } else if (isActive && !nowActive) {
            // Just deactivated
            if (isSingleUse)
                hashname_5edaa1df_bf88549a_gameCom_setFukidashi234(player, typeId, cancelParam3);
            else
                hashname_9723880b_dea8588e_gameCom_setFukidashiAll234(player, typeId, cancelParam3);
        }
    }

    isActive = nowActive;

    return 1;
};


OVERWRITE_PROFILE(MOTION_CONTROL_PROMPT_MNG, MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM,
    &daMotionControlPromptMng_c::build,
    MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM, MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM, 0);

OVERWRITE_SPRITE_DATA(MOTION_CONTROL_PROMPT_MNG_SPRITE_NUM, MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM,
    0, 0,        // spawn offset: x/y
    0, 0, 0, 0,  // spawn rect: x1/y1/x2/y2
    SPRITE_FLAGS_NULL);

OVERWRITE_SPRITE_RESOURCES_NULL(MOTION_CONTROL_PROMPT_MNG_SPRITE_NUM);
