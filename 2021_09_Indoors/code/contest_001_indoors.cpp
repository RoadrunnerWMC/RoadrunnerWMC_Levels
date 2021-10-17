#include <kamek.h>
#include <game.h>
#include <dCourse.h>
#include <sfx.h>


/* **************************************************************** */
/* ******************** Custom assembly macros ******************** */
/* **************************************************************** */

// Full register-file save/restore

#define ASM_DUMP_CONTEXT_TO_STACK \
    stwu  sp,  -0x188(sp) ; \
    stw   r0,  0x08(sp)   ; \
    stw   r3,  0x0C(sp)   ; \
    stw   r4,  0x10(sp)   ; \
    stw   r5,  0x14(sp)   ; \
    stw   r6,  0x18(sp)   ; \
    stw   r7,  0x1C(sp)   ; \
    stw   r8,  0x20(sp)   ; \
    stw   r9,  0x24(sp)   ; \
    stw   r10, 0x28(sp)   ; \
    stw   r11, 0x2C(sp)   ; \
    stw   r12, 0x30(sp)   ; \
    stw   r14, 0x34(sp)   ; \
    stw   r15, 0x38(sp)   ; \
    stw   r16, 0x3C(sp)   ; \
    stw   r17, 0x40(sp)   ; \
    stw   r18, 0x44(sp)   ; \
    stw   r19, 0x48(sp)   ; \
    stw   r20, 0x4C(sp)   ; \
    stw   r21, 0x50(sp)   ; \
    stw   r22, 0x54(sp)   ; \
    stw   r23, 0x58(sp)   ; \
    stw   r24, 0x5C(sp)   ; \
    stw   r25, 0x60(sp)   ; \
    stw   r26, 0x64(sp)   ; \
    stw   r27, 0x68(sp)   ; \
    stw   r28, 0x6C(sp)   ; \
    stw   r29, 0x70(sp)   ; \
    stw   r30, 0x74(sp)   ; \
    stw   r31, 0x78(sp)   ; \
    stfd  f0,  0x80(sp)   ; \
    stfd  f1,  0x88(sp)   ; \
    stfd  f2,  0x90(sp)   ; \
    stfd  f3,  0x98(sp)   ; \
    stfd  f4,  0xA0(sp)   ; \
    stfd  f5,  0xA8(sp)   ; \
    stfd  f6,  0xB0(sp)   ; \
    stfd  f7,  0xB8(sp)   ; \
    stfd  f8,  0xC0(sp)   ; \
    stfd  f9,  0xC8(sp)   ; \
    stfd  f10, 0xD0(sp)   ; \
    stfd  f11, 0xD8(sp)   ; \
    stfd  f12, 0xE0(sp)   ; \
    stfd  f13, 0xE8(sp)   ; \
    stfd  f14, 0xF0(sp)   ; \
    stfd  f15, 0xF8(sp)   ; \
    stfd  f16, 0x100(sp)  ; \
    stfd  f17, 0x108(sp)  ; \
    stfd  f18, 0x110(sp)  ; \
    stfd  f19, 0x118(sp)  ; \
    stfd  f20, 0x120(sp)  ; \
    stfd  f21, 0x128(sp)  ; \
    stfd  f22, 0x130(sp)  ; \
    stfd  f23, 0x138(sp)  ; \
    stfd  f24, 0x140(sp)  ; \
    stfd  f25, 0x148(sp)  ; \
    stfd  f26, 0x150(sp)  ; \
    stfd  f27, 0x158(sp)  ; \
    stfd  f28, 0x160(sp)  ; \
    stfd  f29, 0x168(sp)  ; \
    stfd  f30, 0x170(sp)  ; \
    stfd  f31, 0x178(sp)  ; \
    mfcr  r0              ; \
    stw   r0,  0x180(sp)  ; \
    mfctr r0              ; \
    stw   r0,  0x184(sp)  ; \
    mflr  r0              ; \
    stw   r0,  0x18C(sp)  ; \
    lwz   r0,  0x08(sp)

