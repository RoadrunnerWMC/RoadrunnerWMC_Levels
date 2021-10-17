# daRemoDoor_c (REMO_DOOR)

    809789c8 g_profile_REMO_DOOR  (profile 208, sprite 319)
    80978a00 daRemoDoor_c::__vt

    808958e0 daRemoDoor_c::build(void)
    80895950 daRemoDoor_c::createMdl(void) (probably)
    80895a00 daRemoDoor_c::create(void)
    80895aa0 daRemoDoor_c::execute(void)
    80895b00 daRemoDoor_c::draw(void)
    80895bb0 daRemoDoor_c::initializeState_OpenReady(void)
    80895bc0 daRemoDoor_c::finalizeState_OpenReady(void)
    80895bd0 daRemoDoor_c::executeState_OpenReady(void) (reads remote tilt input)
    80895c50 daRemoDoor_c::initializeState_Open(void)   (plays open sfx)
    80895ca0 daRemoDoor_c::finalizeState_Open(void)
    80895cb0 daRemoDoor_c::executeState_Open(void)      (reads remote tilt input)
    80895d50 daRemoDoor_c::initializeState_Close(void)
    80895d60 daRemoDoor_c::finalizeState_Close(void)    (plays close sfx)
    80895db0 daRemoDoor_c::executeState_Close(void)
    80895df0 daRemoDoor_c::~daRemoDoor_c(void)
    80895e70 __sinit_\d_remo_door_cpp
    808960f0 sFStateVirtualID_c<daRemoDoor_c>::~sFStateVirtualID_c(void)
    80896150 sFStateID_c<daRemoDoor_c>::isSameName(const char*) const
    808961e0 sFStateID_c<daRemoDoor_c>::initializeState(daRemoDoor_c&) const
    80896210 sFStateID_c<daRemoDoor_c>::executeState(daRemoDoor_c&) const
    80896240 sFStateID_c<daRemoDoor_c>::finalizeState(daRemoDoor_c&) const

    8099e598 daRemoDoor_c::StateID_OpenReady
    8099e5d8 daRemoDoor_c::StateID_Open
    8099e618 daRemoDoor_c::StateID_Close

`initializeState_OpenReady()` manual decompilation:

    /* 80429d54 */ extern "C" int LeadingPlayerNumber_maybe;
    /* 8042a230 */ extern "C" void **ptrToRemoconMng;  // ptr to {u32 _, void *P1, void *P2, void *P3, void *P4}
    /* 800b5ca0 */ extern "C" u32 shiftedTiltAmount__7RemoconFv(Remocon *this_);
    void daRemoDoor_c::initializeState_OpenReady(void) {
        if (LeadingPlayerNumber_maybe < 4) {
            s16 sta = GetRemoconMng()->controllers[LeadingPlayerNumber_maybe]->shiftedTiltAmount() >> 16;
            if (sta >= 0x2000) {
                acState.setState(&StateID_Open);
            }
        }
    }
