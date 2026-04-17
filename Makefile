# Ashen Frontier DS - BlocksDS-style starter Makefile
# Adjust BLOCKSDS path as needed for your machine.

NAME          := ashen_frontier_ds
GAME_TITLE    := Ashen Frontier DS
GAME_SUBTITLE := One-Zone RPG Slice
GAME_AUTHOR   := Crash / OpenAI
GAME_ICON     := icon.bmp

SOURCEDIRS    := source
INCLUDEDIRS   := include
GFXDIRS       :=
BINDIRS       :=
AUDIODIRS     :=
NITROFSDIR    :=

# Use a standard ARM7 core from BlocksDS.
ARM7ELF       := $(BLOCKSDS)/sys/arm7/main_core/arm7_maxmod.elf

LIBS          := -lnds9
LIBDIRS       := $(BLOCKSDS)/libs/libnds

# This include is provided by BlocksDS. If your setup differs,
# replace it with the correct shared rules include.
include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile
