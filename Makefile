# Ashen Frontier DS - BlocksDS-style starter Makefile
# You can override BLOCKSDS/WONDERFUL_TOOLCHAIN on the command line if needed.

BLOCKSDS            ?= /opt/blocksds/core
WONDERFUL_TOOLCHAIN ?= /opt/wonderful

export BLOCKSDS
export WONDERFUL_TOOLCHAIN

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

BLOCKSDS_MAKEFILE := $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile

ifeq (,$(wildcard $(BLOCKSDS_MAKEFILE)))
$(error Missing BlocksDS makefile: $(BLOCKSDS_MAKEFILE). Set BLOCKSDS to your BlocksDS core path, e.g. `make BLOCKSDS=/path/to/blocksds/core`)
endif

# This include is provided by BlocksDS. If your setup differs,
# replace it with the correct shared rules include.
include $(BLOCKSDS_MAKEFILE)
