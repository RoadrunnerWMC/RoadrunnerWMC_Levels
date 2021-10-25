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
#include <sfx.h>

#include "extra_externals.h"
#include "game_constants.h"
#include "new_sprites/ids.h"
#include "new_sprites/macros.h"
#include "util/play_sound.h"
#include "util/tilt_state_for_remo_door.h"


// Retail daRemoDoor_c uses 0x400
// This is as high as I was able to make it before Mario starts clipping into it when it moves
#define DOOR_FLIP_SPEED 0x500

#define DIRECTION_NONE 0
#define DIRECTION_U 1
#define DIRECTION_R 2
#define DIRECTION_D 3
#define DIRECTION_L 4


class daHyperRemoDoor_c : public dActorState_c {
public:
    daHyperRemoDoor_c() {};

    mHeapAllocator_c allocator;
    nw4r::g3d::ResFile resFile;
    m3d::mdl_c model;

    Physics physics;

    u8 facingForwardAtCurrentPos;
    u8 primaryDirection;
    u8 oppositeDirection;
    u8 currentDirection;  // DIRECTION_* constants
    u8 stepCounter;
    u8 stepCounterMax;

    USING_STATES(daHyperRemoDoor_c);
    DECLARE_STATE(Ready);
    DECLARE_STATE(Moving);

    static daHyperRemoDoor_c *build(void);

    virtual int onCreate(void);
    void createMdl(int direction);
    virtual int onExecute(void);
    virtual int onDraw(void);

    void overwritePosDeltaForPhysCallback(void);
    bool amIClosestToMario(void);
};

daHyperRemoDoor_c *daHyperRemoDoor_c::build(void) {
    return new(AllocFromGameHeap1(sizeof(daHyperRemoDoor_c))) daHyperRemoDoor_c;
};

void daHyperRemoDoor_c::overwritePosDeltaForPhysCallback(void) {
    // This is kind of a hack.
    // The sprite's position changes abruptly between steps, not smoothly, but this sort of
    // prevents HurtMarioBecauseOfBeingSquashed from determining which way we're moving when
    // it's trying to decide whether Mario should get squished or not.
    // We solve that by manually setting pos_delta to a magnitude-1 vector here.

    if (currentDirection == DIRECTION_U) {
        pos_delta.x = 0.0f;
        pos_delta.y = 1.0f;
    } else if (currentDirection == DIRECTION_R) {
        pos_delta.x = 1.0f;
        pos_delta.y = 0.0f;
    } else if (currentDirection == DIRECTION_D) {
        pos_delta.x = 0.0f;
        pos_delta.y = -1.0f;
    } else if (currentDirection == DIRECTION_L) {
        pos_delta.x = -1.0f;
        pos_delta.y = 0.0f;
    } else {  // none
        pos_delta.x = 0.0f;
        pos_delta.y = 0.0f;
    }
};

static void daHyperRemoDoor_c_PhysCallbackFoot(daHyperRemoDoor_c *one, dStageActor_c *two) {
    if (two->stageActorType != 1)
        return;

    if (one->currentDirection == DIRECTION_L || one->currentDirection == DIRECTION_R)
        return;

    one->overwritePosDeltaForPhysCallback();

    if (one->pos_delta.y > 0.0f)
        HurtMarioBecauseOfBeingSquashed(two, one, 1);
    else
        HurtMarioBecauseOfBeingSquashed(two, one, 9);
};
static void daHyperRemoDoor_c_PhysCallbackHead(daHyperRemoDoor_c *one, dStageActor_c *two) {
    if (two->stageActorType != 1)
        return;

    if (one->currentDirection == DIRECTION_L || one->currentDirection == DIRECTION_R)
        return;

    one->overwritePosDeltaForPhysCallback();

    if (one->pos_delta.y < 0.0f)
        HurtMarioBecauseOfBeingSquashed(two, one, 2);
    else
        HurtMarioBecauseOfBeingSquashed(two, one, 10);
};
static void daHyperRemoDoor_c_PhysCallbackWall(daHyperRemoDoor_c *one, dStageActor_c *two, u8 unk) {
    if (two->stageActorType != 1)
        return;

    if (one->currentDirection == DIRECTION_U || one->currentDirection == DIRECTION_D)
        return;

    one->overwritePosDeltaForPhysCallback();

    one->pos_delta.x = -1.0f;

    if (unk > 0) {
        if (one->pos_delta.x > 0.0f)
            HurtMarioBecauseOfBeingSquashed(two, one, 6);
        else
            HurtMarioBecauseOfBeingSquashed(two, one, 12);
    } else {
        if (one->pos_delta.x < 0.0f)
            HurtMarioBecauseOfBeingSquashed(two, one, 5);
        else
            HurtMarioBecauseOfBeingSquashed(two, one, 11);
    }
};
static bool daHyperRemoDoor_c_PhysCheckRevFoot(daHyperRemoDoor_c *one, dStageActor_c *two) {
    one->overwritePosDeltaForPhysCallback();
    return (one->pos_delta.y > 0.0f);
};
static bool daHyperRemoDoor_c_PhysCheckRevHead(daHyperRemoDoor_c *one, dStageActor_c *two) {
    one->overwritePosDeltaForPhysCallback();
    return (one->pos_delta.y < 0.0f);
};
static bool daHyperRemoDoor_c_PhysCheckRevWall(daHyperRemoDoor_c *one, dStageActor_c *two, u8 unk) {
    one->overwritePosDeltaForPhysCallback();
    if (unk > 0) {
        if (one->pos_delta.x > 0.0f)
            return true;
    } else {
        if (one->pos_delta.x < 0.0f)
            return true;
    }
    return false;
};

