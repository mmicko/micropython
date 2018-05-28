include ../../py/mkenv.mk

# qstr definitions (must come before including py.mk)
QSTR_DEFS = qstrdefsport.h

# include py core make definitions
include $(TOP)/py/py.mk

XC32 = /opt/microchip/xc32/v2.05
CROSS_COMPILE = $(XC32)/bin/xc32-

PART = 32MX370F512H

INC += -I.
INC += -I$(TOP)
INC += -I$(BUILD)
INC += -I$(XC32)/include

CFLAGS = $(INC) -D_SUPPRESS_PLIB_WARNING -Wall  -Werror -std=gnu99 -nostdlib -mprocessor=$(PART) $(COPT) 

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CFLAGS += -O0 -ggdb
else
CFLAGS += -O1 -DNDEBUG
endif

LDFLAGS = -mprocessor=32MX370F512H -Wl,--defsym=__MPLAB_BUILD=1,--defsym=_min_heap_size=128,--defsym=_min_stack_size=128,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="build/micropython.map",--memorysummary,build/memoryfile.xml
LIBS = -L$(XC32)/lib -lc -lm  -legacy-libc

SRC_C = \
	main.c \
	board.c \
	hw.c \
	disp.c \
	vt100.c \
	badge.c \
	basic_mphal.c \
	modpyb.c \
	modpybled.c \
	modpybswitch.c \
	lib/utils/pyexec.c \
	lib/utils/sys_stdio_mphal.c \
	lib/mp-readline/readline.c \

SRC_S = \
#	gchelper.s \

OBJ = $(PY_O) $(addprefix $(BUILD)/, $(SRC_C:.c=.o) $(SRC_S:.s=.o))

# List of sources for qstr extraction
SRC_QSTR += $(SRC_C)
# Append any auto-generated sources that are needed by sources listed in
# SRC_QSTR
SRC_QSTR_AUTO_DEPS +=

all: $(BUILD)/firmware.hex

$(BUILD)/firmware.hex: $(BUILD)/firmware.elf
	$(ECHO) "Create $@"
	$(Q)$(CROSS_COMPILE)bin2hex $<

$(BUILD)/firmware.elf: $(OBJ)
	$(ECHO) "LINK $@"
	$(Q)$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)
	$(Q)size $@

$(PY_BUILD)/gc.o: CFLAGS += -O1
$(PY_BUILD)/vm.o: CFLAGS += -O1
$(PY_BUILD)/nlr%.o: CFLAGS += -O1

include $(TOP)/py/mkrules.mk