#define ASM_RESTORE_CONTEXT_FROM_STACK \
    lwz   r0,  0x180(sp) ; \
    mtcr  r0             ; \
    lwz   r0,  0x184(sp) ; \
    mtctr r0             ; \
    lwz   r0,  0x18C(sp) ; \
    mtlr  r0             ; \
    lwz   r0,  0x08(sp)  ; \
    lwz   r3,  0x0C(sp)  ; \
    lwz   r4,  0x10(sp)  ; \
    lwz   r5,  0x14(sp)  ; \
    lwz   r6,  0x18(sp)  ; \
    lwz   r7,  0x1C(sp)  ; \
    lwz   r8,  0x20(sp)  ; \
    lwz   r9,  0x24(sp)  ; \
    lwz   r10, 0x28(sp)  ; \
    lwz   r11, 0x2C(sp)  ; \
    lwz   r12, 0x30(sp)  ; \
    lwz   r14, 0x34(sp)  ; \
    lwz   r15, 0x38(sp)  ; \
    lwz   r16, 0x3C(sp)  ; \
    lwz   r17, 0x40(sp)  ; \
    lwz   r18, 0x44(sp)  ; \
    lwz   r19, 0x48(sp)  ; \
    lwz   r20, 0x4C(sp)  ; \
    lwz   r21, 0x50(sp)  ; \
    lwz   r22, 0x54(sp)  ; \
    lwz   r23, 0x58(sp)  ; \
    lwz   r24, 0x5C(sp)  ; \
    lwz   r25, 0x60(sp)  ; \
    lwz   r26, 0x64(sp)  ; \
    lwz   r27, 0x68(sp)  ; \
    lwz   r28, 0x6C(sp)  ; \
    lwz   r29, 0x70(sp)  ; \
    lwz   r30, 0x74(sp)  ; \
    lwz   r31, 0x78(sp)  ; \
    lfd   f0,  0x80(sp)  ; \
    lfd   f1,  0x88(sp)  ; \
    lfd   f2,  0x90(sp)  ; \
    lfd   f3,  0x98(sp)  ; \
    lfd   f4,  0xA0(sp)  ; \
    lfd   f5,  0xA8(sp)  ; \
    lfd   f6,  0xB0(sp)  ; \
    lfd   f7,  0xB8(sp)  ; \
    lfd   f8,  0xC0(sp)  ; \
    lfd   f9,  0xC8(sp)  ; \
    lfd   f10, 0xD0(sp)  ; \
    lfd   f11, 0xD8(sp)  ; \
    lfd   f12, 0xE0(sp)  ; \
    lfd   f13, 0xE8(sp)  ; \
    lfd   f14, 0xF0(sp)  ; \
    lfd   f15, 0xF8(sp)  ; \
    lfd   f16, 0x100(sp) ; \
    lfd   f17, 0x108(sp) ; \
    lfd   f18, 0x110(sp) ; \
    lfd   f19, 0x118(sp) ; \
    lfd   f20, 0x120(sp) ; \
    lfd   f21, 0x128(sp) ; \
    lfd   f22, 0x130(sp) ; \
    lfd   f23, 0x138(sp) ; \
    lfd   f24, 0x140(sp) ; \
    lfd   f25, 0x148(sp) ; \
    lfd   f26, 0x150(sp) ; \
    lfd   f27, 0x158(sp) ; \
    lfd   f28, 0x160(sp) ; \
    lfd   f29, 0x168(sp) ; \
    lfd   f30, 0x170(sp) ; \
    lfd   f31, 0x178(sp) ; \
    addi  sp,  sp, 0x188

// Kamek *HELPFULLY* errors out ("Function contains a return partway through")
// if there's a blr within a kmBranchDefAsm(x, y) hook.
// So I substitute a useless instruction that codewarrior would never generate
// naturally for blr, and the build script later does find-replace on it in the
// output kamekfiles.
#define BACKDOOR_BLR or r31, r31, r31  // 0x7ffffb78


/* **************************************************************** */
/* *********** Custom sprite/profile replacement macros *********** */
/* **************************************************************** */

#define PROFILES_TABLE 0x8076a748
#define SPRITES_TABLE 0x8030a340
#define SPRITE_RESOURCES_TABLE 0x8031ab4c

typedef struct Profile {
    dActor_c* (*buildFunc)(void);
    u16 _4;
    u16 _6;
    u32 _8;
} Profile;

