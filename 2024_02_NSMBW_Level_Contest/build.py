#!/usr/bin/env python3

# Copyright (C) 2024 RoadrunnerWMC

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

from __future__ import annotations

import argparse
import dataclasses
import enum
import os
from pathlib import Path
import random
import shutil
import subprocess

import nsmbpy.level


os.chdir(Path(__file__).parent)


ASSETS_DIR = Path('assets')
LEVEL_DIR = Path('level')

README_IN = ASSETS_DIR / 'dist_readme.txt'

OUTPUT_DIR = Path('dist')
OUTPUT_README_PATH = OUTPUT_DIR / 'readme.txt'


LEVEL_PAD_SIZE = 80000

SPRITE_TYPE_THIN_ICE_PLATFORM_PLACEHOLDER = 22

SPRITE_TYPE_ZONE_ENTER = 33
SPRITE_TYPE_AND = 34
SPRITE_TYPE_CHAINER = 37
SPRITE_TYPE_IF = 38
SPRITE_TYPE_TILE_GOD = 191

PLATFORM_3x1_TILES = [
    (5, 6),
    (5, 7),
    (5, 8),
]
PLATFORM_5x1_TILES = [
    (3, 6),
    (3, 7),
    (4, 6),
    (4, 7),
    (4, 8),
]

PLATFORM_RESPAWN_TIMER = 9
ENEMY_SUPPRESSION_EVENT_TIMER = 5
LOWEST_RESERVED_EVENT_PER_ZONE = {
    1: 64,
}
MAX_EVENT = 64
RAND_SEED = 1234

Position = tuple[int, int]


def make_zone_enter(
    *,
    x: int,
    y: int,
    target_event_id: int = 0,
    activates_event: bool = False,
    time_activated: int = 0,
    do_nothing: bool = False,
) -> nsmbpy.level.LevelSprite:
    """Create an Event Controller - Zone Enter sprite"""
    return nsmbpy.level.LevelSprite(
        type=SPRITE_TYPE_ZONE_ENTER,
        x=x, y=y,
        data_1=bytes([
            0,
            target_event_id,
            time_activated,
            0,
            0,
            (int(activates_event) << 4) | int(do_nothing),
        ]),
    )


class AndUsageMode(enum.Enum):
    """Represents usage modes for Event Controller - "AND" """
    SINGLE_USE = 0
    MULTI_USE = 1
    ONLY_WHILE_ALL_ACTIVE = 2


def make_and(
    *,
    x: int,
    y: int,
    triggering_event_id_1: int = 0,
    triggering_event_id_2: int = 0,
    triggering_event_id_3: int = 0,
    triggering_event_id_4: int = 0,
    target_event_id: int = 0,
    usage_mode: AndUsageMode = AndUsageMode.SINGLE_USE,
    activates_event: bool = True,
) -> nsmbpy.level.LevelSprite:
    """Create an Event Controller - "AND" sprite"""
    return nsmbpy.level.LevelSprite(
        type=SPRITE_TYPE_AND,
        x=x, y=y,
        data_1=bytes([
            triggering_event_id_1,
            target_event_id,
            triggering_event_id_2,
            triggering_event_id_3,
            triggering_event_id_4,
            (usage_mode.value << 4) | int(not activates_event),
        ]),
    )


class ChainerUsageMode(enum.Enum):
    """Represents usage modes for Event Controller - Chainer"""
    SINGLE_USE = 0
    MULTI_USE = 1
    ONLY_WHILE_ACTIVE = 2


def make_chainer(
    *,
    x: int,
    y: int,
    triggering_event_id: int = 0,
    target_event_id: int = 0,
    triggers_from_deactivation: bool = False,
    activates_event: bool = True,
    delay: int = 0,
    usage_mode: ChainerUsageMode = ChainerUsageMode.SINGLE_USE,
) -> nsmbpy.level.LevelSprite:
    """Create an Event Controller - Chainer sprite"""
    return nsmbpy.level.LevelSprite(
        type=SPRITE_TYPE_CHAINER,
        x=x, y=y,
        data_1=bytes([
            triggering_event_id,
            target_event_id,
            delay,
            int(triggers_from_deactivation),
            0,
            (usage_mode.value << 4) | int(not activates_event),
        ]),
    )


