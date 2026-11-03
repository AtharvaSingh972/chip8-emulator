# ROMs

No ROM files are bundled with this repository. Download them separately from the sources below.

## Test ROMs

Download the compiled `.ch8` files from the [Timendus CHIP-8 test suite](https://github.com/Timendus/chip8-test-suite) (see its Releases page).

The suite includes:

1. CHIP-8 logo
2. IBM logo
3. Corax+ opcode test
4. Flags
5. Quirks
6. Keypad
7. Beep
8. Scrolling

## Games

Find PONG and other CHIP-8 games in the [CHIP-8 Archive](https://github.com/JohnEarnest/chip8Archive).

Check the individual license of each ROM before redistributing it.

## Error-handling ROMs

Generate the error-handling test ROMs locally using:

`python tools/rom_maker.py`

## Running a ROM

From the project root, run:

`./chip8 roms/tests/4-flags.ch8`

Replace the path with the location of your downloaded ROM.