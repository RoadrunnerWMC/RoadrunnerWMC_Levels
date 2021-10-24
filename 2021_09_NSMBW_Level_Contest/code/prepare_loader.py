from pathlib import Path
import shutil
from typing import List


DOT = Path('.')
LOADER_DIR = Path('Kamek/loader')
OUTPUT_DIR = Path('dist_loader')


def indent_all_strings(strings: List[str], amount: int) -> List[str]:
    """
    Indent all strings in a list by some amount
    """
    return [' ' * amount + s for s in strings]


def get_xml_memory_patches_filler() -> str:
    """
    Get the XML text to replace the "{{MEMORY_PATCHES}}" placeholder text with
    """
    with (LOADER_DIR / 'loader.xml').open('r', encoding='utf_8') as f:
        xml = f.read()

    xml_lines = xml.splitlines()

    # Kamek puts the whole loader as one big line in its XML.
    # That's dumb, so we replace that with "valuefile='loader.bin'" instead.

    first_line = xml_lines[0]
    assert len(first_line) > 1000, 'unexpectedly short first loader XML line'
    assert 'value=' in first_line, 'first loader XML line is in an unexpected format'

    first_line = first_line[:first_line.index('value=')]
    first_line += "valuefile='loader.bin' />"

    xml_lines[0] = first_line
    return '\n'.join(indent_all_strings(xml_lines, 8))


def build_output_xml(output_path: Path) -> None:
    """
    Build and save the final XML
    """
    with (DOT / 'contest_level_template.xml').open('r', encoding='utf_8') as f:
        xml = f.read()

    assert '{{MEMORY_PATCHES}}' in xml, 'no memory-patches replacement area found in XML template'
    xml = xml.replace('{{MEMORY_PATCHES}}', get_xml_memory_patches_filler())

    with output_path.open('w', encoding='utf_8') as f:
        f.write(xml)


def main() -> None:
    """
    Main function
    """
    if not OUTPUT_DIR.is_dir():
        OUTPUT_DIR.mkdir()

    build_output_xml(OUTPUT_DIR / 'contest_level.xml')

    shutil.copy(LOADER_DIR / 'loader.bin', OUTPUT_DIR / 'loader.bin')


main()