#define OVERWRITE_PROFILE(profileName, profileId, buildFunc, a, b, c) \
    Profile g_profile_##profileName = {(dActor_c* (*)(void))(buildFunc), (a), (b), (c)}; \
    kmWritePointer(PROFILES_TABLE + 4 * (profileId), &g_profile_##profileName)

// Adjacent 16-bit writes here are combined into 32-bit writes to optimize the kamekfiles a little
#define OVERWRITE_SPRITE_DATA(spriteId, profileId, xOff, yOff, rectX1, rectY1, rectX2, rectY2, flags) \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x00, (profileId) << 16); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x04, (xOff)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x08, (yOff)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x0C, (rectX1)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x10, (rectY1)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x14, (rectX2)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x18, (rectY2)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x1C, (0)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x20, (0)); \
    kmWrite32(SPRITES_TABLE + 0x28 * (spriteId) + 0x24, (flags) << 16)

#define OVERWRITE_SPRITE_RESOURCES(profileName, spriteId, ...) \
    const char *l_##profileName##_res[] = {__VA_ARGS__, NULL}; \
    kmWritePointer(SPRITE_RESOURCES_TABLE + 4 * (spriteId), &l_##profileName##_res)

#define OVERWRITE_SPRITE_RESOURCES_NULL(spriteId) \
    kmWrite32(SPRITE_RESOURCES_TABLE + 4 * (spriteId), 0)

#define SPRITE_FLAGS_NULL 0
#define SPRITE_FLAGS_ALWAYS_SPAWNED 2
#define SPRITE_FLAGS_UNK_8 8

#define ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME 0
#define ONCREATE_RESULT_SUCCESS 1
#define ONCREATE_RESULT_PLEASE_DELETE_ME 2


/* **************************************************************** */
/* ****************** GLOBAL: Anti-hardcode hacks ***************** */
/* **************************************************************** */

// Disable hardcoded 1-1 bg
kmWrite32(0x801180A0, 0x48000018);


/* **************************************************************** */
/* ********************* GLOBAL: Camera hacks ********************* */
/* **************************************************************** */

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


/* **************************************************************** */
/* ********* GLOBAL: C++ version of PlaySound() from Newer ******** */
/* **************************************************************** */

/* 8006ced0 */ extern "C" void Actor_GetVec3WithValuesAdded(Vec *dest, void *obj);
/* 8006a4b0 */ extern "C" void ConvertStagePositionIntoScreenPosition__Maybe(Vec *one, Vec *two);
/* 80199100 */ extern "C" void YetAnotherSoundPlayer(void *src, int id, Vec *pos, int whatever);
/* 8042a768 */ extern "C" void *BufferToCheckIfPlaying;
/* 8042a03c */ extern "C" void *SoundClassRelated;

void PlaySound(dActor_c *this_, int id) {
    // Actor_GetVec3WithValuesAdded adds pos_delta which is dumb and bad, so, skipping that
    Vec vec1 = {this_->pos.x, this_->pos.y, this_->pos.z};

    Vec vec2;
    ConvertStagePositionIntoScreenPosition__Maybe(&vec1, &vec2);

    if (!CheckIfPlayingSound(BufferToCheckIfPlaying, id))
        YetAnotherSoundPlayer(SoundClassRelated, id, &vec2, 0);
};

// Decompiled from daRemoDoor_c::initializeState_Open(void). Seems to work better than the other one.

/* 8006a4b0 */ extern void ConvertStagePositionToScreenPosition(Point2d *screenPos, Point3d *stagePos);
/* 80198d70 */ extern "C" void SoundPlayingClass_PlaySoundAtPosition(SoundPlayingClass *spc, int id, Vec *pos, int unk);
extern "C" void PlaySound_Better(dStageActor_c *this_, int id) {
    Vec3 temp1 = {0.0f, 0.0f, 0.0f};
    ConvertStagePositionToScreenPosition((Vec2*)(&temp1), &this_->pos);
    SoundPlayingClass_PlaySoundAtPosition(SoundPlayingClass::instance2, id, &temp1, 0);
};


/* **************************************************************** */
/* **** GLOBAL: Custom function for checking wiimote tilt state *** */
/* **************************************************************** */

/* 80429f84 */ extern "C" int SomethingPlayerRelated_80429f84;
/* 800b5ca0 */ extern "C" u32 shiftedTiltAmount__7RemoconFv(Remocon *this_);

// You can use < and > to compare these from leftmost to rightmost
#define TILT_STATE_LEFT -2
#define TILT_STATE_NEUTRAL_LEFT -1
#define TILT_STATE_NEUTRAL 0
#define TILT_STATE_NEUTRAL_RIGHT 1
#define TILT_STATE_RIGHT 2

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


/* **************************************************************** */
/* ****************** Boo circle (spr 323) hacks ****************** */
/* **************************************************************** */

void BooCircle_SetInitialRotationAngle(dStageActor_c *this_) {
    // settings & 0xf0000000: initial rotation angle
    *((s16*)((u32)this_ + 0x448)) = (this_->settings >> 28) << 12;
};

kmCallDefAsm(0x8089ff60) {
    nofralloc  // don't auto-generate a prologue
    stw r8, 0x3d0(r3)  // (original instruction)
    ASM_DUMP_CONTEXT_TO_STACK
    bl BooCircle_SetInitialRotationAngle
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start
};


/* **************************************************************** */
/* **************** Arrow Signboard (spr 310) hacks *************** */
/* **************************************************************** */

void ArrowSignboard_SetRotation(dStageActor_c *this_) {
    // settings & 0xffff0000: rotation angle
    this_->rot.z = this_->settings >> 16;
};

kmBranchDefAsm(0x8081c950, 0x8081c954) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r31
    bl ArrowSignboard_SetRotation
    ASM_RESTORE_CONTEXT_FROM_STACK
    lwz r31, 0x1c(sp)  // (original instruction)
    blr  // needed because we used nofralloc at the start
};


/* **************************************************************** */
/* ****************** Doors (sprs 259, 276) hacks ***************** */
/* **************************************************************** */

/* 8002b3c0 */ extern "C" bool daEnDoor_c_checkOpenOk(dStageActor_c *this_);

// If settings & 0x10000000, door is non-enterable
kmCallDefCpp(0x8002b308, bool, dStageActor_c *this_) {
    if ((this_->settings >> 28) & 1)
        return false;
    else
        return daEnDoor_c_checkOpenOk(this_);
};

// settings & 0x0f000000: z-order (higher values = farther behind)
void Door_SetZOrder(dStageActor_c *this_) {
    int zorder_int = (this_->settings >> 24) & 0xf;
    this_->pos.z = 32.0f - 100.0f * zorder_int;
};
kmBranchDefAsm(0x8002af10, 0x8002af14) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r31
    bl Door_SetZOrder
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};


/* **************************************************************** */
/* ********************** Mass door renderer ********************** */
/* **************************************************************** */

/* 8016c030 */ extern "C" void __ct__Q23m3d5mdl_cFv(m3d::mdl_c *this_);
/* 80169e60 */ extern "C" void __dt__Q23m3d5mdl_cFv(m3d::mdl_c *this_);
/* 800d91b0 */ extern "C" dCourse_c::rail_s* GetRail(int railId);

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


// Replaces KAWANAGARE (actor 41, sprite 250)
#define MASS_DOOR_RENDERER_PROFILE_NUM 41
#define MASS_DOOR_RENDERER_SPRITE_NUM 250

OVERWRITE_PROFILE(MASS_DOOR_RENDERER, MASS_DOOR_RENDERER_PROFILE_NUM,
    &daMassDoorRenderer_c::build,
    MASS_DOOR_RENDERER_PROFILE_NUM, MASS_DOOR_RENDERER_PROFILE_NUM, 0);

