## AC_LIFT_REMOCON_SEESAW analysis
*(as it relates to fukidashi stuff)*

Profile at `80968030`, build func at `8083ebc0`, vtable at `80968094`.
Also, a member class: at 0x454, vtable `80968178`.

Functions:

    8083ebc0 build (profile 460 (AC_LIFT_REMOCON_SEESAW), sprite 51)
    8083ebf0 build (profile 509 (AC_LIFT_BOSS_REMOCON_SEESAW), sprite 190)
    8083ec20                     (shared build code between the above)
    -- vtable-accessible stuff
    8083ecf0 class_80968178::vf_08()
    8083ed30 daLiftRemoconSeesaw_c::create()
    8083ee80 daLiftRemoconSeesaw_c::execute()
    8083ef10 daLiftRemoconSeesaw_c::draw()
    8083ef40 daLiftRemoconSeesaw_c::doDelete()
    8083ef70 class_80968178::vf_0c()
    8083f110 daLiftRemoconSeesaw_c::calcMdl() (probably)
    8083f190 daLiftRemoconSeesaw_c::createMdl() (probably)
    8083f350 (xref: 8083f3b0)    (just spawns "Wm_en_burst_s" particle effect)
    8083f3b0 (xref: execute)     (updates graphics in response to a control-player change)
                                 (could be useful to see how this thing detects player-control changes)
    -- PTMFs and functions reachable from them
    8083f4f0 (xref: 8083f570)    (just retrieves and clamps tilt amount)
    8083f550 (xref: 8083f570, 8083fa10) just retrieves tilt amount)
    8083f570 (xref: 8083fa10)    (very long, does some sfx, does some fukidashi. if blr'd, no tilting occurs.)
    8083f820 (xref: 8083fa10)    (just calls dQuake_c::_m_instance::hashname_05a0ae8c_9c80a82b(...))
    8083f850 (xref: 8083fa10)    (short, plays a sfx, does a fukidashi. if blr'd, fukidashi never ends)
    8083f930 (xref: 8083fa10)    (medium length, seems exclusively about fukidashi. if blr'd, no fukidashi!)
    8083f9b0 (PTMF)              (just calls dRide_ctr_c::entry())
    8083fa00 (PTMF)              (nullsub)
    8083fa10 (PTMF)              (lots of stuff! some calls to unknown functions. if blr'd, you can't gain control of the platform.)
    -- more boilerplate
    8083fb80 daLiftRemoconSeesaw_c::~daLiftRemoconSeesaw_c()
    -- states things
    8083fc30 __sinit_\d_lift_remocon_seesaw_cpp
    8083fd40 sFStateVirtualID_c<daLiftRemoconSeesaw_c>::~sFStateVirtualID_c()
    8083fda0 sFStateID_c<daLiftRemoconSeesaw_c>::isSameName(const char*) const
    8083fe30 sFStateID_c<daLiftRemoconSeesaw_c>::initializeState(daLiftRemoconSeesaw_c&) const
    8083fe60 sFStateID_c<daLiftRemoconSeesaw_c>::executeState(daLiftRemoconSeesaw_c&) const
    8083fe90 sFStateID_c<daLiftRemoconSeesaw_c>::finalizeState(daLiftRemoconSeesaw_c&) const

There are two ways to end fukidashi, empirically:
* exit the platform (duh)
* if you tilt to the maximum angle 8 times, the prompt goes away permanently

Every obvious fukidashi-related function call:
* `FUN_8083f570`/`8083f714` -> `hashname_5edaa1df_bf88549a_gameCom_setFukidashi234`
* `FUN_8083f850`/`8083f8c0` -> `hashname_9723880b_dea8588e_gameCom_setFukidashiAll234`
* `FUN_8083f930`/`8083f9a4` -> `hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi`

So... in summary:
* `hashname_5edaa1df_bf88549a_gameCom_setFukidashi234`: is used to end fukidashi by tilting a lot
    * Called after the eighth time reaching maximum tilt
* `hashname_9723880b_dea8588e_gameCom_setFukidashiAll234`: is used to end fukidashi by platform exit
    * Fukidashi can be re-triggered later
* `hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi`: is used to begin fukidashi

`hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi`:
* r3: player number?
* r4: zero for AC_LIFT_REMOCON_SEESAW, other values depending on the actor that calls it

So. An event-controlled fukidashi sprite should work like this:

    When event is detected:
        For each player you want to activate fukidashi for:
            Call hashname_b6f64cb2_f0c70be1_gameCom_doFukidashi([no "this",] player_num, type_num)
    When event is deactivated:
        For each player you want to deactivate fukidashi for:
            Call either:
                hashname_9723880b_dea8588e_gameCom_setFukidashiAll234(player_num, type_num, 0x14)
                hashname_5edaa1df_bf88549a_gameCom_setFukidashi234(player_num, type_num, 0x14)
                Third argument is often 0, but AC_LIFT_REMOCON_SEESAW uses 0x14
