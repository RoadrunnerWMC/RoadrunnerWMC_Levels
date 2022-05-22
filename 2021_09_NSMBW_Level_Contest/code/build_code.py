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


from pathlib import Path
import subprocess
import sys


# Paths to things
KAMEK_ROOT_DIR = Path('Kamek')

CODEWARRIOR_EXE = KAMEK_ROOT_DIR / 'cw' / 'mwcceppc.exe'
KAMEK_EXE = KAMEK_ROOT_DIR / 'Kamek' / 'bin' / 'Debug' / 'net6.0' / 'Kamek'
KSTDLIB_DIR = KAMEK_ROOT_DIR / 'k_stdlib'

EXTERNALS_TXT = Path('externals-nsmbw-P1.txt')
ADDRESS_MAP_TXT = Path('address-map.txt')

SRC_DIR = Path('src')
INCLUDE_DIR = Path('include')
LIBS_DIR = INCLUDE_DIR / 'libs'

OUTPUT_DIR = Path('dist')

# Source files
CPP_FILES = list(SRC_DIR.glob('**/*.cpp'))

# CLI args
OUTPUT_DEBUG = ('--debug' in sys.argv)

# For the "backdoor" blr injection
BACKDOOR_BLR_INSTRUCTION = bytes.fromhex('7ffffb78')
BLR_INSTRUCTION = bytes.fromhex('4e800020')


print('Compiling...')

CC = [str(CODEWARRIOR_EXE)]
if sys.platform != 'win32':
    CC = ['wine'] + CC

CFLAGS = [
    '-I-',
    '-i', str(KSTDLIB_DIR),
    '-i', str(LIBS_DIR),
    '-i', str(INCLUDE_DIR),
    '-Cpp_exceptions', 'off',
    '-enum', 'int',
    '-O4,s',
    '-use_lmw_stmw', 'on',
    '-fp', 'hard',
    '-rostr',
    '-sdata', '0',
    '-sdata2', '0',
    '-RTTI', 'off']

for fp in CPP_FILES:
    if OUTPUT_DEBUG:
        print(f'(Debug) Preprocessing {fp}...')
        subprocess.run(CC + CFLAGS + ['-P', '-c', '-o', str(fp.parent / (fp.stem + '_preprocessed.txt')), str(fp)])
    print(f'Compiling {fp}...')
    subprocess.run(CC + CFLAGS + ['-c', '-o', str(fp.with_suffix('.o')), str(fp)])


print('Linking...')

if not OUTPUT_DIR.is_dir():
    OUTPUT_DIR.mkdir()

subprocess.run([
    str(KAMEK_EXE),
    *[str(fn.with_suffix('.o')) for fn in CPP_FILES],
    '-dynamic',
    '-externals=' + str(EXTERNALS_TXT),
    '-versions=' + str(ADDRESS_MAP_TXT),
    '-output-kamek=' + str(OUTPUT_DIR / '$KV$.bin'),
])


print('Injecting backdoor blrs...')

for fp in OUTPUT_DIR.glob('*.bin'):
    fd = fp.read_bytes()
    fp.write_bytes(fd.replace(BACKDOOR_BLR_INSTRUCTION, BLR_INSTRUCTION))
