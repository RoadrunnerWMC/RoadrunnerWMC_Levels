#!/usr/bin/env python3

# Copyright (C) 2021 RoadrunnerWMC

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


import argparse
import dataclasses
import os
from pathlib import Path
import shutil
import struct
import subprocess
from typing import List

import nsmbpy.level


os.chdir(Path(__file__).parent)

CODE_DIR = Path('code')
CODE_DIST_DIR = CODE_DIR / 'dist'
CODE_DIST_LOADER_DIR = CODE_DIR / 'dist_loader'
ASSETS_DIR = Path('assets')
LEVEL_DIR = Path('level')

RIIVO_XML_NAME = 'contest_level.xml'

README_IN = ASSETS_DIR / 'dist_readme.txt'

OUTPUT_PATH = Path('dist')
OUTPUT_ASSETS_PATH = OUTPUT_PATH / 'contest_level'
OUTPUT_README_PATH = OUTPUT_PATH / 'readme.txt'
OUTPUT_RIIVO_PATH = OUTPUT_PATH / 'riivolution'
OUTPUT_RIIVO_XML_PATH = OUTPUT_RIIVO_PATH / RIIVO_XML_NAME

SUPPORTED_GAME_VERSIONS = ['P1', 'E1', 'J1', 'P2', 'E2', 'J2']  # K/W/C aren't yet supported by the loader

LEVEL_PAD_SIZE = 80000

MASS_DOOR_RENDERER_SPRITE_TYPE = 250


def pad_to_size(data: bytes, size: int) -> bytes:
    if len(data) > size:
        raise ValueError(f'File already exceeds padded size: {len(data)} > {size}')
    return data + b'\0' * (size - len(data))



def copy_file(from_: Path, to_: Path, *, create_parents:bool=False):
    """
    Copy a file, optionally creating parent directories.
    """
    if create_parents:
        if not to_.parent.is_dir():
            to_.parent.mkdir(parents=True)
    shutil.copy(from_, to_)


class Asset:
    """
    A single file available somewhere on the file system, to be copied to output directories.
    """
    src_path: Path  # absolute path to where the file can be found
    ingame_path: Path  # relative path on disc, e.g. Path('Stage/01-01.arc')

    def __init__(self, src_path: Path, ingame_path: Path):
        self.src_path = src_path
        self.ingame_path = ingame_path

    def copy_to_game_dir(self, game_dir: Path, *, create_parents:bool=False, debug:bool=False):
        """
        Copy the file to (a subdirectory of, probably) a game dir.
        """
        copy_file(self.src_path, game_dir / self.ingame_path, create_parents=create_parents)


@dataclasses.dataclass
class Door2D:
    """
    Represents a 2D door
    """
    x: int  # pixels -- position of top-left corner
    y: int  # pixels -- position of top-left corner
    direction: int  # up, down, left, right


def extract_2d_doors_from_area(area: nsmbpy.level.LevelArea) -> List[Door2D]:
    """
    Remove placeholder 2D door objects from the area, and return them as
    a list of Door2D
    """
    # The doors were tiled with Pa1_doors, so the input level should be
    # using that
    assert area.tileset_names[1] == 'Pa1_doors'
    # We're going to remove them and convert them all to sprites, so the
    # tileset isn't actually needed (and isn't even included with the
    # final Riivo patch, moreover)
    area.tileset_names[1] = ''

    doors = []

    new_layer = []
    for obj in area.layer_0:
        if obj.tileset_id == 1:
            if obj.type in {0, 1}:
                assert (obj.width, obj.height) == (2, 3)
            elif obj.type in {2, 3}:
                assert (obj.width, obj.height) == (3, 2)
            else:
                assert False

            doors.append(Door2D(obj.x * 16, obj.y * 16, obj.type))
        else:
            new_layer.append(obj)

    area.layer_0 = new_layer

    return doors


def add_mass_door_renderer(area: nsmbpy.level.LevelArea, zone: nsmbpy.level.LevelZone, doors: List[Door2D]) -> None:
    """
    Set up a Mass Door Renderer sprite for this zone.
    """
    if not doors:
        return

    path_id = zone.id + 1
    path = nsmbpy.level.LevelPath(id=path_id, start_node_index=len(area.path_nodes), num_nodes=len(doors))
    area.paths.append(path)

    for door in sorted(doors, key=lambda d: (d.x, d.y)):
        path_node = nsmbpy.level.LevelPathNode(x=door.x, y=door.y, speed=0, acceleration=0, delay=door.direction)
        area.path_nodes.append(path_node)

    spr = nsmbpy.level.LevelSprite(
        type=MASS_DOOR_RENDERER_SPRITE_TYPE,
        x=zone.x, y=zone.y,
        data_1=struct.pack('>5xB', path_id))
    area.sprites.append(spr)