OVERWRITE_SPRITE_DATA(MASS_DOOR_RENDERER_SPRITE_NUM, MASS_DOOR_RENDERER_PROFILE_NUM,
    0, 0,        // spawn offset: x/y
    0, 0, 0, 0,  // spawn rect: x1/y1/x2/y2
    SPRITE_FLAGS_ALWAYS_SPAWNED);

OVERWRITE_SPRITE_RESOURCES(MASS_DOOR_RENDERER, MASS_DOOR_RENDERER_SPRITE_NUM, "obj_door_flat");


/* **************************************************************** */
/* ************ Motion-control input prompt controller ************ */
/* **************************************************************** */

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
/* 800b3600 */ extern "C" void hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi(int player, int id);
// Permanently end fukidashi -- will not be shown again for this player + ID for the rest of the level.
// Use this when the player has demonstrated they definitely know how to use the motion mechanic.
// For example, the tilt platforms in 1-2 call this when the player has tilted the platform to a maximum
// angle eight times.
/* 800b3720 */ extern "C" void hashname_5edaa1df_bf88549a_gameCom_setFukidashi234(int player, int id, int param3);
// Temporarily end fukidashi; it can be re-shown again later.
// For example, the tilt platforms in 1-2 call this when the player leaves the platform.
/* 800b3750 */ extern "C" void hashname_9723880b_dea8588e_gameCom_setFukidashiAll234(int player, int id, int param3);

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


// Replaces RIVER_MGR (actor 50, sprite 244)
#define MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM 50
#define MOTION_CONTROL_PROMPT_MNG_SPRITE_NUM 244

OVERWRITE_PROFILE(MOTION_CONTROL_PROMPT_MNG, MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM,
    &daMotionControlPromptMng_c::build,
    MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM, MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM, 0);

OVERWRITE_SPRITE_DATA(MOTION_CONTROL_PROMPT_MNG_SPRITE_NUM, MOTION_CONTROL_PROMPT_MNG_PROFILE_NUM,
    0, 0,        // spawn offset: x/y
    0, 0, 0, 0,  // spawn rect: x1/y1/x2/y2
    SPRITE_FLAGS_NULL);

OVERWRITE_SPRITE_RESOURCES_NULL(MOTION_CONTROL_PROMPT_MNG_SPRITE_NUM);


/* **************************************************************** */
/* *********** LIFT_ROTATION_FULL_COLOR (spr 362) hacks *********** */
/* **************************************************************** */

// If spritedata & 0x20000000, remove draw() function
kmBranchDefAsm(0x8089a600, 0x8089a604) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lwz r0, 0x4(r3)
    rlwinm. r0, r0, 3, 31, 31  // r0 = (r0 <<< 3) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    BACKDOOR_BLR
_OptionNotEnabled:
    // (Original instruction)
    stwu sp, -0x10(sp)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// If spritedata & 0x10000000, don't add 3 to width/height nybbles
// For visuals:
kmBranchDefAsm(0x8089a6b8, 0x8089a6c0) {
    // Check spritedata
    lwz r4, 0x4(r28)
    rlwinm. r4, r4, 4, 31, 31  // r4 = (r4 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Don't add 3 when moving r29/r30 to r4/r5)
    mr r4, r29
    mr r5, r30
    b _End
_OptionNotEnabled:
    // (Original code)
    addi r4, r29, 0x3
    addi r5, r30, 0x3
_End:
};
// For collision:
kmBranchDefAsm(0x8089a6cc, 0x8089a6d8) {
    // Check spritedata
    lwz r4, 0x4(r28)
    rlwinm. r4, r4, 4, 31, 31  // r4 = (r4 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Don't add 3 when moving r29/r30 to r4/r0)
    mr r4, r29
    mr r0, r30
    b _End
_OptionNotEnabled:
    // (Original code)
    addi r4, r29, 0x3
    addi r0, r30, 0x3
_End:
    // (Was originally between the two addi instructions)
    stw  r4, 0x14(sp)
};


/* **************************************************************** */
/* ***************** Rolling Hill (spr 212) hacks ***************** */
/* **************************************************************** */

/* 800daf70 */ extern "C" void addToList__13BasicColliderFv(void);
/* 800dafc0 */ extern "C" void removeFromList__13BasicColliderFv(void);

// If event_nybbles & 0x0001, remove draw() function
kmBranchDefAsm(0x807ef060, 0x807ef064) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lhz r0, 0x354(r3)
    andi. r0, r0, 0x1
    beq _OptionNotEnabled
_OptionEnabled:
    BACKDOOR_BLR
_OptionNotEnabled:
    // (Original instruction)
    stwu sp, -0x40(sp)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};

// If event_nybbles & 0x0002, remove collision
// (onCreate: start of function, just after the prologue)
kmBranchDefAsm(0x807eecfc, 0x807eed00) {
    nofralloc  // don't auto-generate a prologue
    // Check if this is the first or second time running onCreate
    lhz r0, 0x356(r3)
    cmpwi r0, 1
    bne _FirstTimeRunning

_SecondTimeRunning:
    // Check spritedata
    lhz r0, 0x354(r3)
    andi. r0, r0, 0x2
    bne _OptionEnabled
_OptionNotEnabled:
    // Call the collider-add function
    addi r3, r3, 0x3e4
    bl addToList__13BasicColliderFv
_OptionEnabled:
    // Epilogue and return 1
    lwz r0, 0x34(sp)
    mtspr lr, r0
    addi sp, sp, 0x30
    li r3, ONCREATE_RESULT_SUCCESS
    BACKDOOR_BLR

_FirstTimeRunning:
    // Write a "1" to a pad area so we know we ran once
    li r0, 1
    sth r0, 0x356(r3)
    // (Original instruction)
    stw r31, 0x2c(sp)
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};
// (onCreate: original call to addToList__13BasicColliderFv)
kmWrite32(0x807eeeac, 0x60000000);  // nop
// (onCreate: original li r3, 1 before the blr)
kmWrite32(0x807eef04, 0x38600000);  // li r3, 0 (ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME)
// (onDelete)
kmBranchDefAsm(0x807ef1d8, 0x807ef1dc) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lhz r0, 0x354(r31)
    andi. r0, r0, 0x2
    bne _OptionEnabled
