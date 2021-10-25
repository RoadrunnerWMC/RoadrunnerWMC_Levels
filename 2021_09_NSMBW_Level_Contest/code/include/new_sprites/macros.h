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

#ifndef NEW_SPRITES_MACROS_H
#define NEW_SPRITES_MACROS_H

#include <kamek.h>
#include <game.h>


// Note: these are used as inputs to kmWrite* macros, so Kamek does
// know that it needs to translate them between versions
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


// Sprite-data-table flags
#define SPRITE_FLAGS_NULL 0
#define SPRITE_FLAGS_ALWAYS_SPAWNED 2
#define SPRITE_FLAGS_UNK_8 8


#endif  // NEW_SPRITES_MACROS_H
