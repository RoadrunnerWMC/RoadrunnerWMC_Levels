# daEnCoinAngle_c (EN_COIN_ANGLE)

*(and other rotation-controlled actors with broken Starting Rotations)*

Class header pseudocode:

    class daEnCoinAngle_c {

        // [for sway controller]
        /* ad0 */ float yOffsetFromController;
        /* adc */ float xOffsetFromController;

        // [for spin controller]
        /* ade */ s16 theControllersZRotation;

        // [for both]
        /* ae0 */ u8 isUsingSpinController;
        /* ae4 */ u32 foundRotationControllerActorID;

        // [for spin controller]
        /* ae8 */ float distanceFromController;
        /* af0 */ s16 rotationDistance;  // (copied from controller)
        /* af2 */ s16 initialAngleFromController;  // (takes controller's initial angle into account)
        /* af4 */ u8 rotationID;


        /* 809e4e20 */ build(void);
        /* 809e4fa0 */ create(void) override;
        /* 809e51e0 */ execute(void) override;
        /* 809e5290 */ draw(void) override;
        /* 809e52a0 */ delete(void) override;
        /* 809e5300 */ setDeathInfo_Quake(int) override;
        /* 809e5390 */ updatePanelObjList(void);  // unofficial name
        /* 809e54b0 */ searchParent(void);
        /* 809e5550 */ setParentInfo(dStageActor_c *rotcon);
        /* 809e5660 */ rotation_move(void);
        /* 809e5840 */ initializeState_SearchID(void);
        /* 809e5850 */ finalizeState_SearchID(void);
        /* 809e5860 */ executeState_SearchID(void);
        /* 809e58c0 */ initializeState_WaitDown(void);
        /* 809e5950 */ finalizeState_WaitDown(void);
        /* 809e5960 */ executeState_WaitDown(void);
        /* 809e5a50 */ _vf64(void) override;  // unknown name
        /* 809e5a60 */ checkDispIn(void) override;
        /* 809e5a70 */ isQuakeDamage(void) override;
        /* 809e5a80 */ ~daEnCoinAngle_c(void);
        /* 809e5c30 */ __sinit_\da_en_coin_angle_cpp(void);

        /* 80ae260c */ __vt;
        /* 80ae2600 */ g_profile_EN_COIN_ANGLE;
        /* 80b13a08 */ StateID_SearchID;
        /* 80b13a48 */ StateID_WaitDown;
    }

State transition diagram:

    (start)
        -> StateID_SearchID
        -> daEnObjCoinBlock_c::StateID_CoinWait
    StateID_SearchID
        -> daEnObjCoinBlock_c::StateID_CoinWait
    (any call to setDeathInfo_Quake)
        -> StateID_WaitDown

Broken rotation controlled sprites:
* 107 (EN_LIFT_ROTATION_HALF)
* 252 (EN_ROTATION_BLOCK)
* 253 (EN_COIN_ANGLE)
* 255 (EN_BLOCK_HATENA_ANGLE)
* 256 (EN_BLOCK_RENGA_ANGLE)

A correct implementation of updating oneself from a rotation controller is `80062520` (`daRotObjsBase_c::setParentInfo(u32)`)

`80874a50` is a function on the rotation controller to get the starting rotation value.
Every bugged actor calls this and no others do, so to fix them, just follow every xref and eliminate the call to it.
(Or make the function always return a constant value, but that feels hackier.)

* `80a5d690` is daEnLiftRotHalf_c's searchParent
* `80a5d7f0` is daEnLiftRotHalf_c's rotation_move
* `80a5d8e0` is daEnLiftRotHalf_c's setParentInfo
