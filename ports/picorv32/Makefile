include ../../py/mkenv.mk

CROSS = 1

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk
CROSS_COMPILE = /opt/riscv32ic/bin/riscv32-unknown-elf-

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)

CFLAGS_RISCV = -mabi=ilp32 -march=rv32ic -ffreestanding
CFLAGS = $(INC) -Wall -Werror -std=c99 $(COPT) $(CFLAGS_RISCV) 
LDFLAGS = -lgcc  -nostdlib -Wl,-Bstatic,-T,sections.lds,--strip-debug,-Map=$@.map,--cref -ffreestanding  -mabi=ilp32 -march=rv32ic 

LIBS =

SRC_C = \
	main.c \
	divmod.c \
	udivmod.c \
	udivmodsi4.c \
	lib/utils/stdout_helpers.c \
	lib/utils/pyexec.c \
	lib/libc/string0.c \
	lib/mp-readline/readline.c

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o))

all: $(BUILD)/firmware.bin micropython.bin

$(BUILD)/start.o: start.s
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/setjmp.o: setjmp.S
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/muldi3.o: muldi3.S
	$(Q)$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD)/firmware.elf: $(OBJ) $(BUILD)/start.o $(BUILD)/muldi3.o $(BUILD)/setjmp.o sections.lds
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $(BUILD)/start.o $(OBJ) $(BUILD)/setjmp.o $(BUILD)/muldi3.o  $(LIBS)
	$(Q)$(SIZE) $@

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(CROSS_COMPILE)objcopy  -O binary $(BUILD)/firmware.elf /dev/stdout | tail -c +1048577 > $(BUILD)/firmware.bin

# ---- iCE40 UP5k Breakout Board ----

micropython.blif: rtl/micropython.v rtl/spimemio.v rtl/simpleuart.v rtl/picosoc.v rtl/up_spram.v rtl/ip_wrapper.v rtl/picorv32.v
	yosys -ql micropython.log -p 'synth_ice40 -top micropython -blif micropython.blif' $^

micropython.asc: rtl/micropython.pcf micropython.blif
	arachne-pnr -d 5k -o micropython.asc -p rtl/micropython.pcf micropython.blif

micropython.bin: micropython.asc
	icepack micropython.asc micropython.bin

prog: micropython.bin $(BUILD)/firmware.bin
	iceprog micropython.bin
	iceprog -o 1M $(BUILD)/firmware.bin

prog_fw: $(BUILD)/firmware.bin
	iceprog -o 1M $(BUILD)/firmware.bin

# ---- Clean ----
clean: myclean
myclean:
	rm -f micropython.blif micropython.log micropython.asc micropython.rpt micropython.bin

.PHONY: prog prog_fw clean


include $(TOP)/py/mkrules.mk
