include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# MicroPython feature configurations
MICROPY_ROM_TEXT_COMPRESSION ?= 1

# include py core make definitions
include $(TOP)/py/py.mk

CROSS_COMPILE ?= m68k-elf-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

CFLAGS_M68K = -mcpu=68000 -march=68000 -mtune=68000
CFLAGS = $(INC) -Wall -Werror -std=c99 -nostartfiles $(CFLAGS_M68K) $(COPT)
LDFLAGS = -nostartfiles -Wl,-T,rosco_m68k_program.ld,-Map=$@.map,--cref $(CFLAGS_M68K)

CSUPEROPT = -Os # save some code space
ASFLAGS=-Felf -m68000 -quiet $(DEFINES)
AS=vasmm68k_mot

# Tune for Debugging or Optimization
CFLAGS += -Os -DNDEBUG

LIBS =

SRC_C = \
	main.c \
	uart_core.c \
	lib/utils/printf.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/libc/string0.c \
	lib/mp-readline/readline.c \
	$(BUILD)/_frozen_mpy.c \

OBJ = $(BUILD)/init.o $(BUILD)/kinit.o $(BUILD)/machine.o $(PY_CORE_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o)) 

all: $(BUILD)/firmware.bin

$(BUILD)/init.o: init.S
	$(Q)$(AS) $(ASFLAGS) -o $@ $<
$(BUILD)/machine.o: machine.S
	$(Q)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD)/_frozen_mpy.c: frozentest.mpy $(BUILD)/genhdr/qstrdefs.generated.h
	$(ECHO) "MISC freezing bytecode"
	$(Q)$(TOP)/tools/mpy-tool.py -f -q $(BUILD)/genhdr/qstrdefs.preprocessed.h -mlongint-impl=none $< > $@

$(BUILD)/firmware.bin: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

load: $(BUILD)/firmware.bin
	ckermit -i -l /dev/ttyUSB0 -b 9600 -s $(BUILD)/firmware.bin
	screen /dev/ttyUSB0 9600

include $(TOP)/py/mkrules.mk