_OptionNotEnabled:
    // (Original instruction)
    bl removeFromList__13BasicColliderFv
_OptionEnabled:
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};


/* **************************************************************** */
/* ****************** Toad block (spr 422) hacks ****************** */
/* **************************************************************** */

/* 800fd140 */ extern "C" bool FUN_P1_800fd140(void);
/* 80022080 */ extern "C" void player_set_15daEnBlockMain_cFii(void *this_, int a, int b);

// If settings & 0x20000000, block will always contain Toad no matter what
kmBranchDefAsm(0x809c3030, 0x809c3034) {
    nofralloc  // don't auto-generate a prologue
    // Check spritedata
    lwz r3, 0x4(r30)
    rlwinm. r3, r3, 3, 31, 31  // r3 = (r3 <<< 3) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    // (Just return true)
    li r3, 1
    b _End
_OptionNotEnabled:
    // (Original code)
    bl FUN_P1_800fd140
_End:
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};


void ToadBlock_DisappearWithBooLaugh(dStageActor_c *this_) {
    PlaySound(this_, SE_EMY_CS_TERESA_BEAT_YOU);
    S16Vec nullRot = {0, 0, 0};
    Vec oneVec = {1.0f, 1.0f, 1.0f};
    SpawnEffect("Wm_en_obakedoor", 0, &this_->pos, &nullRot, &oneVec);

    // event_nybbles & 0xff: optional event ID to trigger when the fake block is opened
    int event_id = this_->spriteFlagNum & 0xff;
    if (event_id) {
        dFlagMgr_c::instance->set(event_id - 1, 0, true, false, false);
    }

    this_->Delete(1);
};

// If settings & 0x10000000, instead of giving Toad, make the block disappear with boo laugh
kmCallDefAsm(0x809c39e8) {
    // Check spritedata
    lwz r0, 0x4(r31)
    rlwinm. r0, r0, 4, 31, 31  // r0 = (r0 <<< 4) & 1
    beq _OptionNotEnabled
_OptionEnabled:
    b ToadBlock_DisappearWithBooLaugh
_OptionNotEnabled:
    // (Original code)
    b player_set_15daEnBlockMain_cFii
};


/* **************************************************************** */
/* *********** Unused giant Wiimote door (spr 319) hacks ********** */
/* **************************************************************** */

/* 8007f7a0 */ extern "C" void __ct__7PhysicsFv(Physics *physics);
/* 8007f810 */ extern "C" void __dt__7PhysicsFv(Physics *physics);
/* 80162e90 */ extern "C" dActor_c *Actor_SearchByName(u32 actorID, dActor_c *after);

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


/* **************************************************************** */
/* **************** Rotation controller (149) hacks *************** */
/* **************************************************************** */

// This is the worst workaround

bool RotationController_IsActive(dStageActor_c *this_) {
    int disablingEventId = this_->currentLayerID;  // nybbles 15-16

    if (disablingEventId && dFlagMgr_c::instance->active(disablingEventId - 1)) {
        *(u16*)((u32)this_ + 0x104) = 0xC000;  // (this is actually 0, trust me)
        // force rotation to 0
        return false;
    }
    return true;
};

kmBranchDefAsm(0x80874968, 0x8087496c) {
    nofralloc  // don't auto-generate a prologue
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r31
    bl RotationController_IsActive
    cmpwi r3, 1
    bne _IsntActive
_IsActive:
    ASM_RESTORE_CONTEXT_FROM_STACK
    bctrl  // (original instruction)
    b _End
_IsntActive:
    ASM_RESTORE_CONTEXT_FROM_STACK
_End:
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};


/* **************************************************************** */
/* ********************** P-switch (41) hacks ********************* */
/* **************************************************************** */

// This is so stupid

// settings & 0x000000f0: event ID that prevents it from spawning
bool PSwitch_ShouldSpawn(dStageActor_c *this_) {
    int spawnPreventingEventId = (this_->settings >> 4) & 0xf;

    if (spawnPreventingEventId && dFlagMgr_c::instance->active(spawnPreventingEventId - 1)) {
        return false;
    }

    return true;
};

kmBranchDefAsm(0x80a197b4, 0x80a197b8) {
    nofralloc  // don't auto-generate a prologue
    mfspr r0, lr      // (original instructions)
    stw r0, 0x24(sp)  // (^)

    ASM_DUMP_CONTEXT_TO_STACK
    bl PSwitch_ShouldSpawn
    cmpwi r3, 1
    beq _ShouldSpawn
_ShouldntSpawn:
    ASM_RESTORE_CONTEXT_FROM_STACK
    // return 2 ("delete me pls")
    li r3, ONCREATE_RESULT_PLEASE_DELETE_ME
    // epilogue and blr
    lwz r0, 0x24(sp)
    mtspr lr, r0
    addi sp, sp, 0x20
    BACKDOOR_BLR
_ShouldSpawn:
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};


