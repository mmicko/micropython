include ../../py/mkenv.mk

FREEDOM_E_SDK ?= $(HOME)/src/freedom-e-sdk
RISCV_OPENOCD_PATH ?= /opt/riscv-openocd

BSP_BASE = $(FREEDOM_E_SDK)/bsp
BOARD ?= sifive-hifive1
#BOARD ?= sifive-hifive1-revb
#BOARD ?= freedom-e310-arty
#BOARD ?= coreip-e20-arty
#BOARD ?= coreip-e20-rtl
#BOARD ?= coreip-e21-arty
#BOARD ?= coreip-e21-rtl
#BOARD ?= coreip-e24-arty
#BOARD ?= coreip-e24-rtl
#BOARD ?= coreip-e31-arty
#BOARD ?= coreip-e31-rtl
#BOARD ?= coreip-e34-arty
#BOARD ?= coreip-e34-rtl
#BOARD ?= coreip-e76-arty
#BOARD ?= coreip-e76-rtl
#BOARD ?= coreip-s51-arty
#BOARD ?= coreip-s51-rtl
#BOARD ?= coreip-s54-arty
#BOARD ?= coreip-s54-rtl
#BOARD ?= coreip-s76-arty
#BOARD ?= coreip-s76-rtl

PLATFORM_DIR = $(BSP_BASE)/$(BOARD)
CONFIGURATION = release

# There must be a settings makefile fragment in the BSP's board directory.
ifeq ($(wildcard $(PLATFORM_DIR)/settings.mk),)
$(error Unable to find BSP for $(BOARD), expected to find $(PLATFORM_DIR)/settings.mk)
endif
include $(PLATFORM_DIR)/settings.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

ifeq ($(RISCV_PATH),)
CROSS_COMPILE ?= riscv64-unknown-elf-
else
CROSS_COMPILE = $(RISCV_PATH)/bin/riscv64-unknown-elf-
endif

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(PLATFORM_DIR)
INC += -I$(FREEDOM_E_SDK)/freedom-metal

CFLAGS_RISCV = -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL) -ffunction-sections -fdata-sections -Wno-format
CFLAGS = $(INC) -Wall -Werror $(COPT) $(CFLAGS_RISCV) -Os
LDFLAGS = -Wl,--gc-sections -Wl,-Map,$@.map -nostartfiles -nostdlib -L$(PLATFORM_DIR)/install/lib/$(CONFIGURATION) -T$(PLATFORM_DIR)/metal.default.lds  -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=$(RISCV_CMODEL)
LDFLAGS += -Xlinker --defsym=__stack_size=0x800 -Xlinker --defsym=__heap_size=0x000
LIBS = -Wl,--start-group -lc -lm -lgcc -lmetal -lmetal-gloss -Wl,--end-group

SRC_C = \
	main.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: micropython

micropython: $(OBJ)
	$(Q)$(MAKE) -C $(FREEDOM_E_SDK) TARGET=$(BOARD) CONFIGURATION=$(CONFIGURATION)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	$(Q)$(SIZE) $@

upload: micropython
	$(FREEDOM_E_SDK)/scripts/upload --elf micropython --openocd $(RISCV_OPENOCD_PATH)/bin/openocd --gdb $(CROSS_COMPILE)gdb --openocd-config $(BSP_BASE)/$(BOARD)/openocd.cfg
	echo "Successfully uploaded 'micropython' to $(BOARD)."

# ---- Clean ----
clean: myclean
myclean:
	$(Q)$(MAKE) -C $(FREEDOM_E_SDK) TARGET=$(BOARD) CONFIGURATION=$(CONFIGURATION) clean
	rm -f micropython micropython.map

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) mpconfigport.h
# Append any auto-generated sources that are needed by sources listed in
# SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

include $(TOP)/py/mkrules.mk
