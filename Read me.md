diff --git a/README.md b/README.md
index dd1cfdb800c23011382c04f2360e257ffa8145b9..e5bd83b58ccdbe44b70751b764bd9967e790756c 100644
--- a/README.md
+++ b/README.md
@@ -24,47 +24,52 @@ This is not a fake ROM wrapper. It is a **real DS homebrew source project scaffo
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
-3. Adjust the `Makefile` paths if your setup differs.
-4. Build the ROM.
-5. Import the resulting `.nds` into Delta on iPhone.
+3. Build the ROM with `make` (the Makefile now auto-detects common `/opt` and `/workspace` toolchain paths).
+4. Import `ashen_frontier_ds.nds` into Delta on iPhone.
+
+#### Build commands
+```bash
+make clean
+make -j$(nproc)
+```
 
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
 - `DESIGN.md` – level and feature plan
 - `icon.bmp` – placeholder ROM icon
 
 ## One honest limitation
 The current prototype is **mechanically RPG-like**, but visually minimalist so it stays realistic for a first DS homebrew pass. It captures the structure and feel, not modern 3D spectacle. The DS is a brave little brick, not a PS5 in cosplay.
