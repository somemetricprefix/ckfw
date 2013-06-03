MCU          = atmega32u4
ARCH         = AVR8
BOARD        = TEENSY2
F_CPU        = 16000000
F_USB        = $(F_CPU)

CPP_FLAGS   += -std=c++11 -include $(CONFIG_H) -I$(CKFW_PATH)/src

include $(CKFW_PATH)/src/core/core.mk

LUFA_PATH = $(CKFW_PATH)/lib/lufa/LUFA
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
