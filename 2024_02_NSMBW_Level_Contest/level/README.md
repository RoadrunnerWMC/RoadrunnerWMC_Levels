# Level file for "Make or Break"

This is the "source" version of the level -- suitable for manual editing, but not the exact arc file that is used in-game. (Details on that below.)

level.arc can be edited with Reggie. Add the custom tileset from the ["tileset" folder](../assets/tilesets) to your Reggie Stage folder first.

In this copy of the level, the ice platforms are represented as nonexistent "sprite 22". "build.py" in the parent directory removes them and adds appropriate tile gods and locations. There are also no event controllers associated with the ice platforms, as build.py takes care of creating those, too. Instead, placeholder sprites 0, 1, 2, and 3 (zone ID minus 1) are used to indicate their positions, which allowed me to arrange them into simple artwork as an Easter egg.

Spritedata for sprite 22:

- Nybble 5: width ("3" or "5")
- Nybble 6: 1 = the player might approach the platform from below while carrying a shell, 0 = that's unlikely or impossible. Enabling this fixes a bug that occurs in that situation, but this uses an additional event ID, so enabling it for all platforms would limit the number of platforms you can put in one zone. Therefore, in the level, this is set to 0 for all platforms that would require a propeller suit to approach from below.
- Nybble 7: type of terrain immediately to the left of the platform: 0 = none, 1 = floor, 2 = wall. This is used to adjust the positions and sizes of the event-trigger locations.
- Nybble 8: type of terrain immediately to the right of the platform (see Nybble 7)