int daHyperRemoDoor_c::onCreate(void) {

    facingForwardAtCurrentPos = true;
    currentDirection = DIRECTION_NONE;

    int availableForwardSteps = (settings >> 4) & 0xf;
    int availableBackwardSteps = settings & 0xf;
    stepCounter = availableBackwardSteps;
    stepCounterMax = availableForwardSteps + availableBackwardSteps;

    primaryDirection = ((settings >> 8) & 3) + 1;

    if (primaryDirection == DIRECTION_U)
        oppositeDirection = DIRECTION_D;
    else if (primaryDirection == DIRECTION_R)
        oppositeDirection = DIRECTION_L;
    else if (primaryDirection == DIRECTION_D)
        oppositeDirection = DIRECTION_U;
    else
        oppositeDirection = DIRECTION_R;

    scale.x = 1.5f;
    scale.y = 1.66666666666666666666666666f;
    scale.z = 1.0f;
    pos.z = -32.0f * (settings >> 28);
    rot.x = 0.0f;
    rot.y = 0.0f;

    createMdl(primaryDirection);

    physics.setup(this,
        0.0f, 80.0f, 48.0f, 0.0f,
        (void*)&daHyperRemoDoor_c_PhysCallbackFoot,
        (void*)&daHyperRemoDoor_c_PhysCallbackHead,
        (void*)&daHyperRemoDoor_c_PhysCallbackWall,
        1, 0, 0);
    physics.callback1 = (void*)&daHyperRemoDoor_c_PhysCheckRevFoot;
    physics.callback2 = (void*)&daHyperRemoDoor_c_PhysCheckRevHead;
    physics.callback3 = (void*)&daHyperRemoDoor_c_PhysCheckRevWall;
    physics.addToList();

    acState.setState(&StateID_Ready);

    return ONCREATE_RESULT_SUCCESS;
};

void daHyperRemoDoor_c::createMdl(int direction) {
    allocator.link(-1, GameHeaps[0], 0, 0x20);

    // TODO: delete me
    // full door
    // resFile.data = getResource("obj_door_full", "g3d/obj_door.brres");
    // nw4r::g3d::ResMdl resmdl = resFile.GetResMdl("obj_doorA");

    char brresName[40] = "g3d/obj_hyper_remo_door_";
    if (direction == DIRECTION_U) {
        *(u32*)(&brresName[24]) = 0x75702e62;  // "up.b"
        *(u32*)(&brresName[28]) = 0x72726573;  // "rres"
        brresName[32] = 0;
    } else if (direction == DIRECTION_R) {
        *(u32*)(&brresName[24]) = 0x72696768;  // "righ"
        *(u32*)(&brresName[28]) = 0x742e6272;  // "t.br"
        *(u32*)(&brresName[32]) = 0x72657300;  // "res\0"
    } else if (direction == DIRECTION_D) {
        *(u32*)(&brresName[24]) = 0x646f776e;  // "down"
        *(u32*)(&brresName[28]) = 0x2e627272;  // ".brr"
        *(u32*)(&brresName[32]) = 0x65730000;  // "es\0\0"
    } else if (direction == DIRECTION_L) {
        *(u32*)(&brresName[24]) = 0x6c656674;  // "left"
        *(u32*)(&brresName[28]) = 0x2e627272;  // ".brr"
        *(u32*)(&brresName[32]) = 0x65730000;  // "es\0\0"
    }

    resFile.data = getResource("obj_hyper_remo_door", brresName);
    nw4r::g3d::ResMdl resmdl = resFile.GetResMdl("obj_doorA");
    model.setup(resmdl, &allocator, 0x20, 1, 0);
    SetupTextures_MapObj(&model, 0);

    allocator.unlink();
};

