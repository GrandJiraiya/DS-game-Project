# Ashen Frontier DS

A tiny Nintendo DS homebrew **one-zone action RPG vertical slice** designed for Delta (`.nds`) rather than the web.

## Vibe
Think **WoW quest loop + Crimson Desert travel/combat mood**, but scaled down to what makes sense for a DS homebrew demo:
- one outpost hub
- one field route
- one miniboss
- one chest
- one quest turn-in
- one exit beacon that unlocks when the miniboss is down

This is not a fake ROM wrapper. It is a **real DS homebrew source project scaffold** intended to build into an `.nds` ROM with **BlocksDS/libnds**.

## What works in the starter code
- top screen HUD / quest text
- bottom screen map
- D-pad movement
- A to interact / open chest / trigger exit
- B to attack adjacent enemy
- X to toggle quest log
- simple HP / XP / gold / potion stats
- quest state machine
- miniboss gate to finish the level
- touch support hook scaffold (easy to extend)

## Controls
- **D-pad**: move
- **A**: interact
- **B**: attack
- **X**: quest log on/off
- **START**: restart run

## Build target
This project is shaped for the modern DS homebrew stack:
- **BlocksDS + libnds**
- output target: **`.nds`** for use in Delta

## Why BlocksDS
BlocksDS is the current DS homebrew-friendly path and its docs note that it includes most NDS functionality from older devkitARM projects, with a clearer build system and easy Makefile customization.

## Build notes
You will need a real DS homebrew toolchain installed on your machine. This container did **not** have BlocksDS/devkitARM installed, so I couldn't honestly compile a ROM here.

### Expected flow
1. Install **BlocksDS** (or devkitPro/devkitARM if you prefer legacy flow).
2. Open this folder in your terminal.
3. Build with your BlocksDS path if needed:
   - `make BLOCKSDS=/path/to/blocksds/core`
4. Confirm an `.nds` ROM was produced:
   - `find . -maxdepth 3 -name "*.nds"`
5. Import the resulting `.nds` into Delta on iPhone.

### Quick test checklist before Delta import
- Boot test in an emulator (melonDS or Delta desktop equivalent if available).
- Confirm controls: D-pad movement, `A` interact, `B` attack, `X` quest log, `START` restart.
- Kill the boss, interact with exit beacon, and verify completion text.
- Rebuild from clean state (`make clean && make ...`) to ensure reproducibility.

## Suggested next upgrades
- sprite art instead of ASCII/tile chars
- loot rarity and equipment slots
- dodge roll / stamina
- quest journal menu with item descriptions
- village NPC portraits on top screen
- basic sound via maxmod
- save file to FAT/NitroFS
- second zone or dungeon room

## Project structure
- `source/main.c` – full gameplay loop
- `Makefile` – BlocksDS-style starter build file
- `docs/DESIGN.md` – level and feature plan and vertical-slice intent
- `assets/icon.bmp` – placeholder ROM icon

## One honest limitation
The current prototype is **mechanically RPG-like**, but visually minimalist so it stays realistic for a first DS homebrew pass. It captures the structure and feel, not modern 3D spectacle. The DS is a brave little brick, not a PS5 in cosplay.
