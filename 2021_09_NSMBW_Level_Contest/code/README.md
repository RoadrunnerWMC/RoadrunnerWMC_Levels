# Custom Code for "Indoors"

This level has much more custom code than I originally planned on writing. But most of it is custom sprites and new settings for existing sprites, which can be reused in other levels -- so it was definitely worth it! It also served as a catalyst for some much-needed bugfixes and improvements to Kamek itself, since version 2.0 unfortunately hasn't seen very much real-world usage yet despite being six years old now.

## Setup

The custom code is built with [Kamek 2.0](https://github.com/Treeki/Kamek/) (also known as "C# Kamek", as Kamek 1.0 was written in Python).

* If using an OS other than Windows, install Wine (since CodeWarrior is an EXE).
* Clone the Kamek repo in the current directory (so that there's a "Kamek" subdirectory within this one).
* Obtain CodeWarrior as described in the Kamek documentation.
    * Put it in a "cw" directory of the Kamek repo, which is the convention used by Kamek's included examples.
* Build Kamek (see its own documentation for details).
* Make a small change to the Kamek loader: in "Kamek/loader/nsmbw.cpp", replace `"/engine.%c%d.bin"` with `"/Code/%c%d.bin"`.
* Build the Kamek loader (again, see its own documentation for details).
    * "loader.bin" and "loader.xml" should now exist in Kamek's "loader" directory.
* Open "Kamek/k_stdlib/kamek.h" and comment out these two lines near the bottom:

```
#include "base/c_stdlib.h"
#include "base/rvl_sdk.h"
```

## Building

First, prepare the loader.bin and Riivolution XML files by running

    python3 prepare_loader.py

This will create a `dist_loader` directory containing those two files. You only
need to run this once, unless you modify the loader itself.

To build the actual code, run

    python3 build_code.py

Output is placed in the `dist` directory.

## New sprites

### "Hyper" Giant Tilt-Controlled Door (mapped to sprite 79)

This is the fancy red door that serves as the main mechanic in the second half of the level.

* Nybble 5: z-order (higher values = farther behind)
* Nybble 10: primary direction (0-3 = up/right/down/left)
* Nybble 11: number of allowed steps in the primary direction
* Nybble 12: number of allowed steps in the reverse direction

### Universal Reconfigurator (mapped to sprite 116)

This is a really broadly useful sprite that basically lets you edit level data on the fly. It's used in this level to redirect the destination of the return door from the star coin 3 area directly back to the main zone after you collect the star coin. Initially that was all I planned to have it be able to do, but I decided to make it more general since there's much more you could potentially do with this.

The idea is that you specify a thing (for example, "path 8 node 5 in the current area"), a field in that thing ("x position"), a triggering event ID ("3"), and a new value ("576") that you want to set it to when the event is triggered.

The spritedata's pretty tightly packed. I don't think nybble 2's format can even be properly represented in current versions of Reggie.

* Nybble 1: item type (0 = area settings, 2 = zone bounds, 4 = bgA, 5 = bgB, 6 = entrances, 9 = zones, 10 = locations, 12 = paths, 13 = path nodes)
* Nybble 2, floor-div by 3: area ID to search for the item in (0 = current)
* Nybble 2, modulo 3: usage mode (0 = single, 1 = multi, 2 = "only while active")
* Nybble 3, mask 8: "application mode" (0 = overwrite value, 1 = add to previous value)
* Nybbles 3-4, lower 7 bits: triggering event ID
* Nybbles 5-6: item ID (entrance ID, zone ID, location ID, etc.)
* Nybbles 7-8: field ID (depends on the item type -- see below)
* Nybbles 9-12: field value
* Nybbles 15-16: item "sub ID" (represents path node ID when using the "path node" item type)

#### Field IDs: entrances

* 0: x position
* 1: y position
* 2: camera x position (?)
* 3: camera y position (?)
* 4: entrance ID
* 5: dest. area
* 6: dest. ID
* 7: type
* 8: pad byte 0xc
* 9: zone ID
* 10: layer
* 11: connected pipe path ID
* 12: connected pipe direction
* 16-31: flags, from 0x8000 to 0x0001

#### Field IDs: locations

* 0: x position
* 1: y position
* 2: width
* 3: height
* 4: location ID
* 5: pad byte 0x9
* 6: pad byte 0xa
* 7: pad byte 0xb

#### Field IDs: paths

* 0: path ID
* 1: pad byte 0x1
* 2: start node index
* 3: node count
* 16-31: flags, from 0x8000 to 0x0001

#### Field IDs: path nodes

* 0: x position
* 1: y position
* 2: speed (upper 16 bits)
* 3: speed (lower 16 bits)
* 4: acceleration (upper 16 bits)
* 5: acceleration (lower 16 bits)
* 6: delay
* 7: pad u16 at 0xe

Fields for other item types aren't supported yet in this version of the sprite, unfortunately.

### Mass Door Renderer (mapped to sprite 250)

This is used to draw almost all of the static doors used for the level terrain. You could try to use regular doors instead, but they lag the console pretty hard with the amount that would be on-screen simultaneously. Tileset objects are also a bad solution because they're a lot blurrier than sprite doors (especially in HD), which looks pretty bad.

The sprite renders a door (using `obj_door_flat.arc`, which is a high-quality flat image of a door, instead of the regular fully-modeled `obj_door.arc`) at each node of a specified path. The rotation of each door is controlled by each path node's "delay" setting (0-3 = up/down/left/right).

* Nybbles 11-12: path ID

### Motion-Control Input Prompt Controller (mapped to sprite 244)

You know the little Wii-remote animation that appears over Mario's head when the game wants you to tilt or shake the remote? The game calls that a "fukidashi" (Japanese for "speech bubble"). I'm going to call it a "motion-control input prompt", or MCIP.

This sprite lets you activate/deactivate MCIPs using events.

NSMBW gives each unique use case for MCIPs (different actors, for the most part) its own hardcoded ID, probably so that it can sensibly prioritize them when multiple things want to display them at once. For example, MCIP ID 5 is for POW blocks, and 10 is for airship bolts. There are 22 valid IDs in total, and the (almost) full table can be found below

Since you can't easily add new MCIP IDs for custom sprites, you should pick one for a sprite that you know isn't used elsewhere in your level. For example, the tilt-controlled doors in this level use the ID (0) officially assigned to the Tilt-Controlled Girder sprite (51), because their prompt is a "tilt" one and none of those girders are in this level.

* Nybbles 3-4: triggering event ID
* Nybble 7, mask 1: single-use only
* Nybble 8, mask 1: apply to player 1
* Nybble 8, mask 2: apply to player 2
* Nybble 8, mask 4: apply to player 3
* Nybble 8, mask 8: apply to player 4
* Nybbles 9-10: the third parameter to the retail functions used to cancel the motion-control input prompt (exact purpose is unknown)
* Nybbles 11-12: MCIP ID

#### Motion-Control Input Prompt IDs

Note: this table is a property of the retail game, and not something invented for the custom sprite.

| ID | Assigned to                             | Animation                 |
|----|-----------------------------------------|---------------------------|
| 0  | daLiftRemoconSeesaw_c (sprites 51, 190) | Tilt                      |
| 1  | dAcPy_c (for propeller suit?) [1]       | (just blinks like crazy?) |
| 2  | daJrClownForPlayer_c                    | Shake                     |
| 3  | daPropBlock_c (sprite 393) [1]          | "(1) + [shake]"           |
| 4  | daLightBlock_c (sprite 391)             | "(1) + [shake]"           |
| 5  | daPowBlock_c (sprite 386)               | "(1) + [shake]"           |
| 6  | daBlockTaru_c (sprite 388)              | "(1) + [shake]"           |
| 7  | daSlideBlock_c (sprite 385)             | "(1) + [shake]"           |
| 8  | dAcPy_c (for propeller suit?) [2]       | (nothing at all?)         |
| 9  | daLiftSpinRotation_c (sprites 172, 382) | Shake                     |
| 10 | OBJ_SPIN_PARENT (sprite 315) (bolt)     | Shake                     |
| 11 | dActorMng_c (huh??)                     | Tilt                      |
| 12 | daRemoconBalance_c (sprite 56)          | Tilt                      |
| 13 | daLiftRemoconXline_c (sprite 59)        | Tilt                      |
| 14 | daWireRemocon_c (sprite 267)            | Tilt                      |
| 15 | daEnRemoconCannon_c (sprite 202)        | Tilt                      |
| 16 | daLineSpinLift_c (sprite 327)           | Shake                     |
| 17 | daIce_c                                 | "(1) + [shake]"           |
| 18 | daEnJumpdai_c (sprite 148)              | "(1) + [shake]"           |
| 19 | daPropBlock_c (sprite 393) [2]          | Shake                     |
| 20 | daEnHatenaBalloon_c (sprite 171)        | Shake                     |
| 21 | daNiceBoat_c (sprite 368)               | Tilt                      |

*(21 is the highest valid ID, according to multiple pieces of evidence in the code)*

TODO: one of these is probably Rescuable Toad, but I'm not sure which one...

## New sprite settings

All of these are set up so that value 0 produces the original retail behavior.

### P-Switch (41)

* Nybble 11: event ID that prevents it from spawning

### Rotation Controller (Spinning) (149)

* Nybbles 15-16: disabling event ID (locks it to angle 0)

### Rolling Hill (212)

* Nybble 4, mask 1: invisible
* Nybble 4, mask 2: collisionless

### Rotation-Controlled Coin (253)

* Nybbles 5-8: rotation angle adjustment

### Door (259, 276)

* Nybble 5, mask 1: non-enterable
* Nybble 6: z-order (0 = retail z-order, 1+ = farther behind)

### Arrow Signboard (310)

* Nybbles 5-8: rotation angle

### Unused Giant Tilt-Controlled Door (319)

* Nybble 5: z-order (0 = retail z-order, 1+ = same as for the corresponding hack on sprite 259)
* Nybble 6, mask 1: solid
* Nybble 6, mask 2: make a checkpoint flag placed on the door horizontally follow the door's center

### Boo Circle (323)

* Nybble 5: initial rotation angle

### Rotation-Controlled Colored Box (362)

* Nybble 5, mask 1: width/height values are used directly, rather than as "value + 3"
* Nybble 5, mask 2: invisible

### "?" Block with Toad (422)

* Nybbles 3-4: target event ID (triggered when block is opened, in "fake block" mode)
* Nybble 5, mask 1: "fake" block (disappears with Boo laugh instead of giving you Toad)
* Nybble 5, mask 2: block always contains Toad in single-player mode, regardless of world map status

## Globally applied hacks

These can only be disabled by removing them from the code.

* *(Copypasted from [the NSMBW-Updated project](https://github.com/RoadrunnerWMC/NSMBW-Updated))* Hack to fix bugged "starting rotation" settings on various rotation-controller sprites (107, 252, 253, 255, 256).
* Hack to disable the hardcoded `bgA_0104.arc` from being used in the World 1-1 level slot.
* Hack to edit the fourth zoom level in the "normal" camera mode zoom-levels list to be 24 tiles tall instead of 20. The reasoning behind this is explained in comments in the code.
* *(Commented out and unused)* Hack to disable the initial zoom-out that occurs a few seconds after you launch a level.
* Hack to allow the unused giant tilt-controlled door sprite (319) to spawn already in the "opened" state if the remote happens to be tilted that way.
* Hack to improve the unused giant tilt-controlled door sprite (319)'s buggy spawning behavior.
* Hack to make the unused giant tilt-controlled door sprite (319) use `obj_remo_door.arc` instead of `obj_door.arc`.

## Utilities

### Custom sprite macros

Custom macros to make it easy to replace an existing sprite with a custom one. You can see usage examples in the code.

* `OVERWRITE_PROFILE(profileName, profileId, buildFunc, a, b, c)`: overwrites profile entry `profileId` with new data. `profileName` is used as part of the symbol name for the generated static profile struct, `buildFunc` is a pointer to the new process's `build` function, and `a`/`b`/`c` are the three currently-undocumented ints (u16, u16, u32) that follow the build function pointer in the profile struct (they may be related to the order in which processes are updated on each frame).
* `OVERWRITE_SPRITE_DATA(spriteId, profileId, xOff, yOff, rectX1, rectY1, rectX2, rectY2, flags)`: overwrites sprite entry `spriteId` with new data. `profileId` is the ID of the profile to spawn, and the other parameters relate to spawning behavior.
* `OVERWRITE_SPRITE_RESOURCES(profileName, spriteId, ...)`: overwrites sprite-resources entry `spriteId` with new data. Provide a list of resource names (arc filenames in the Object folder, without the ".arc" extension) that the game should load in levels where this sprite is present.
* `OVERWRITE_SPRITE_RESOURCES_NULL(spriteId)`: overwrites sprite-resources entry `spriteId` with a null pointer, indicating that the sprite needs no arcs to be loaded. This is different from using `OVERWRITE_SPRITE_RESOURCES` with no filenames specified because that creates a pointer to an empty array, instead of a null pointer.
* `SPRITE_FLAGS_ALWAYS_SPAWNED`, `SPRITE_FLAGS_UNK_8`: some commonly used values for the "flags" field in sprite data structs (can be combined with `|`). `SPRITE_FLAGS_NULL` is just 0.

### Assembly helpers

* Custom assembly macros `ASM_DUMP_CONTEXT_TO_STACK` and `ASM_RESTORE_CONTEXT_FROM_STACK`, to help with safely hooking new code at a location without accidentally modifying processor state.
* Custom assembly macro `BACKDOOR_BLR`, which `build_code.py` detects in the output files and replaces with a `blr` instruction. This is useful for working around Kamek safeguards against using `blr` in certain contexts where it thinks you shouldn't be (but you can safely if you know what you're doing).

### Other helpers

* C++ port of Newer SMBW's `PlaySound()` function
* An alternative implementation of `PlaySound()` that solved some issued I was having with panning.
* `ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME`, `ONCREATE_RESULT_SUCCESS`, `ONCREATE_RESULT_PLEASE_DELETE_ME`: aliases for 0, 1, and 2, which you can use as return values from `create` functions to make them easier to understand.