extern "C" void setOfs_9dBg_ctr_cFffffP7mVec3_c(Physics *physics, float x1, float y1, float x2, float y2, Vec *scale = 0);
int daHyperRemoDoor_c::onExecute() {
    acState.execute();

    // actually s16's, but u16 works better here because it's continuous at 0x8000
    u16 angleX = *(u16*)(&rot.x);
    u16 angleY = *(u16*)(&rot.y);

    // Calculate physics x pos
    float edgeCosX = -cos((float)angleY / 32768.0f * M_PI);  // -1 -> 1 -> -1
    float physicsX = pos.x + edgeCosX * 24.0 - 24.0;

    // Calculate physics y pos and height
    float edgeCosY = cos((float)angleX / 32768.0f * M_PI);  // 1 -> -1 -> 1
    float edgePadY = sin((float)angleX / 32768.0f * M_PI);  // 0 -> 1 -> 0
    float outerY = pos.y + edgeCosY * 80.0;
    float topY, bottomY;
    if (edgeCosY >= 0) {  // facing up
        // sigh, no fmax()...
        float temp1 = outerY;
        float temp2 = pos.y + 4.0;
        topY = (temp1 > temp2) ? temp1 : temp2;
        bottomY = pos.y - edgePadY * 8.0;
    } else {  // facing down
        topY = pos.y + edgePadY * 4.0;
        bottomY = outerY;
    }
    float physicsY = topY - 80.0f;
    float physicsHeight = 80.0f - (topY - bottomY);

    // Run physics->update() with our own position temporarily set to
    // the pos we want the physics to have, then change it back
    setOfs_9dBg_ctr_cFffffP7mVec3_c(&physics,
        0.0f,
        80.0f,
        48.0f,
        physicsHeight);
    float origX = pos.x;
    float origY = pos.y;
    pos.x = physicsX;
    pos.y = physicsY;
    physics.update();
    pos.x = origX;
    pos.y = origY;

    return 1;
};

int daHyperRemoDoor_c::onDraw(void) {
    // From daRemoDoor_c::onDraw(), modified
    PSMTXTrans(*matrix, pos.x, pos.y, pos.z);

    if (primaryDirection == DIRECTION_U || primaryDirection == DIRECTION_D) {
        if (!facingForwardAtCurrentPos) {
            s16 temp = rot.x - 0x8000;
            matrix.applyRotationX(&temp);
        } else {
            matrix.applyRotationX(&rot.x);
        }
    } else {
        if (!facingForwardAtCurrentPos) {
            s16 temp = rot.y - 0x8000;
            matrix.applyRotationY(&temp);
        } else {
            matrix.applyRotationY(&rot.y);
        }
    }

    Mtx temp;
    if (primaryDirection == DIRECTION_U || primaryDirection == DIRECTION_D) {
        if (!facingForwardAtCurrentPos)
            PSMTXTrans(temp, -24.0f, -80.0f, 0.0f);
        else
            PSMTXTrans(temp, -24.0f, 0.0f, 0.0f);
    } else {
        if (!facingForwardAtCurrentPos)
            PSMTXTrans(temp, 24.0f, 0.0f, 0.0f);
        else
            PSMTXTrans(temp, -24.0f, 0.0f, 0.0f);
    }

    PSMTXConcat(*matrix, temp, *matrix);
    model.setDrawMatrix(matrix);
    model.setScale(&scale);
    model.scheduleForDrawing();

    return 1;
};