/* **************************************************************** */
/* ************* Rotation-controlled coin (253) hacks ************* */
/* **************************************************************** */


void RotationControlledCoin_ApplyDisplayAngleChange(dStageActor_c *this_) {
    s16 initialAngle = (this_->settings >> 16) & 0xffff;
    this_->rot.z += initialAngle;
};


kmBranchDefAsm(0x809e5828, 0x809e582c) {
    nofralloc  // don't auto-generate a prologue
    lwz r29, 0x44(sp)  // (original instruction)
    ASM_DUMP_CONTEXT_TO_STACK
    mr r3, r28
    bl RotationControlledCoin_ApplyDisplayAngleChange
    ASM_RESTORE_CONTEXT_FROM_STACK
    blr  // needed because we used nofralloc at the start -- also, Kamek will replace this with a branch to the target return address
};



/* **************************************************************** */
/* *************** Bugged rotation-controlled actors ************** */
/* **************************************************************** */

// (Copypasted from NSMBW-Updated)

// daEnLiftRotHalf_c (EN_LIFT_ROTATION_HALF: profile 481, sprite 107)
kmWrite32(0x80a5d980, 0x38a0c000);  // li r5, -0x4000

// daEnObjRotationBlock_c (EN_ROTATION_BLOCK: profile 529, sprite 252)
kmWrite32(0x80a7b558, 0x3860c000);  // li r3, -0x4000
kmWrite32(0x80a7b570, 0x3860c000);  // li r3, -0x4000

// daEnCoinAngle_c (EN_COIN_ANGLE: profile 530, sprite 253)
kmWrite32(0x809e55f4, 0x3860c000);  // li r3, -0x4000
kmWrite32(0x809e560c, 0x3860c000);  // li r3, -0x4000

// daEnBlockAngle_c (EN_BLOCK_HATENA_ANGLE: profile 532, sprite 255 & EN_BLOCK_RENGA_ANGLE: profile 533, sprite 256)
kmWrite32(0x809c15c4, 0x3860c000);  // li r3, -0x4000
kmWrite32(0x809c15dc, 0x3860c000);  // li r3, -0x4000



/* **************************************************************** */
/* ************************ HYPER_REMO_DOOR *********************** */
/* **************************************************************** */

// Retail daRemoDoor_c uses 0x400
// This is as high as I was able to make it before Mario starts clipping into it when it moves
#define DOOR_FLIP_SPEED 0x500

#define DIRECTION_NONE 0
#define DIRECTION_U 1
#define DIRECTION_R 2
#define DIRECTION_D 3
#define DIRECTION_L 4

// Replaces EN_HELPOS (actor 109, sprite 79)
#define HYPER_REMO_DOOR_PROFILE_NUM 109
#define HYPER_REMO_DOOR_SPRITE_NUM 79

/* 80056370 */ extern "C" void HurtMarioBecauseOfBeingSquashed(void *mario, dStageActor_c *squasher, int type);


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


/* **************************************************************** */
/* ******************* Universal Reconfigurator ******************* */
/* **************************************************************** */

// Values match the respective course block indices
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_AREA_SETTINGS 0
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_BOUNDS 2
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_BGA 4
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_BGB 5
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES 6
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ZONES 9
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS 10
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS 12
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES 13

// Set: set the field to the specified value
#define UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_SET 0
// Add: add the specified value to the field
#define UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_ADD 1

// Single: apply once and then never again (until the zone is reloaded, at least)
#define UNIVERSAL_RECONFIGURATOR_USAGE_MODE_SINGLE 0
// Multi: apply every time the event is activated
#define UNIVERSAL_RECONFIGURATOR_USAGE_MODE_MULTI 1
// Only While: same as Multi, but also restore the previous value every time the event is deactivated
#define UNIVERSAL_RECONFIGURATOR_USAGE_MODE_ONLY_WHILE 2

class daUniversalReconfigurator_c : public dStageActor_c {
public:

    u8 itemId;
    u8 applicationMode : 1;
    u8 eventId : 7;
    u8 itemSubId;
    u8 fieldId;
    s16 fieldValue;
    s16 storedFieldValue;
    u8 itemType        : 4;
    u8 isActive        : 1;
    u8 isDoneForever   : 1;
    u8 usageMode       : 2;
    u8 isSecondTimeRunningOnCreate : 1;
    u8 areaId : 3;

    void *itemPtr;

    static daUniversalReconfigurator_c *build();

    dCourse_c::rail_s *getPathFromCourse(dCourse_c *course, int pathId);
    bool populateItemPtr();
    s32 getEntranceFieldValue();
    void setEntranceFieldValue(s32 value);
    s32 getLocationFieldValue();
    void setLocationFieldValue(s32 value);
    s32 getPathFieldValue();
    void setPathFieldValue(s32 value);
    s32 getPathNodeFieldValue();
    void setPathNodeFieldValue(s32 value);
    s32 getFieldValue();
    void setFieldValue(s32 value);

    int onCreate();
    int onExecute();
};

daUniversalReconfigurator_c *daUniversalReconfigurator_c::build() {
    return new(AllocFromGameHeap1(sizeof(daUniversalReconfigurator_c))) daUniversalReconfigurator_c;
};

