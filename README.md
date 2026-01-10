# 🚀 vox8
A lightweight Chip-8 emulator for Linux, bringing classic 8-bit games to life with smooth performance and faithful emulation.

---

# 🧠 Vox8 Emulator

A **from-scratch CHIP-8 emulator** written in **C**, targeting **Linux**, with **SDL2** handling video, input, and audio.  
Minimal dependencies, traditional design, and a focus on correctness over gimmicks.

This project aims to be a clean reference-style emulator that runs real CHIP-8 software reliably.

---

## 📊 Repository Stats (Live)

![GitHub stars](https://img.shields.io/github/stars/Sparklight77/vox8?style=flat-square)
![GitHub forks](https://img.shields.io/github/forks/Sprklight77/vox8?style=flat-square)
![GitHub issues](https://img.shields.io/github/issues/Sparklight77/vox8?style=flat-square)
![GitHub license](https://img.shields.io/github/license/Sparklight77/vox8?style=flat-square)
![Last commit](https://img.shields.io/github/last-commit/Sparklight77/vox8?style=flat-square)


---

## 🎮 Overview

CHIP-8 is a simple interpreted language (kind of silly system orientated language) from the 1970s, but emulating it *correctly* still requires neat discipline.

This emulator implements:

- A full CHIP-8 CPU core
- Accurate memory, stack, and register behavior
- Proper 60Hz delay and sound timers
- XOR-based sprite rendering with collision detection
- SDL2-based framebuffer output
- SDL2 audio callback for real-time sound
- Keyboard input mapped to the original CHIP-8 layout

It is **Linux-only by design**. No platform abstraction layers, no portability gymnastics.

---

## 🖥️ Platform Support

| Platform | Status |
|--------|--------|
| Linux | ✅ Supported |
| Windows | ❌ Not supported |
| macOS | ❌ Not supported |
| Android | ☑️ with Termux|
| Web | ❌ Not a chance |

---

## ⚙️ Requirements
- git
- GCC or Clang
- SDL2 development libraries
- A Linux environment (X11 or Wayland)

---

## 🔨 Build Instructions
First clone this repository using:

```bash
git clone https://github.com/Sparklight77/vox8
```
Navigate to vox8 using `cd vox8`

then build the binary by running

```bash
make
```

The binary will be located at `vox8/vox8-{arch}`. The binary will be built depending on your GCC config or system default architecture.

---

## ▶️ Running a ROM
For Example to run a CHIP-8 ROM with vox8 32-bit arm build you could run:

```bash
./vox8-armv7a path/to/rom
```
just change vox8-{arch} according to builds.


---

⌨️ Key Mapping

Original CHIP-8 keypad mapped to a modern keyboard:

CHIP-8      Keyboard

1 2 3 C  →  1 2 3 4

4 5 6 D  →  Q W E R

7 8 9 E  →  A S D F

A 0 B F  →  Z X C V

ESC exits the emulator cleanly.


---

🧬 CPU & Architecture

Memory: 4 KB (0x000–0xFFF)

Program start: 0x200

Registers: V0–VF (8-bit)

Index register: I (16-bit)

Stack: 16 levels

Display: 64×32 monochrome framebuffer

Timers: 60Hz delay & sound timers


The emulator keeps CPU, timers, rendering, and input clearly separated to avoid timing bugs and logic bleed.


---

📜 Opcode Coverage (35 / 36)

This emulator implements 35 of the 36 standard CHIP-8 opcodes.

The only opcode intentionally omitted is the legacy SYS instruction, which is obsolete and unused by modern CHIP-8 software.

Opcode Availability

Opcode	Description	Status

00E0	Clear screen	✅
00EE	Return from subroutine	✅
0NNN	SYS addr	❌
1NNN	Jump	✅
2NNN	Call subroutine	✅
3XNN	Skip if equal (byte)	✅
4XNN	Skip if not equal (byte)	✅
5XY0	Skip if equal (reg)	✅
6XNN	Load byte	✅
7XNN	Add byte	✅
8XY0	Load reg	✅
8XY1	OR	✅
8XY2	AND	✅
8XY3	XOR	✅
8XY4	Add reg	✅
8XY5	Subtract	✅
8XY6	Shift right	✅
8XY7	Reverse subtract	✅
8XYE	Shift left	✅
9XY0	Skip if not equal	✅
ANNN	Load index	✅
BNNN	Jump with offset	✅
CXNN	Random	✅
DXYN	Draw sprite	✅
EX9E	Skip if key pressed	✅
EXA1	Skip if key not pressed	✅
FX07	Get delay timer	✅
FX0A	Wait for key	✅
FX15	Set delay timer	✅
FX18	Set sound timer	✅
FX1E	Add to index	✅
FX29	Font character	✅
FX33	BCD	✅
FX55	Store registers	✅
FX65	Load registers	✅



---

🔊 Audio

Square-wave beep generated via SDL audio callback

Triggered when SOUND_TIMET > 0

Runs independently of frame rendering


No busy-waiting, no hacks.

---


🎯 Project Goals

Accuracy over feature creep

Readable, maintainable C code

Faithful CHIP-8 behavior

Thanks to C for such a magic


This is meant to be understood, not just run. I made it as a learning project.


---

📄 License

MIT License.
Use it, fork it, study it, break it, fix it, just make sure to credit me :).

---

📒Notes

This emulator keeps things simple on purpose.
No shaders. No rewind. No fluff.

Just opcodes, pixels, timers, and a beep that sounds exactly like it should.

