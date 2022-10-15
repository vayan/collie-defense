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
PYTHON      :=  python
SOURCES     :=  src lib/src
INCLUDES    :=  include lib/include
DATA        :=
GRAPHICS    :=  graphics graphics/generated/levels graphics/fonts
AUDIO       :=  audio
ROMTITLE    :=  CollieDefence
ROMCODE     :=  CDGBA
USERFLAGS   := -DBN_CFG_MEMORY_MAX_EWRAM_ALLOC_ITEMS=64
USERLIBDIRS :=
USERLIBS    :=
USERBUILD   :=
EXTTOOL     :=

RAW_GRAPHICS = $(wildcard graphics_raw/*.aseprite)

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
	poetry run python ./tools/generate-level.py

clean_graphics:
	@rm graphics/*.bmp

graphics: clean_graphics $(RAW_GRAPHICS)

cl: clean
	@echo cleaning generated assets
	@rm -rf graphics/generated/levels

.PHONY: force
$(RAW_GRAPHICS): force
	aseprite --split-layers --batch $@ --color-mode indexed --sheet graphics/$(@F:.aseprite=.bmp) --sheet-type vertical
