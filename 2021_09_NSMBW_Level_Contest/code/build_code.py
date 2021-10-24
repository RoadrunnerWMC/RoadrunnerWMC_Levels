from pathlib import Path
import subprocess
import sys

# Paths to things
KAMEK_ROOT_DIR = Path('Kamek')

CODEWARRIOR_EXE = KAMEK_ROOT_DIR / 'cw' / 'mwcceppc.exe'
KAMEK_EXE = KAMEK_ROOT_DIR / 'Kamek' / 'bin' / 'Debug' / 'Kamek.exe'
KSTDLIB_DIR = KAMEK_ROOT_DIR / 'k_stdlib'

EXTERNALS_TXT = Path('externals-nsmbw-P1.txt')
ADDRESS_MAP_TXT = Path('address-map.txt')

LIBS_DIR = Path('libs')

OUTPUT_DIR = Path('dist')

# Source files
CPP_FILES = [Path('contest_001_indoors')]

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
    '-i', '.',
    '-Cpp_exceptions', 'off',
    '-enum', 'int',
    '-O4,s',
    '-use_lmw_stmw', 'on',
    '-fp', 'hard',
    '-rostr',
    '-sdata', '0',
    '-sdata2', '0',
    '-RTTI', 'off']

for i in CPP_FILES:
    if OUTPUT_DEBUG:
        print(f'(Debug) Preprocessing {i}.cpp...')
        subprocess.run(CC + CFLAGS + ['-P', '-c', '-o', f'{i}_preprocessed.cpp', f'{i}.cpp'])
    print(f'Compiling {i}.cpp...')
    subprocess.run(CC + CFLAGS + ['-c', '-o', f'{i}.o', f'{i}.cpp'])


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