class IfUsageMode(enum.Enum):
    """Represents usage modes for Event Controller - "IF" """
    SINGLE_USE = 0
    MULTI_USE = 1
    ONLY_WHILE_SATISFIED = 2


class IfPlayerStatus(enum.Enum):
    """Represents player status modes for Event Controller - "IF" """
    ANY = 0
    GROUND = 1
    AIR = 2
    CLIMBING_FENCE = 3


class IfPlayerPowerupStatus(enum.Enum):
    """Represents powerup status modes for Event Controller - "IF" """
    ANY = 0
    NONE = 1
    MUSHROOM = 2
    FIRE_FLOWER = 3
    MINI_MUSHROOM = 4
    STAR = 7


LOCATION_ID_IGNORE = 0xff


def make_if(
    *,
    x: int,
    y: int,
    target_event_id: int = 0,
    activates_event: bool = True,
    player_location_id: int = 0,
    player_status: IfPlayerStatus = IfPlayerStatus.ANY,
    player_powerup_status: IfPlayerPowerupStatus = IfPlayerPowerupStatus.ANY,
    enemy_location_id: int = 0,
    usage_mode: IfUsageMode = IfUsageMode.SINGLE_USE,
) -> nsmbpy.level.LevelSprite:
    """Create an Event Controller - "IF" sprite"""
    return nsmbpy.level.LevelSprite(
        type=SPRITE_TYPE_IF,
        x=x, y=y,
        data_1=bytes([
            0,
            target_event_id,
            player_location_id,
            enemy_location_id,
            (player_powerup_status.value << 4) | player_status.value,
            (usage_mode.value << 4) | int(not activates_event),
        ]),
    )


class TileGodPattern(enum.Enum):
    """Represents fill-pattern modes for Tile God"""
    FILL = 0
    CHECKERS = 1
    REVERSE_CHECKERS = 2


class TileGodTile(enum.Enum):
    """Represents tile choices for Tile God"""
    RED_BLOCK = 0
    BRICK_BLOCK = 1
    STONE_BLOCK = 3
    WOODEN_BLOCK = 4
    NEWER_PA0 = 8
    NEWER_PA1 = 9
    NEWER_PA2 = 10
    NEWER_PA3 = 11
    PA0_2_13 = 6
    PA1_0_0 = 14
    PA3_4_3 = 12


class TileGodLayer(enum.Enum):
    """Represents layer choices for Tile God"""
    LAYER_0 = 2
    LAYER_1 = 0
    LAYER_2 = 1


def make_tile_god(
    *,
    x: int, y: int,
    triggering_event_id: int = 0,
    width: int = 0,
    height: int = 0,
    layer: TileGodLayer = TileGodLayer.LAYER_1,
    creates_tiles: bool = False,
    single_shot: bool = False,
    enable_effects: bool = True,
    enable_sounds: bool = True,
    pattern: TileGodPattern = TileGodPattern.FILL,
    tile: TileGodTile = TileGodTile.RED_BLOCK,
    newer_tileset_row: int = 0,
    newer_tileset_column: int = 0,
) -> nsmbpy.level.LevelSprite:
    """Create a Tile God sprite"""
    return nsmbpy.level.LevelSprite(
        type=SPRITE_TYPE_TILE_GOD,
        x=x,
        y=y,
        data_1=bytes([
            0,
            triggering_event_id,
            (int(single_shot) << 4) | newer_tileset_row,
            (newer_tileset_column << 4) | (0 if enable_sounds else 8) | (0 if enable_effects else 4) | pattern.value,
            (tile.value << 4) | int(creates_tiles),
            (width << 4) | height,
        ]),
        layer = layer.value,
    )


