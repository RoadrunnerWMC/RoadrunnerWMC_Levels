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
#include "extra_externals.h"
#include "new_sprites/macros.h"
#include "util/tilt_state_for_remo_door.h"


// Unused giant Wiimote door (spr 319) hacks


// settings & 0x01000000: add solid collision box
#define MACRO_WIIMOTE_DOOR_USING_COLLISION ((this_->settings >> 24) & 1)

/* 80978a00 */ class daRemoDoor_c : public dActorState_c {
public:

    /* 0x3d0 */ mHeapAllocator_c allocator;
    /* 0x3ec */ nw4r::g3d::ResFile resFile;
    /* 0x3f0 */ m3d::mdl_c model;
    /* end: 0x430 */

    /* 808958e0 */ static daRemoDoor_c *build(void);
    /* 80895950 */ void createMdl(void);
    /* 80895a00 */ virtual int onCreate(void);
    /* 80895aa0 */ virtual int onExecute(void);
    /* 80895b00 */ virtual int onDraw(void);
    /* 80895bb0 */ void beginState_OpenReady(void);
    /* 80895bc0 */ void endState_OpenReady(void);
    /* 80895bd0 */ void executeState_OpenReady(void);
    /* 80895c50 */ void beginState_Open(void);
    /* 80895ca0 */ void endState_Open(void);
    /* 80895cb0 */ void executeState_Open(void);
    /* 80895d50 */ void beginState_Close(void);
    /* 80895d60 */ void endState_Close(void);
    /* 80895db0 */ void executeState_Close(void);
    /* 80895df0 */ ~daRemoDoor_c(void);

    USING_STATES(daRemoDoor_c);
    /* 8099e598 */ REF_NINTENDO_STATE(OpenReady);
    /* 8099e5d8 */ REF_NINTENDO_STATE(Open);
    /* 8099e618 */ REF_NINTENDO_STATE(Close);

    /* NEW */
    DECLARE_STATE(CloseReady);
};

class daRemoDoorImproved_c : public daRemoDoor_c {
public:
    Physics physics;
    u32 posManagedActorId;
};

CREATE_STATE(daRemoDoor_c, CloseReady);
void daRemoDoor_c::beginState_CloseReady() {
    rot.y = 0x8000;
};
void daRemoDoor_c::executeState_CloseReady() {
    if (GetTiltStateForRemoDoor() <= TILT_STATE_NEUTRAL)
        acState.setState(&daRemoDoor_c::StateID_Close);
};
void daRemoDoor_c::endState_CloseReady() {}

// Improve spawning behavior
#define REMO_DOOR_SPRITE_NUM 319
#define REMO_DOOR_PROFILE_NUM 208
OVERWRITE_SPRITE_DATA(REMO_DOOR_SPRITE_NUM, REMO_DOOR_PROFILE_NUM,
    0x30, -0x50,
    0, 0, 0x30, 0x50,
    SPRITE_FLAGS_UNK_8);

// Load obj_remo_door.arc instead of obj_door.arc
OVERWRITE_SPRITE_RESOURCES(REMO_DOOR, REMO_DOOR_SPRITE_NUM, "obj_remo_door");
// And also use it
extern "C" void *DVDClass;
const char daRemoDoor_c_ResName[] = "obj_remo_door";
kmCallDefAsm(0x808959a0) {
    nofralloc  // don't auto-generate a prologue
    lis r4, daRemoDoor_c_ResName@h
    ori r4, r4, daRemoDoor_c_ResName@l
    b GetRes
};

// settings & 0xf0000000: z-order (higher values = farther behind)
// (0 = retail default, in front of Mario; 1+ = same values as for regular doors)
void WiimoteDoor_SetZOrder(daRemoDoorImproved_c *this_) {
    int zorder_int = (this_->settings >> 28) & 0xf;
    if (zorder_int == 0) {
        this_->pos.z = 5500.0;
    } else {
        this_->pos.z = 32.0f - 100.0f * zorder_int;
    }
};
kmBranchDefAsm(0x80895a5c, 0x80895a60) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r30
    bl WiimoteDoor_SetZOrder
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// Bugfix: door will now spawn in the correct state depending on the initial remote tilt
void WiimoteDoor_SetInitialState(daRemoDoorImproved_c *this_) {
    if (GetTiltStateForRemoDoor() == TILT_STATE_RIGHT)
        this_->acState.setState(&daRemoDoor_c::StateID_CloseReady);
    else
        this_->acState.setState(&daRemoDoor_c::StateID_OpenReady);
};
kmBranchDefAsm(0x80895a70, 0x80895a80) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r30
    bl WiimoteDoor_SetInitialState
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// Allocate extra space for new things
kmWrite32(0x808958e8, 0x38600000 | sizeof(daRemoDoorImproved_c));  // li r3, (size)