int daUniversalReconfigurator_c::onCreate() {
    // Run onCreate twice so we get access to all spritedata nybbles
    if (!isSecondTimeRunningOnCreate) {
        isSecondTimeRunningOnCreate = true;
        return ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME;
    }

    // Spritedata:
    // ABCC DDEE FFFF __GG
    // A = item type
    // B =...
    //     value / 3: area ID (0, 1, 2, 3, 4)
    //     value % 3: usage mode (0, 1, 2)
    // CC =...
    //     Top bit: application mode (0, 1)
    //     Lower 7 bits: event ID
    // DD = item ID
    // EE = field ID
    // FFFF = field value
    // GG = item sub-ID (mainly for path nodes)

    // Note: you could also free up the second bit in nybble 3 if you rename event 64
    // to "event 0," since 0 isn't a meaningful event ID value for this actor anyway

    itemType = spriteFlagNum >> 12;
    int nybble2 = (spriteFlagNum >> 8) & 0xf;
    areaId = nybble2 / 3;
    usageMode = nybble2 % 3;
    applicationMode = (spriteFlagNum >> 7) & 1;
    eventId = spriteFlagNum & 0x7f;
    itemId = settings >> 24;
    fieldId = (settings >> 16) & 0xff;
    fieldValue = settings & 0xffff;
    itemSubId = currentLayerID;

    if (areaId > 4) areaId = 4;

    storedFieldValue = 0;
    isActive = false;
    isDoneForever = false;

    if (!populateItemPtr())
        return ONCREATE_RESULT_PLEASE_DELETE_ME;

    return ONCREATE_RESULT_SUCCESS;
};

#define PATHS_BLOCK_INDEX 12
dCourse_c::rail_s *daUniversalReconfigurator_c::getPathFromCourse(dCourse_c *course, int pathId) {
    for (int i = 0; i < course->blockCounts[PATHS_BLOCK_INDEX]; i++) {
        dCourse_c::rail_s *path = (dCourse_c::rail_s*)(course->blocks[PATHS_BLOCK_INDEX]);
        if (path->entryID == pathId)
            return path;
    }
    return NULL;
};

bool daUniversalReconfigurator_c::populateItemPtr() {
    dCourse_c *course = dCourseFull_c::instance->get((areaId == 0) ? GetAreaNum() : (areaId - 1));
    dCourse_c::rail_s *tmp_path;

    switch (itemType) {
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES:
            itemPtr = course->getNextGotoByID(itemId);
            break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS:
            itemPtr = course->getRectByID(itemId);
            break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS:
            itemPtr = getPathFromCourse(course, itemId);
            break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES:
            tmp_path = getPathFromCourse(course, itemId);
            if (tmp_path == NULL) return false;
            if (itemSubId >= tmp_path->nodeCount) return false;
            itemPtr = &course->railNode[tmp_path->startNode + itemSubId];
            break;
        default:
            OSReport("Universal Reconfigurator: unsupported item type: %d\n", itemType);
            return false;
    }

    return itemPtr != NULL;
}

s32 daUniversalReconfigurator_c::getEntranceFieldValue() {
    dCourse_c::nextGoto_s *entrance = (dCourse_c::nextGoto_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0:  return entrance->xPos;
            case 1:  return entrance->yPos;
            case 2:  return entrance->cameraXPos;
            case 3:  return entrance->cameraYPos;
            case 4:  return entrance->entryID;
            case 5:  return entrance->destArea;
            case 6:  return entrance->destEntrance;
            case 7:  return entrance->type;
            case 8:  return entrance->unk1;
            case 9:  return entrance->zoneID;
            case 10: return entrance->layerID;
            case 11: return entrance->pathID;
            case 12: return entrance->unk2;
            default: return 0;
        }

    } else {
        int flagId = fieldId & 0xf;
        return (entrance->flags >> (15 - flagId)) & 1;
    }
};

void daUniversalReconfigurator_c::setEntranceFieldValue(s32 value) {
    dCourse_c::nextGoto_s *entrance = (dCourse_c::nextGoto_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0:  entrance->xPos = value; break;
            case 1:  entrance->yPos = value; break;
            case 2:  entrance->cameraXPos = value; break;
            case 3:  entrance->cameraYPos = value; break;
            case 4:  entrance->entryID = value; break;
            case 5:  entrance->destArea = value; break;
            case 6:  entrance->destEntrance = value; break;
            case 7:  entrance->type = value; break;
            case 8:  entrance->unk1 = value; break;
            case 9:  entrance->zoneID = value; break;
            case 10: entrance->layerID = value; break;
            case 11: entrance->pathID = value; break;
            case 12: entrance->unk2 = value; break;
        }

    } else {
        int flagId = fieldId & 0xf;
        u16 mask = (u16)1 << (15 - flagId);

        if (value == 0) {
            entrance->flags &= ~mask;
        } else {
            entrance->flags |= mask;
        }
    }
};

s32 daUniversalReconfigurator_c::getLocationFieldValue() {
    dCourse_c::rect_s *location = (dCourse_c::rect_s*)itemPtr;

    switch (fieldId) {
        case 0:  return location->xPos;
        case 1:  return location->yPos;
        case 2:  return location->xSize;
        case 3:  return location->ySize;
        case 4:  return location->entryID;
        case 5:  return location->unused[0];
        case 6:  return location->unused[1];
        case 7:  return location->unused[2];
        default: return 0;
    }
};

