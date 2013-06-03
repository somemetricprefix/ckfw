CORE         = $(CKFW_PATH)/src/core

SRC         += $(wildcard $(CORE)/*.cpp)
SRC         += $(wildcard $(CORE)/usb/*.cpp $(CORE)/usb/*.c)
SRC         += $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)

CC_FLAGS    += -DUSE_LUFA_CONFIG_HEADER -I$(CKFW_PATH)/src/core/usb