// Physics callbacks
static void WiimoteDoor_PhysCallbackFoot(dStageActor_c *one, dStageActor_c *two) {}
static void WiimoteDoor_PhysCallbackHead(dStageActor_c *one, dStageActor_c *two) {}
static void WiimoteDoor_PhysCallbackWall(dStageActor_c *one, dStageActor_c *two, u8 unk) {}

// Set up Physics
void WiimoteDoor_SetUpPhysics(daRemoDoorImproved_c *this_) {
    if (MACRO_WIIMOTE_DOOR_USING_COLLISION) {
        __ct__7PhysicsFv(&this_->physics);
        this_->physics.setup(this_,
            0.0f, 80.0f, 48.0f, 0.0f,
            (void*)&WiimoteDoor_PhysCallbackFoot,
            (void*)&WiimoteDoor_PhysCallbackHead,
            (void*)&WiimoteDoor_PhysCallbackWall,
            1, 0, 0);
        this_->physics.addToList();
    }
};

void WiimoteDoor_AppendToCreate(daRemoDoorImproved_c *this_) {
    WiimoteDoor_SetUpPhysics(this_);
    this_->posManagedActorId = 0;
};
kmBranchDefAsm(0x80895a84, 0x80895a88) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r30
    bl WiimoteDoor_AppendToCreate
    ASM_RESTORE_CONTEXT_FROM_STACK
    li r3, 1  // (original instruction)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// Update Physics in Execute
// Also get rid of the call to dStageActor_c::checkZoneBoundaries()
#define CHECKPOINT_ACTOR_ID 235
void WiimoteDoor_Execute(daRemoDoorImproved_c *this_) {

    // actually an s16, but u16 works better here because it's continuous at 0x8000
    u16 rotAngle = *(u16*)(&this_->rot.y);
    float edgeCos = -cos((float)rotAngle / 32768.0f * M_PI);  // -1 -> 1 -> -1
    float physics_x = this_->pos.x + edgeCos * 24.0 - 24.0;

    if (MACRO_WIIMOTE_DOOR_USING_COLLISION) {
        // Run physics->update() with our own x position temporarily set to
        // the x pos we want the physics to have, then change it back
        float orig_x = this_->pos.x;
        this_->pos.x = physics_x;
        this_->physics.update();
        this_->pos.x = orig_x;
    }

    // settings & 0x02000000: attach a checkpoint
    // Do this in execute() because the checkpoint may spawn after the door does
    if (((this_->settings >> 24) & 2) && (this_->posManagedActorId == 0)) {
        dActor_c *candidate = NULL;
        while (candidate = Actor_SearchByName(CHECKPOINT_ACTOR_ID, candidate)) {
            // note: a remodoor's x/y position at runtime is at the bottom of the hinge
            if ((this_->pos.x - 48.0f <= candidate->pos.x && candidate->pos.x <= this_->pos.x + 48.0f)
                    && (this_->pos.y + 64.0f <= candidate->pos.y && candidate->pos.y <= this_->pos.y + 144.0f)) {
                this_->posManagedActorId = candidate->id;
                break;
            }
        }
    }

    // update checkpoint pos if there is one
    if (this_->posManagedActorId) {
        dActor_c *posManagedActor = Actor_SearchByID(this_->posManagedActorId);
        if (posManagedActor == NULL) {
            // start searching for it again, I guess
            this_->posManagedActorId = 0;
        } else {
            posManagedActor->pos.x = physics_x + 24.0f;
        }
    }
};
kmBranchDefAsm(0x80895ab8, 0x80895acc) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    bl WiimoteDoor_Execute
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// Delete Physics
int WiimoteDoor_Delete(daRemoDoorImproved_c *this_) {
    if (MACRO_WIIMOTE_DOOR_USING_COLLISION) {
        this_->physics.removeFromList();
        __dt__7PhysicsFv(&this_->physics);
    }
    return 1;
};
kmWritePointer(0x80978a14, &WiimoteDoor_Delete);
