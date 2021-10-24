# Level file for "Indoors"

This is the "source" version of the level -- suitable for manual editing, but not the exact arc file that is used in-game. (Details on that below.)

level.arc can be edited with Reggie. Add both custom tilesets from the ["tilesets" folder](../assets/tilesets) to your Reggie Stage folder first.

In this copy of the level, terrain doors are tiled with tileset objects on layer 0 (required). "build.py" in the parent directory removes these objects and replaces them with Mass Door Renderer sprites using auto-generated paths. Terrain doors are a bit desaturated to make them easily distinguishable from sprite doors.

For details on the custom sprites used in this level, see the readme in the ["code" folder](../code).