def pad_to_size(data: bytes, size: int) -> bytes:
    """Add null bytes to data to make it reach a desired size"""
    if len(data) > size:
        raise ValueError(f'File already exceeds padded size: {len(data)} > {size}')
    return data + b'\0' * (size - len(data))


_did_lzx_warning = False
def attempt_compress(path: Path) -> Path:
    """
    Try to LZ-compress the file at `path` using lxz.exe (from Nintendo
    DS Compressors by CUE), and rename it to add ".LZ" to the end if
    successful.

    Returns the final file path, whether compression was successful or
    not.
    """
    global _did_lzx_warning

    LZX_EXE = Path('lzx.exe')

    if LZX_EXE.is_file():
        if os.name == 'nt':
            cmd = [str(LZX_EXE)]
        else:
            cmd = ['wine', str(LZX_EXE)]

        cmd += ['-ewb', path]

        try:
            subprocess.check_output(cmd)

        except (FileNotFoundError, subprocess.CalledProcessError):
            if not _did_lzx_warning:
                print("couldn't run lzx.exe -- output files will be uncompressed")
                _did_lzx_warning = True

            return path

        new_path = path.parent / (path.name + '.LZ')
        path.rename(new_path)

        return new_path

    else:
        if not _did_lzx_warning:
            print('lzx.exe not found -- output files will be uncompressed')
            _did_lzx_warning = True

        return path


class ThinIcePlatformEdgeBehavior(enum.IntEnum):
    """
    Describes what's on either side of a ThinIcePlatform, so the
    locations can be positioned and sized appropriately
    """
    OPEN = 0   # nothing on this side, just open air
    FLOOR = 1  # player can walk off this side directly onto some other platform
    WALL = 2   # platform is up against a wall


@dataclasses.dataclass
class ThinIcePlatform:
    """Represents a thin-ice platform"""
    x: int  # 1/16 tiles -- position of top-left corner of center tile
    y: int  # 1/16 tiles -- position of top-left corner of center tile
    width: int  # either 3 or 5
    accessible_from_below: bool
    left_side: ThinIcePlatformEdgeBehavior
    right_side: ThinIcePlatformEdgeBehavior


def extract_snowflake_patterns_from_area(
    area: nsmbpy.level.LevelArea,
) -> dict[int, list[Position]]:
    """
    Remove placeholder snowflake-pattern sprites from the area, and
    return them as a list of positions (mapped from zone IDs)
    """
    positions = {}

    new_sprites = []

    for sprite in area.sprites:
        if sprite.type < 10:
            positions.setdefault(sprite.type, []).append((sprite.x, sprite.y))
        else:
            new_sprites.append(sprite)

    area.sprites.clear()
    area.sprites.extend(new_sprites)

    return positions


def extract_thin_ice_platforms_from_area(area: nsmbpy.level.LevelArea) -> list[ThinIcePlatform]:
    """
    Remove placeholder thin-ice platform sprites from the area, and
    return them as a list of ThinIcePlatform
    """
    platforms = []

    new_sprites = []

    for sprite in area.sprites:
        if sprite.type == SPRITE_TYPE_THIN_ICE_PLATFORM_PLACEHOLDER:
            width = sprite.data_1[0] >> 4
            accessible_from_below = bool(sprite.data_1[0] & 1)
            left_side = ThinIcePlatformEdgeBehavior((sprite.data_1[1] >> 4) & 15)
            right_side = ThinIcePlatformEdgeBehavior(sprite.data_1[1] & 15)
            plat = ThinIcePlatform(
                sprite.x,
                sprite.y,
                width,
                accessible_from_below,
                left_side,
                right_side,
            )
            platforms.append(plat)
        else:
            new_sprites.append(sprite)

    area.sprites.clear()
    area.sprites.extend(new_sprites)

    return platforms


