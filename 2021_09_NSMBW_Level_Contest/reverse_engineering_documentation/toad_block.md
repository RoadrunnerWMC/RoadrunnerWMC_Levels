# EN_BLOCK_HATENA_PLAYER

Profile at `80adbcdc`, vtable at `80adbcf4`.

## Spritedata

Last bit of spritedata is a flag: "0" puts 2 into this+6f8, "1" puts 1 there.

this+6f8 gets reset to 0 if:
* there's more than 1 player
* the current world number is >= 11, or the current level number is >= 43
* some condition regarding world map scripts (800fd140)
* some other condition (809c3410)

6f8 is referenced in 809c3870, 809c3a00, 809c3b10.

After testing in-game: the spritedata bit causes Toad to spawn small rather than super.

## Forcing block to always contain Toad, regardless of world-map status

Make `800fd140` return 1.

## Making block haunted when you open it

    Doing the following:
        809c3ad0 -> 4e800020
        809c3ae0 -> 4e800020
        Makes the block never stop bouncing after being hit
    Doing the following:
        809a5520 -> 4e800020
        has no noticeable effect
    Doing the following:
        809c35a0 -> 4e800020
        makes the block shut up as you approach it, but that's all
    Doing the following:
        809c37b0 -> 4e800020
        has no noticeable effect
    Doing the following:
        809c3870 -> 4e800020
        makes the block un-hittable
    
I think `809c397c` is what determines if you get a coin or a Toad. Let's see...

    Doing the following:
        809c397c -> 2c040000 (force branch not taken)
        makes a coin come out
    Doing the following:
        809c397c -> 4800004c (force branch taken)
        makes no change (probably forces Toad if he wasn't originally going to spawn)
