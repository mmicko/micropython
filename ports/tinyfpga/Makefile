include ../../py/mkenv.mk

CROSS = 1

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk
CROSS_COMPILE = /opt/riscv32imc/bin/riscv32-unknown-elf-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

CFLAGS_RISCV = -mabi=ilp32 -march=rv32imc -ffreestanding -nostartfiles
CFLAGS = $(INC) -Wall -Werror -std=c99 $(COPT) $(CFLAGS_RISCV) -Os
LDFLAGS = -nostartfiles -Wl,-Bstatic,-T,sections.lds,--strip-debug,-Map=$@.map,--cref -ffreestanding  -mabi=ilp32 -march=rv32imc 

LIBS = -lm -lc -lgcc 

SRC_C = \
	main.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/mp-readline/readline.c

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(BUILD)/firmware.bin micropython.bin

$(BUILD)/start.o: start.s
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/firmware.elf: $(OBJ) $(BUILD)/start.o sections.lds
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $(BUILD)/start.o $(OBJ) $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(CROSS_COMPILE)objcopy  -O binary $(BUILD)/firmware.elf /dev/stdout | tail -c +331777 > $(BUILD)/firmware.bin
																					
# ---- TinyFPGA-B2 Breakout Board ----

micropython.blif: rtl/micropython.v rtl/spimemio.v rtl/simpleuart.v rtl/picosoc.v rtl/picorv32.v
	yosys -ql micropython.log -p 'synth_ice40 -top micropython -blif micropython.blif' $^

micropython.asc: rtl/micropython.pcf micropython.blif
	arachne-pnr -d 8k -P cm81 -o micropython.asc -p rtl/micropython.pcf micropython.blif

micropython.bin: micropython.asc
	icepack micropython.asc micropython.bin

prog: micropython.bin $(BUILD)/firmware.bin
	iceprog micropython.bin

prog_fw: $(BUILD)/firmware.bin
	iceprog -o 331776 $(BUILD)/firmware.bin

# ---- Clean ----
clean: myclean
myclean:
	rm -f micropython.blif micropython.log micropython.asc micropython.rpt micropython.bin

.PHONY: prog prog_fw clean

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C) mpconfigport.h
# Append any auto-generated sources that are needed by sources listed in
# SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

include $(TOP)/py/mkrules.mk
