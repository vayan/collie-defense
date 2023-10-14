#---------------------------------------------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# LIBBUTANO is the main directory of butano library (https://github.com/GValiente/butano).
# PYTHON is the path to the python interpreter.
# SOURCES is a list of directories containing source code.
# INCLUDES is a list of directories containing extra header files.
# DATA is a list of directories containing binary data.
# GRAPHICS is a list of directories containing files to be processed by grit.
# AUDIO is a list of directories containing files to be processed by mmutil.
# ROMTITLE is a uppercase ASCII, max 12 characters text string containing the output ROM title.
# ROMCODE is a uppercase ASCII, max 4 characters text string containing the output ROM code.
# USERFLAGS is a list of additional compiler flags:
#     Pass -flto to enable link-time optimization.
#     Pass -O0 to improve debugging.
# USERLIBDIRS is a list of additional directories containing libraries.
#     Each libraries directory must contains include and lib subdirectories.
# USERLIBS is a list of additional libraries to link with the project.
# USERBUILD is a list of additional directories to remove when cleaning the project.
# EXTTOOL is an optional command executed before processing audio, graphics and code files.
#
# All directories are specified relative to the project directory where the makefile is found.
#---------------------------------------------------------------------------------------------------------------------
TARGET      :=  $(notdir $(CURDIR))
BUILD       :=  build
LIBBUTANO   :=  $(LIBBUTANO)
PYTHON      :=  python3
SOURCES     :=  src lib/src
INCLUDES    :=  include lib/include
DATA        :=
GRAPHICS    :=  graphics graphics/generated/levels graphics/fonts
AUDIO       :=  audio/music audio/sfx
ROMTITLE    :=  CollieDefense
ROMCODE     :=  CDGBA
USERFLAGS   := -DBN_CFG_AUDIO_MIXING_RATE=BN_AUDIO_MIXING_RATE_21_KHZ -DBN_CFG_SPRITES_MAX_SORT_LAYERS=1000
USERLIBDIRS :=
USERLIBS    :=
USERBUILD   :=
EXTTOOL     :=
DEFAULTLIBS := stdlib string

ifndef TYPE
	TYPE := DEBUG
endif

ifeq ($(TYPE),RELEASE)
	USERFLAGS := -O3 -DBN_CFG_LOG_ENABLED=false $(USERFLAGS)
endif

ifeq ($(TYPE), DEBUG)
	USERFLAGS := -DBN_CFG_LOG_ENABLED=true -Werror -O3 $(USERFLAGS)
endif

RAW_GRAPHICS_BG = $(wildcard graphics_raw/bg/*.aseprite)
RAW_GRAPHICS_SPRITE = $(wildcard graphics_raw/sprite/*.aseprite)

ALL_SFX = $(wildcard audio/sfx/*.wav)


#---------------------------------------------------------------------------------------------------------------------
# Export absolute butano path:
#---------------------------------------------------------------------------------------------------------------------
ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

#---------------------------------------------------------------------------------------------------------------------
# Include main makefile:
#---------------------------------------------------------------------------------------------------------------------
include $(LIBBUTANOABS)/butano.mak

ifneq (,$(wildcard ./.env))
	include ./.env
endif

levels:
	poetry run python ./tools/generate-level.py $(TYPE)

graphics: $(RAW_GRAPHICS_BG) $(RAW_GRAPHICS_SPRITE)

cl:
	@echo cleaning generated assets
	@rm -rf graphics/generated/levels
	@rm -f graphics/*.bmp

.PHONY: force
$(RAW_GRAPHICS_SPRITE): force
	aseprite --split-layers --batch $@ --color-mode indexed --sheet graphics/$(@F:.aseprite=.bmp) --sheet-type vertical

.PHONY: force
$(RAW_GRAPHICS_BG): force
	aseprite --batch $@ --color-mode indexed --sheet graphics/$(@F:.aseprite=.bmp)


$(ALL_SFX): force
	mv $@ $@.old.wav
	sox $@.old.wav -b 8 -r 22050 $@
	rm $@.old.wav

sfx: $(ALL_SFX)

run:
	make -j16 && mgba collie-defense.gba -l 4