void daUniversalReconfigurator_c::setLocationFieldValue(s32 value) {
    dCourse_c::rect_s *location = (dCourse_c::rect_s*)itemPtr;

    switch (fieldId) {
        case 0: location->xPos = value; break;
        case 1: location->yPos = value; break;
        case 2: location->xSize = value; break;
        case 3: location->ySize = value; break;
        case 4: location->entryID = value; break;
        case 5: location->unused[0] = value; break;
        case 6: location->unused[1] = value; break;
        case 7: location->unused[2] = value; break;
    }
};

s32 daUniversalReconfigurator_c::getPathFieldValue() {
    dCourse_c::rail_s *path = (dCourse_c::rail_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0:  return path->entryID;
            case 1:  return path->unknown;
            case 2:  return path->startNode;
            case 3:  return path->nodeCount;
            default: return 0;
        }

    } else {
        int flagId = fieldId & 0xf;
        return (path->flags >> (15 - flagId)) & 1;
    }
};

void daUniversalReconfigurator_c::setPathFieldValue(s32 value) {
    dCourse_c::rail_s *path = (dCourse_c::rail_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0: path->entryID = value; break;
            case 1: path->unknown = value; break;
            case 2: path->startNode = value; break;
            case 3: path->nodeCount = value; break;
        }

    } else {
        int flagId = fieldId & 0xf;
        u16 mask = (u16)1 << (15 - flagId);

        if (value == 0) {
            path->flags &= ~mask;
        } else {
            path->flags |= mask;
        }
    }
};

s32 daUniversalReconfigurator_c::getPathNodeFieldValue() {
    dCourse_c::railNode_s *pathNode = (dCourse_c::railNode_s*)itemPtr;

    switch (fieldId) {
        case 0:  return pathNode->xPos;
        case 1:  return pathNode->yPos;
        case 2:  return ((u16*)(&pathNode->speed))[0];
        case 3:  return ((u16*)(&pathNode->speed))[1];
        case 4:  return ((u16*)(&pathNode->accel))[0];
        case 5:  return ((u16*)(&pathNode->accel))[1];
        case 6:  return pathNode->unk1;
        case 7:  return pathNode->unk2;
        default: return 0;
    }
};

void daUniversalReconfigurator_c::setPathNodeFieldValue(s32 value) {
    dCourse_c::railNode_s *pathNode = (dCourse_c::railNode_s*)itemPtr;

    switch (fieldId) {
        case 0: pathNode->xPos = value; break;
        case 1: pathNode->yPos = value; break;
        case 2: ((u16*)(&pathNode->speed))[0] = value; break;
        case 3: ((u16*)(&pathNode->speed))[1] = value; break;
        case 4: ((u16*)(&pathNode->accel))[0] = value; break;
        case 5: ((u16*)(&pathNode->accel))[1] = value; break;
        case 6: pathNode->unk1 = value; break;
        case 7: pathNode->unk2 = value; break;
    }
};

s32 daUniversalReconfigurator_c::getFieldValue() {
    switch (itemType) {
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES: return getEntranceFieldValue();
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS: return getLocationFieldValue();
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS: return getPathFieldValue();
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES: return getPathNodeFieldValue();
        default: return 0;
    }
};

void daUniversalReconfigurator_c::setFieldValue(s32 value) {
    switch (itemType) {
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES: setEntranceFieldValue(value); break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS: setLocationFieldValue(value); break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS: setPathFieldValue(value); break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES: setPathNodeFieldValue(value); break;
    }
};

int daUniversalReconfigurator_c::onExecute() {
    if (isDoneForever)
        return 1;

    bool nowActive = dFlagMgr_c::instance->active(eventId - 1);

    if (nowActive != isActive) {
        // Handle inactive -> active and active -> inactive separately
        if (!isActive && nowActive) {
            // Event was just activated

            storedFieldValue = getFieldValue();
            if (applicationMode == UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_SET)
                setFieldValue(fieldValue);
            else if (applicationMode == UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_ADD)
                setFieldValue(storedFieldValue + fieldValue);

            // In single-use mode? Disable ourselves forever
            if (usageMode == UNIVERSAL_RECONFIGURATOR_USAGE_MODE_SINGLE)
                isDoneForever = true;

        } else if (isActive && !nowActive && usageMode == UNIVERSAL_RECONFIGURATOR_USAGE_MODE_ONLY_WHILE) {
            // Event was just deactivated, and we're on "only while" mode.
            // Restore the previous value
            setFieldValue(storedFieldValue);
            storedFieldValue = 0;
        }

        isActive = nowActive;
    }

    return 1;
};


// Replaces EN_PUKUCOIN (actor 162, sprite 116)
#define UNIVERSAL_RECONFIGURATOR_PROFILE_NUM 162
#define UNIVERSAL_RECONFIGURATOR_SPRITE_NUM 116

OVERWRITE_PROFILE(UNIVERSAL_RECONFIGURATOR, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM,
    &daUniversalReconfigurator_c::build,
    UNIVERSAL_RECONFIGURATOR_PROFILE_NUM, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM, 0);

OVERWRITE_SPRITE_DATA(UNIVERSAL_RECONFIGURATOR_SPRITE_NUM, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM,
    0, 0,        // spawn offset: x/y
    0, 0, 0, 0,  // spawn rect: x1/y1/x2/y2
    SPRITE_FLAGS_ALWAYS_SPAWNED);

OVERWRITE_SPRITE_RESOURCES_NULL(UNIVERSAL_RECONFIGURATOR_SPRITE_NUM);