def add_platforms_to_zone(
    area: nsmbpy.level.LevelArea,
    zone: nsmbpy.level.LevelZone,
    platforms: list[ThinIcePlatform],
    snowflake_pattern: list[Position] | None,
    rand: random.Random,
) -> None:
    """Add sprites and locations for all platforms in this zone"""
    if not platforms:
        return

    print(f'---- Zone {zone.id + 1} ----')

    if snowflake_pattern is not None:
        snowflake_pattern = list(snowflake_pattern)
        snowflake_pattern.sort()
        rand.shuffle(snowflake_pattern)

    next_platform_specific_event_id = 1
    def get_next_platform_specific_event_id() -> int:
        nonlocal next_platform_specific_event_id
        next_platform_specific_event_id += 1
        return next_platform_specific_event_id - 1

    first_enemy_trigger_suppression_event_id = 1 + len(platforms) * 3
    for p in platforms:
        if p.accessible_from_below:
            first_enemy_trigger_suppression_event_id += 1

    next_enemy_trigger_suppression_event_id = first_enemy_trigger_suppression_event_id
    def get_next_enemy_trigger_suppression_event_id() -> int:
        nonlocal next_enemy_trigger_suppression_event_id
        ret = next_enemy_trigger_suppression_event_id

        next_enemy_trigger_suppression_event_id += 1
        if next_enemy_trigger_suppression_event_id >= LOWEST_RESERVED_EVENT_PER_ZONE.get(zone.id + 1, MAX_EVENT + 1):
            next_enemy_trigger_suppression_event_id = first_enemy_trigger_suppression_event_id

        return ret

    next_location_id = 1
    for loc in area.locations:
        if loc.id >= next_location_id:
            next_location_id = loc.id + 1

    def get_next_location_id() -> int:
        nonlocal next_location_id
        next_location_id += 1
        return next_location_id - 1

    controllers_start_x = zone.x + 32
    controllers_start_y = zone.y + 32
    controllers_next_x = controllers_start_x
    controllers_next_y = controllers_start_y

    def start_new_controller_column() -> None:
        nonlocal controllers_next_x, controllers_next_y
        controllers_next_x += 16
        controllers_next_y = controllers_start_y

    def get_next_controller_pos() -> Position:
        if snowflake_pattern is None:
            nonlocal controllers_next_x, controllers_next_y
            controllers_next_y += 16
            return controllers_next_x, controllers_next_y - 16
        else:
            try:
                return snowflake_pattern.pop(0)
            except IndexError:
                raise ValueError('Not enough snowflake pattern positions -- please add some more')

    used_enemy_trigger_suppression_event_ids = set()

    for platform in platforms:
        start_new_controller_column()

        event_id_1 = get_next_platform_specific_event_id()
        event_id_2 = get_next_platform_specific_event_id()
        event_id_3 = get_next_platform_specific_event_id()
        if platform.accessible_from_below:
            event_id_4 = get_next_platform_specific_event_id()
            enemy_trigger_suppression_event_id = get_next_enemy_trigger_suppression_event_id()
            used_enemy_trigger_suppression_event_ids.add(enemy_trigger_suppression_event_id)
        else:
            event_id_4 = None
            enemy_trigger_suppression_event_id = None

        if zone.id != 0:
            x, y = get_next_controller_pos()
            area.sprites.append(make_zone_enter(
                x=x, y=y,
                target_event_id=event_id_1,
                activates_event=False,
            ))
            x, y = get_next_controller_pos()
            area.sprites.append(make_zone_enter(
                x=x, y=y,
                target_event_id=event_id_2,
                activates_event=False,
            ))
            x, y = get_next_controller_pos()
            area.sprites.append(make_zone_enter(
                x=x, y=y,
                target_event_id=event_id_3,
                activates_event=False,
            ))
            if event_id_4 is not None:
                x, y = get_next_controller_pos()
                area.sprites.append(make_zone_enter(
                    x=x, y=y,
                    target_event_id=event_id_4,
                    activates_event=False,
                ))

        if platform.width == 3:
            area.sprites.append(make_tile_god(
                x=platform.x - 16,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=False,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_3x1_TILES[0][0],
                newer_tileset_column=PLATFORM_3x1_TILES[0][1],
            ))
            area.sprites.append(make_tile_god(
                x=platform.x,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=True,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_3x1_TILES[1][0],
                newer_tileset_column=PLATFORM_3x1_TILES[1][1],
            ))
            area.sprites.append(make_tile_god(
                x=platform.x + 16,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=False,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_3x1_TILES[2][0],
                newer_tileset_column=PLATFORM_3x1_TILES[2][1],
            ))

        else:  # 5
            area.sprites.append(make_tile_god(
                x=platform.x - 32,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=False,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_5x1_TILES[0][0],
                newer_tileset_column=PLATFORM_5x1_TILES[0][1],
            ))
            area.sprites.append(make_tile_god(
                x=platform.x - 16,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=False,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_5x1_TILES[1][0],
                newer_tileset_column=PLATFORM_5x1_TILES[1][1],
            ))
            area.sprites.append(make_tile_god(
                x=platform.x,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=True,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_5x1_TILES[2][0],
                newer_tileset_column=PLATFORM_5x1_TILES[2][1],
            ))
            area.sprites.append(make_tile_god(
                x=platform.x + 16,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=False,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_5x1_TILES[3][0],
                newer_tileset_column=PLATFORM_5x1_TILES[3][1],
            ))
            area.sprites.append(make_tile_god(
                x=platform.x + 32,
                y=platform.y,
                triggering_event_id=event_id_3,
                width=1,
                height=1,
                enable_sounds=False,
                tile=TileGodTile.NEWER_PA1,
                newer_tileset_row=PLATFORM_5x1_TILES[4][0],
                newer_tileset_column=PLATFORM_5x1_TILES[4][1],
            ))

        main_loc_id = get_next_location_id()

        loc_x1 = platform.x - {3: 16, 5: 32}[platform.width]
        loc_x2 = loc_x1 + 16 * platform.width
        unadjusted_loc_x1 = loc_x1
        unadjusted_loc_x2 = loc_x2
        if platform.left_side == ThinIcePlatformEdgeBehavior.OPEN:
            loc_x1 -= 8
        elif platform.left_side == ThinIcePlatformEdgeBehavior.FLOOR:
            # note: anything < 5 makes it possible to stand right on the
            # edge and have the platform automatically despawn and
            # respawn over and over
            loc_x1 += 5
        if platform.right_side == ThinIcePlatformEdgeBehavior.OPEN:
            loc_x2 += 8
        elif platform.right_side == ThinIcePlatformEdgeBehavior.FLOOR:
            loc_x2 -= 5

        area.locations.append(nsmbpy.level.LevelLocation(
            x = loc_x1,
            y = platform.y - 1,
            width = loc_x2 - loc_x1,
            height = 2,
            id = main_loc_id,
        ))

        if platform.accessible_from_below:
            extra_loc_id = get_next_location_id()
            extra_loc_x1 = unadjusted_loc_x1
            extra_loc_x2 = unadjusted_loc_x2
            if platform.left_side == ThinIcePlatformEdgeBehavior.OPEN:
                extra_loc_x1 -= 12
            if platform.right_side == ThinIcePlatformEdgeBehavior.OPEN:
                extra_loc_x2 += 12

            area.locations.append(nsmbpy.level.LevelLocation(
                x = extra_loc_x1,
                y = platform.y + 4,
                width = extra_loc_x2 - extra_loc_x1,
                height = 8,
                id = extra_loc_id,
            ))

        else:
            extra_loc_id = None

        # The order of the controllers in the level data is very important!
        x, y = get_next_controller_pos()
        ev34 = event_id_2
        if enemy_trigger_suppression_event_id is not None:
            ev34 = enemy_trigger_suppression_event_id
        area.sprites.append(make_and(
            x=x, y=y,
            triggering_event_id_1=event_id_2,
            triggering_event_id_2=event_id_2,
            triggering_event_id_3=ev34,
            triggering_event_id_4=ev34,
            target_event_id=event_id_3,
            usage_mode=AndUsageMode.MULTI_USE,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_chainer(
            x=x, y=y,
            triggering_event_id=event_id_3,
            target_event_id=event_id_3,
            activates_event=False,
            delay=PLATFORM_RESPAWN_TIMER,
            usage_mode=ChainerUsageMode.MULTI_USE,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_if(
            x=x, y=y,
            target_event_id=event_id_1,
            player_location_id=main_loc_id,
            player_status=IfPlayerStatus.GROUND,
            enemy_location_id=LOCATION_ID_IGNORE,
            usage_mode=IfUsageMode.MULTI_USE,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_chainer(
            x=x, y=y,
            triggering_event_id=event_id_3,
            target_event_id=event_id_2,
            activates_event=False,
            delay=PLATFORM_RESPAWN_TIMER,
            usage_mode=ChainerUsageMode.MULTI_USE,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_chainer(
            x=x, y=y,
            triggering_event_id=event_id_3,
            target_event_id=event_id_1,
            activates_event=False,
            delay=PLATFORM_RESPAWN_TIMER,
            usage_mode=ChainerUsageMode.MULTI_USE,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_chainer(
            x=x, y=y,
            triggering_event_id=event_id_1,
            target_event_id=event_id_3,
            delay=1,
            usage_mode=ChainerUsageMode.MULTI_USE,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_if(
            x=x, y=y,
            target_event_id=event_id_2,
            activates_event=False,
            player_location_id=LOCATION_ID_IGNORE,
            enemy_location_id=main_loc_id,
            usage_mode=IfUsageMode.ONLY_WHILE_SATISFIED,
        ))
        if platform.accessible_from_below:
            x, y = get_next_controller_pos()
            area.sprites.append(make_if(
                x=x, y=y,
                target_event_id=event_id_4,
                activates_event=False,
                player_location_id=LOCATION_ID_IGNORE,
                enemy_location_id=extra_loc_id,
                usage_mode=IfUsageMode.ONLY_WHILE_SATISFIED,
            ))
            x, y = get_next_controller_pos()
            area.sprites.append(make_and(
                x=x, y=y,
                triggering_event_id_1=event_id_4,
                triggering_event_id_2=event_id_4,
                triggering_event_id_3=event_id_4,
                triggering_event_id_4=event_id_4,
                target_event_id=enemy_trigger_suppression_event_id,
                usage_mode=AndUsageMode.MULTI_USE,
                activates_event=False,
            ))

    start_new_controller_column()
    for event_id in sorted(used_enemy_trigger_suppression_event_ids):
        x, y = get_next_controller_pos()
        area.sprites.append(make_zone_enter(
            x=x, y=y,
            target_event_id=event_id,
            activates_event=True,
        ))
        x, y = get_next_controller_pos()
        area.sprites.append(make_chainer(
            x=x, y=y,
            triggering_event_id=event_id,
            target_event_id=event_id,
            triggers_from_deactivation=True,
            delay=ENEMY_SUPPRESSION_EVENT_TIMER,
            usage_mode=ChainerUsageMode.MULTI_USE,
        ))

    if used_enemy_trigger_suppression_event_ids:
        first_free = max(used_enemy_trigger_suppression_event_ids) + 1
    else:
        first_free = next_platform_specific_event_id

    print(f'Used {len(used_enemy_trigger_suppression_event_ids)} enemy-trigger suppression events')
    print(f'First free event ID is {first_free}')

    if snowflake_pattern is not None and snowflake_pattern:
        raise ValueError(f'{len(snowflake_pattern)} unused snowflake pattern positions')


def remove_layer_2_guides(area: nsmbpy.level.LevelArea) -> None:
    """
    Scrub invisible objects from layer 2, which are just used as guides
    for level editing
    """
    new_layer = []
    for obj in area.layer_2:
        if obj.tileset_id != 0 or obj.type not in {1, 2}:
            new_layer.append(obj)

    area.layer_2 = new_layer


def build_level(rand: random.Random, *, use_snowflake_patterns: bool = True, pad: bool = False) -> None:
    # Open master level
    with (LEVEL_DIR / 'level.arc').open('rb') as f:
        level_data = f.read()

    # Parse
    level = nsmbpy.level.Level.load(nsmbpy.Game.NEW_SUPER_MARIO_BROS_WII, level_data)

    # Do stuff
    for area in level.areas:
        remove_layer_2_guides(area)

        platforms = extract_thin_ice_platforms_from_area(area)
        snowflake_patterns = extract_snowflake_patterns_from_area(area)

        for zone in area.zones:
            platforms_for_zone = []
            for p in platforms:
                if area.map_position_to_zone(p.x, p.y).id == zone.id:
                    platforms_for_zone.append(p)

            platforms_for_zone.sort(key=lambda p: p.x)

            if use_snowflake_patterns:
                sp = snowflake_patterns.get(zone.id, [])
            else:
                sp = None

            add_platforms_to_zone(
                area,
                zone,
                platforms_for_zone,
                sp,
                rand,
            )

    # Serialize and save
    level_data = level.save(nsmbpy.Game.NEW_SUPER_MARIO_BROS_WII)

    if pad:
        level_data = pad_to_size(level_data, LEVEL_PAD_SIZE)

    with Path('_level_preprocessed.arc').open('wb') as f:
        f.write(level_data)


def create_dist_package() -> None:
    """Prepare the output directory"""

    print('Clearing output path...')
    if OUTPUT_DIR.is_dir():
        shutil.rmtree(OUTPUT_DIR)
    OUTPUT_DIR.mkdir(parents=True)

    print('Copying assets to output folder and compressing...')

    (OUTPUT_DIR / 'BGs').mkdir()
    for fn in ['bgA_4503.arc', 'bgA_5203.arc', 'bgB_4503.arc']:
        shutil.copy(ASSETS_DIR / 'backgrounds' / fn, OUTPUT_DIR / 'BGs' / fn)
        attempt_compress(OUTPUT_DIR / 'BGs' / fn)

    (OUTPUT_DIR / 'Stages').mkdir()
    shutil.copy('_level_preprocessed.arc', OUTPUT_DIR / 'Stages' / '01-01.arc')
    attempt_compress(OUTPUT_DIR / 'Stages' / '01-01.arc')

    (OUTPUT_DIR / 'Tilesets').mkdir()
    shutil.copy(ASSETS_DIR / 'tileset' / 'Pa1_tileset.arc', OUTPUT_DIR / 'Tilesets' / 'Pa1_tileset.arc')
    # no compression for tilesets (their graphics data is already compressed)

    print('Copying readme...')
    shutil.copy(README_IN, OUTPUT_README_PATH)


def main(args: list[str] | None = None):
    parser = argparse.ArgumentParser(
        description='Build script for Evo contest level 2024')
    parser.add_argument('--no-snowflake-patterns', action='store_true',
        help='put the event controllers in a simple shape instead of using the position-indicator sprites (for debugging)')
    parser.add_argument('--pad-level', action='store_true',
        help='add pading to the level file (useful for testing in Dolphin)')

    parsed_args = parser.parse_args(args)

    rand = random.Random(RAND_SEED)

    print('Building level...')
    build_level(
        rand,
        use_snowflake_patterns=(not parsed_args.no_snowflake_patterns),
        pad=parsed_args.pad_level,
    )

    create_dist_package()


main()
