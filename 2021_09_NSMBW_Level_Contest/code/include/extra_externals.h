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

#ifndef EXTRA_EXTERNALS_H
#define EXTRA_EXTERNALS_H

#include <dCourse.h>


extern "C" {
    /* 80022080 */ extern void player_set_15daEnBlockMain_cFii(void *this_, int a, int b);
    /* 8002b3c0 */ extern bool daEnDoor_c_checkOpenOk(dStageActor_c *this_);
    /* 80056370 */ extern void HurtMarioBecauseOfBeingSquashed(void *mario, dStageActor_c *squasher, int type);
    /* 8006a4b0 */ extern void ConvertStagePositionIntoScreenPosition__Maybe(Vec *one, Vec *two);
    /* 8006ced0 */ extern void Actor_GetVec3WithValuesAdded(Vec *dest, void *obj);
    /* 8007f7a0 */ extern void __ct__7PhysicsFv(Physics *physics);
    /* 8007f810 */ extern void __dt__7PhysicsFv(Physics *physics);
    /* 800b3600 */ extern void hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi(int player, int id);
    /* 800b3720 */ extern void hashname_5edaa1df_bf88549a_gameCom_setFukidashi234(int player, int id, int param3);
    /* 800b3750 */ extern void hashname_9723880b_dea8588e_gameCom_setFukidashiAll234(int player, int id, int param3);
    /* 800b5ca0 */ extern u32 shiftedTiltAmount__7RemoconFv(Remocon *this_);
    /* 800d91b0 */ extern dCourse_c::rail_s* GetRail(int railId);
    /* 800daf70 */ extern void addToList__13BasicColliderFv(void);
    /* 800dafc0 */ extern void removeFromList__13BasicColliderFv(void);
    /* 800fd140 */ extern bool FUN_P1_800fd140(void);
    /* 80162e90 */ extern dActor_c *Actor_SearchByName(u32 actorID, dActor_c *after);
    /* 80169e60 */ extern void __dt__Q23m3d5mdl_cFv(m3d::mdl_c *this_);
    /* 8016c030 */ extern void __ct__Q23m3d5mdl_cFv(m3d::mdl_c *this_);
    /* 80198d70 */ extern void SoundPlayingClass_PlaySoundAtPosition(SoundPlayingClass *spc, int id, Vec *pos, int unk);
    /* 80199100 */ extern void YetAnotherSoundPlayer(void *src, int id, Vec *pos, int whatever);
    /* 80429f84 */ extern int SomethingPlayerRelated_80429f84;
    /* 8042a03c */ extern void *SoundClassRelated;
    /* 8042a768 */ extern void *BufferToCheckIfPlaying;
}  // extern "C"


#endif  // EXTRA_EXTERNALS_H