// Obviously this trick will break in multiplayer (in which case, it should check if it's
// closest to the center of the screen instead), but it should be fine for singleplayer
bool daHyperRemoDoor_c::amIClosestToMario(void) {
    dStageActor_c *mario = GetSpecificPlayerActor(0);

    dActor_c *candidate = NULL;
    dActor_c *closest = NULL;
    float closestDist = 3.402823e+38;  // FLT_MAX

    while (candidate = Actor_SearchByName(HYPER_REMO_DOOR_PROFILE_NUM, candidate)) {
        float itsX = candidate->pos.x;
        float itsY = candidate->pos.y;
        float itsDistX = itsX - mario->pos.x;
        float itsDistY = itsY - mario->pos.y;
        float itsDist = sqrtf(itsDistX * itsDistX + itsDistY * itsDistY);
        if (itsDist < closestDist) {
            closest = candidate;
            closestDist = itsDist;
        }
    }

    return (u32)this == (u32)closest;
};


CREATE_STATE_E(daHyperRemoDoor_c, Ready);
void daHyperRemoDoor_c::executeState_Ready(void) {
    int tiltState = GetTiltStateForRemoDoor();
    if (tiltState == TILT_STATE_RIGHT && stepCounter < stepCounterMax) {
        currentDirection = primaryDirection;
        acState.setState(&StateID_Moving);
        stepCounter++;
    } else if (tiltState == TILT_STATE_LEFT && stepCounter > 0) {
        currentDirection = oppositeDirection;
        acState.setState(&StateID_Moving);
        stepCounter--;
    }
};

CREATE_STATE(daHyperRemoDoor_c, Moving);
void daHyperRemoDoor_c::beginState_Moving(void) {
    if (facingForwardAtCurrentPos && amIClosestToMario()) {
        PlaySound_Better(this, SE_OBJ_DOOR_OPEN);
    }

    if (currentDirection == DIRECTION_U) {
        pos.y += 80.0f;
        rot.x = 0x8000;
        facingForwardAtCurrentPos = !facingForwardAtCurrentPos;
    } else if (currentDirection == DIRECTION_R) {
        rot.y = 0;
    } else if (currentDirection == DIRECTION_D) {
        rot.x = 0;
    } else if (currentDirection == DIRECTION_L) {
        pos.x -= 48.0f;
        rot.y = 0x8000;
        facingForwardAtCurrentPos = !facingForwardAtCurrentPos;
    }
};
void daHyperRemoDoor_c::executeState_Moving(void) {
    if (currentDirection == DIRECTION_U) {
        rot.x -= DOOR_FLIP_SPEED;
        if (rot.x < 0)
            acState.setState(&StateID_Ready);

    } else if (currentDirection == DIRECTION_R) {
        rot.y += DOOR_FLIP_SPEED;
        if (rot.y < 0)
            acState.setState(&StateID_Ready);

    } else if (currentDirection == DIRECTION_D) {
        rot.x += DOOR_FLIP_SPEED;
        if (rot.x < 0)
            acState.setState(&StateID_Ready);

    } else if (currentDirection == DIRECTION_L) {
        rot.y -= DOOR_FLIP_SPEED;
        if (rot.y < 0)
            acState.setState(&StateID_Ready);
    }
};
void daHyperRemoDoor_c::endState_Moving(void) {
    if (currentDirection == DIRECTION_U) {
        rot.x = 0;
    } else if (currentDirection == DIRECTION_R) {
        rot.y = 0;
        pos.x += 48.0f;
        facingForwardAtCurrentPos = !facingForwardAtCurrentPos;
    } else if (currentDirection == DIRECTION_D) {
        rot.x = 0;
        pos.y -= 80.0f;
        facingForwardAtCurrentPos = !facingForwardAtCurrentPos;
    } else if (currentDirection == DIRECTION_L) {
        rot.y = 0;
    }
    currentDirection = DIRECTION_NONE;

    if (facingForwardAtCurrentPos && amIClosestToMario()) {
        PlaySound_Better(this, SE_OBJ_DOOR_CLOSE);
    }
};


OVERWRITE_PROFILE(HYPER_REMO_DOOR, HYPER_REMO_DOOR_PROFILE_NUM,
    &daHyperRemoDoor_c::build,
    HYPER_REMO_DOOR_PROFILE_NUM, HYPER_REMO_DOOR_PROFILE_NUM, 0);
OVERWRITE_SPRITE_DATA(HYPER_REMO_DOOR_SPRITE_NUM, HYPER_REMO_DOOR_PROFILE_NUM,
    0x30, -0x50,
    0, 0, 0, 0,
    SPRITE_FLAGS_ALWAYS_SPAWNED);
OVERWRITE_SPRITE_RESOURCES(HYPER_REMO_DOOR, HYPER_REMO_DOOR_SPRITE_NUM, "obj_hyper_remo_door");
