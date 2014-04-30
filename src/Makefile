TARGET = keyboard
MCU    = atmega32u4
ARCH   = AVR8
BOARD  = TEENSY2
F_CPU  = 16000000
F_USB  = $(F_CPU)

SRC += $(wildcard *.c) $(wildcard usb/*.c)
SRC += $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)

C_FLAGS += -DUSE_LUFA_CONFIG_HEADER -Iusb

all:

LUFA_PATH = ../lib/lufa/LUFA
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
