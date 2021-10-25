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


class daMassDoorRenderer_c : public dStageActor_c {
public:

    nw4r::g3d::ResFile resFile;
    mHeapAllocator_c allocator;
    int num_models;
    m3d::mdl_c *models;

    static daMassDoorRenderer_c *build();

    int onCreate();
    void createMdls(int pathId);
    int onDraw();
    int onDelete();
};

daMassDoorRenderer_c *daMassDoorRenderer_c::build() {
    return new(AllocFromGameHeap1(sizeof(daMassDoorRenderer_c))) daMassDoorRenderer_c;
};

int daMassDoorRenderer_c::onCreate() {
    pos.z = 30.0f;
    createMdls(settings & 0xff);

    return ONCREATE_RESULT_SUCCESS;
};

void daMassDoorRenderer_c::createMdls(int pathId) {

    // get pointer to course
    dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());

    // get path info
    dCourse_c::rail_s *path = GetRail(pathId);
    num_models = path->nodeCount;

    // allocate space for mdl_c's
    models = (m3d::mdl_c*)AllocFromGameHeap1(sizeof(m3d::mdl_c) * num_models);

    // load the model resource
    allocator.link(-1, GameHeaps[0], 0, 0x20);
    resFile.data = getResource("obj_door_flat", "g3d/obj_door_flat.brres");
    nw4r::g3d::ResMdl resmdl = resFile.GetResMdl("obj_door_flat");

    // make each individual model instance
    for (int i = 0; i < num_models; i++) {
        m3d::mdl_c *model = &models[i];
        dCourse_c::railNode_s *pathNode = &course->railNode[path->startNode + i];

        __ct__Q23m3d5mdl_cFv(model);
        model->setup(resmdl, &allocator, 0, 1, 0);
        SetupTextures_MapObj(model, 0);

        matrix.translation(pathNode->xPos, -pathNode->yPos, pos.z);

        if (pathNode->unk1 == 1) {  // down
            s16 zRot = 0x8000;
            matrix.applyRotationZ(&zRot);
            matrix.applyTranslation(32.0f, -48.0f, 0.0f);
        } else if (pathNode->unk1 == 2) {  // left
            s16 zRot = 0x4000;
            matrix.applyRotationZ(&zRot);
            matrix.applyTranslation(0.0f, -32.0f, 0.0f);
        } else if (pathNode->unk1 == 3) {  // right
            s16 zRot = 0xC000;
            matrix.applyRotationZ(&zRot);
            matrix.applyTranslation(48.0f, 0.0f, 0.0f);
        }
        model->setDrawMatrix(matrix);
    }

    allocator.unlink();
};

int daMassDoorRenderer_c::onDraw() {
    for (int i = 0; i < num_models; i++) {
        m3d::mdl_c *model = &models[i];
        model->calcWorld(false);
        model->scheduleForDrawing();
    }
    return 1;
};

int daMassDoorRenderer_c::onDelete() {
    for (int i = 0; i < num_models; i++) {
        m3d::mdl_c *model = &models[i];
        __dt__Q23m3d5mdl_cFv(model);
    }
    FreeFromGameHeap1(models);
    return 1;
};


OVERWRITE_PROFILE(MASS_DOOR_RENDERER, MASS_DOOR_RENDERER_PROFILE_NUM,
    &daMassDoorRenderer_c::build,
    MASS_DOOR_RENDERER_PROFILE_NUM, MASS_DOOR_RENDERER_PROFILE_NUM, 0);

OVERWRITE_SPRITE_DATA(MASS_DOOR_RENDERER_SPRITE_NUM, MASS_DOOR_RENDERER_PROFILE_NUM,
    0, 0,        // spawn offset: x/y
    0, 0, 0, 0,  // spawn rect: x1/y1/x2/y2
    SPRITE_FLAGS_ALWAYS_SPAWNED);

OVERWRITE_SPRITE_RESOURCES(MASS_DOOR_RENDERER, MASS_DOOR_RENDERER_SPRITE_NUM, "obj_door_flat");