def build_level(*, pad=False) -> List[Asset]:
    DOT = Path('.')

    # Open master level
    with (LEVEL_DIR / 'level.arc').open('rb') as f:
        level_data = f.read()

    # Parse
    level = nsmbpy.level.Level.load(nsmbpy.Game.NEW_SUPER_MARIO_BROS_WII, level_data)

    # Do stuff
    for area in level.areas:
        doors = extract_2d_doors_from_area(area)

        for door in doors:
            door.zone_id = area.map_position_to_zone(door.x, door.y).id

        for zone in area.zones:
            add_mass_door_renderer(area, zone, [d for d in doors if d.zone_id == zone.id])

    # Serialize and save
    level_data = level.save(nsmbpy.Game.NEW_SUPER_MARIO_BROS_WII)

    if pad:
        level_data = pad_to_size(level_data, LEVEL_PAD_SIZE)

    with (DOT / '_level_preprocessed.arc').open('wb') as f:
        f.write(level_data)

    # Return asset including both versions
    return [Asset(
        DOT / '_level_preprocessed.arc',
        Path('Stage/01-01.arc'))]


def prepare_assets() -> List[Asset]:
    DOT = Path('.')

    assets = []

    # TILESET_NAME = 'Pa1_doors.arc'
    # assets.append(Asset(DOT / TILESET_NAME, Path('Stage/Texture/' + TILESET_NAME)))

    TILESET_NAME = 'Pa2_code_failure.arc'
    assets.append(Asset(ASSETS_DIR / 'tilesets' / TILESET_NAME, Path('Stage/Texture/' + TILESET_NAME)))

    for name in ['obj_door_flat', 'obj_remo_door', 'obj_hyper_remo_door']:
        assets.append(Asset(ASSETS_DIR / 'models' / name / f'{name}.arc', Path(f'Object/{name}.arc')))
    for name in ['bgB_3F03', 'bgB_3F0F']:
        assets.append(Asset(ASSETS_DIR / 'models' / 'bg' / f'{name}.arc', Path(f'Object/{name}.arc')))

    return assets


def build_code() -> None:
    subprocess.run(['python3', 'build_code.py'], cwd=str(CODE_DIR))


def add_code() -> List[Asset]:

    # Add code files
    assets = []
    for version in SUPPORTED_GAME_VERSIONS:
        code_fn = f'{version}.bin'
        assets.append(Asset(CODE_DIST_DIR / code_fn, Path('Code/' + code_fn)))

    # Add loader.bin
    assets.append(Asset(CODE_DIST_LOADER_DIR / 'loader.bin', Path('loader.bin')))

    return assets


def create_dist_package(assets: List[Asset]) -> None:
    """
    Prepare the output directory
    """
    print('Clearing output path...')
    if OUTPUT_PATH.is_dir():
        shutil.rmtree(OUTPUT_PATH)

    print('Copying assets to output folder...')
    for asset in assets:
        asset.copy_to_game_dir(OUTPUT_ASSETS_PATH, create_parents=True)

    print('Copying readme...')
    copy_file(README_IN, OUTPUT_README_PATH)

    print('Copying Riivo stuff...')
    copy_file(CODE_DIST_LOADER_DIR / RIIVO_XML_NAME, OUTPUT_RIIVO_XML_PATH, create_parents=True)


def main(args=None):
    parser = argparse.ArgumentParser(
        description='Build script for NHD contest level')

    parser.add_argument('--skip-code', action='store_true',
        help='skip rebuilding the code')
    parser.add_argument('--pad-level', action='store_true',
        help='add pading to the level file (useful for testing in Dolphin)')

    parsed_args = parser.parse_args(args)


    assets = []

    print('Building level...')
    assets += build_level(pad=parsed_args.pad_level)

    print('Preparing assets...')
    assets += prepare_assets()

    if not parsed_args.skip_code:
        print('Building code...')
        build_code()

    assets += add_code()

    create_dist_package(assets)


main()
