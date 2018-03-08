include ../../py/mkenv.mk

FREEDOM_E_SDK ?= $(HOME)/src/freedom-e-sdk

RISCV_ARCH = rv32imac 
RISCV_ABI = ilp32 
BSP_BASE = $(FREEDOM_E_SDK)/bsp
BOARD ?= freedom-e300-arty
LINK_TARGET = flash

ENV_DIR = $(BSP_BASE)/env
PLATFORM_DIR = $(ENV_DIR)/$(BOARD)

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE = $(FREEDOM_E_SDK)/work/build/riscv-gnu-toolchain/riscv64-unknown-elf/prefix/bin/riscv64-unknown-elf-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(BSP_BASE)/include
INC += -I$(BSP_BASE)/drivers
INC += -I$(BSP_BASE)/env
INC += -I$(BSP_BASE)/env/$(BOARD)

ASM_SRCS += $(ENV_DIR)/start.S
ASM_SRCS += $(ENV_DIR)/entry.S

CFLAGS_RISCV = -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=medany -ffreestanding -nostartfiles -Wno-format
CFLAGS = $(INC) -Wall -Werror $(COPT) $(CFLAGS_RISCV) -Os
LDFLAGS = -L$(BSP_BASE)/env -nostartfiles -Wl,-Bstatic,-T,$(BSP_BASE)/env/$(BOARD)/$(LINK_TARGET).lds,--strip-debug,-Map=$@.map,--cref -ffreestanding -march=$(RISCV_ARCH) -mabi=$(RISCV_ABI) -mcmodel=medany 
LIBS = -lm -lc -lgcc 

SRC_C = \
	main.c \
	board.c \
	modpyb.c \
	modpybled.c \
	$(PLATFORM_DIR)/init.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o)) $(addprefix $(BUILD)/, $(ASM_SRCS:.S=.o))

all: micropython

micropython: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	$(Q)$(SIZE) $@


#############################################################
# This Section is for uploading a program to SPI Flash
#############################################################
OPENOCDCFG ?= $(FREEDOM_E_SDK)/bsp/env/$(BOARD)/openocd.cfg
OPENOCDARGS += -f $(OPENOCDCFG)
GDB_PORT ?= 3333
GDB_UPLOAD_ARGS ?= --batch

RISCV_OPENOCD ?= $(FREEDOM_E_SDK)/work/build/openocd/prefix/bin/openocd

GDB_UPLOAD_CMDS += -ex "set remotetimeout 240"
GDB_UPLOAD_CMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
GDB_UPLOAD_CMDS += -ex "monitor reset halt"
GDB_UPLOAD_CMDS += -ex "monitor flash protect 0 64 last off"
GDB_UPLOAD_CMDS += -ex "load"
GDB_UPLOAD_CMDS += -ex "monitor resume"
GDB_UPLOAD_CMDS += -ex "monitor shutdown"
GDB_UPLOAD_CMDS += -ex "quit"

upload:
	$(RISCV_OPENOCD) $(OPENOCDARGS) & \
	$(CROSS_COMPILE)gdb micropython $(GDB_UPLOAD_ARGS) $(GDB_UPLOAD_CMDS) && \
	echo "Successfully uploaded 'micropython' to $(BOARD)."

# ---- Clean ----
clean: myclean
myclean:
	rm -f micropython micropython.map

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) mpconfigport.h
# Append any auto-generated sources that are needed by sources listed in
# SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

include $(TOP)/py/mkrules.mk
