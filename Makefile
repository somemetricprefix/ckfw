#
#             LUFA Library
#     Copyright (C) Dean Camera, 2013.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

TARGET       = build/ckfw

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = TEENSY2
F_CPU        = 16000000
F_USB        = $(F_CPU)

SRC          = $(wildcard src/*.cpp)
SRC         += $(wildcard src/usb/*.cpp src/usb/*.c)
SRC         += $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)

CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -Isrc/usb
CPP_FLAGS    = -std=c++11

LUFA_PATH    = lib/lufa/LUFA

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
