# CHIP-8 Emulator

A CHIP-8 interpreter written in C with SDL2 for graphics and input. It implements the full 35-opcode CHIP-8 instruction set and passes the standard CHIP-8 test ROMs.

## Screenshots

### IBM Logo
![IBM Logo running in the emulator](docs/ibm-logo.png)

### Pong
![Pong running in the emulator](docs/pong.png)

## Features

- All 35 standard CHIP-8 opcodes
- 64×32 display, scaled 10× (640×320 window)
- ~700 Hz CPU with separate 60 Hz delay and sound timers
- 16-key hex keypad mapped to the keyboard, including the blocking `FX0A` key wait
- Error detection for invalid or unsupported opcodes, stack overflow/underflow and out-of-range PC/memory access, reported with the offending instruction and PC
- Tested with the Timendus CHIP-8 test suite and PONG

## Status

| Feature                        | Status      |
|--------------------------------|-------------|
| CPU, display, input, timers    | Done        |
| Sound (beep on `sound_timer`)  | Planned     |
| Debugger (`--debug` mode)      | Planned     |

## Building

**Requirements:** a C compiler (GCC or Clang), `make`, and the SDL2 development library.

```bash
# Debian / Ubuntu
sudo apt install build-essential libsdl2-dev

# Fedora
sudo dnf install gcc make SDL2-devel

# macOS (Homebrew)
brew install sdl2
```

Then build:

```bash
make
```

## Usage

```bash
./chip8 path/to/rom.ch8
```

### Keypad

The original CHIP-8 hex keypad is mapped to the left side of the keyboard:

```
CHIP-8 keypad        Keyboard
┌───┬───┬───┬───┐    ┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ C │    │ 1 │ 2 │ 3 │ 4 │
├───┼───┼───┼───┤    ├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ D │    │ Q │ W │ E │ R │
├───┼───┼───┼───┤    ├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ E │    │ A │ S │ D │ F │
├───┼───┼───┼───┤    ├───┼───┼───┼───┤
│ A │ 0 │ B │ F │    │ Z │ X │ C │ V │
└───┴───┴───┴───┘    └───┴───┴───┴───┘
```

Close the window to quit.

## ROMs

No ROMs are bundled with this repository. See [`roms/README.md`](roms/README.md) for where to download test ROMs and games.

Error-handling test ROMs are generated locally:

```bash
python tools/rom_maker.py
```

## Testing

The emulator passes all eight ROMs in the [Timendus CHIP-8 test suite](https://github.com/Timendus/chip8-test-suite):

- [x] 1 - CHIP-8 logo
- [x] 2 - IBM logo
- [x] 3 - Corax+ opcode test
- [x] 4 - Flags
- [x] 5 - Quirks
- [x] 6 - Keypad
- [x] 7 - Beep (sound timer verified; audio output not implemented)
- [ ] 8 - Scrolling (SUPER-CHIP instructions not implemented)

`tools/rom_maker.py` generates ROMs that deliberately trigger each error path: stack overflow, stack underflow, invalid `5XY1` / `9XY1` / `8XY9`, and unsupported `0NNN`. Run any of them to see the emulator report the error instead of crashing.

## Project layout

```
src/
├── main.c       main loop, timing, ROM loading
├── chip8.c/.h   CPU state, fetch/decode/execute, timers
└── display.c/.h SDL2 window, rendering, keyboard input
tools/
└── rom_maker.py generates error-handling test ROMs
roms/
└── README.md    where to get ROMs (none bundled)
```

## Roadmap

- [ ] Audio: square-wave beep while `sound_timer > 0`
- [ ] Debugger: step through instructions, inspect registers, memory and stack
- [ ] Redraw only when the display changes

## References

- [CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM) (Cowgod)
- [Timendus CHIP-8 test suite](https://github.com/Timendus/chip8-test-suite)
- [Guide to making a CHIP-8 emulator](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/) (Tobias V. Langhoff)

## License

[MIT](LICENSE) © Atharva Singh