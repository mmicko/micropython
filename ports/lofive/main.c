#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "platform.h"

#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "lib/utils/pyexec.h"

// Receive single character
char mp_hal_stdin_rx_chr(void) {
    int32_t c = 0;
    while (c<=0) {
        c = UART0_REG(UART_REG_RXFIFO);
    }
    return c & 0xff;
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) = *str++;
    }
}
   
void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        qstr source_name = lex->source_name;
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, true);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}

static char *stack_top;
static char heap[1024*8];

static const char sifive_msg[] = "\n\r\
\n\r\
                SIFIVE, INC.\n\r\
\n\r\
         5555555555555555555555555\n\r\
        5555                   5555\n\r\
       5555                     5555\n\r\
      5555                       5555\n\r\
     5555       5555555555555555555555\n\r\
    5555       555555555555555555555555\n\r\
   5555                             5555\n\r\
  5555                               5555\n\r\
 5555                                 5555\n\r\
5555555555555555555555555555          55555\n\r\
 55555           555555555           55555\n\r\
   55555           55555           55555\n\r\
     55555           5           55555\n\r\
       55555                   55555\n\r\
         55555               55555\n\r\
           55555           55555\n\r\
             55555       55555\n\r\
               55555   55555\n\r\
                 555555555\n\r\
                   55555\n\r\
                     5\n\r\
\n\r";

int main(int argc, char **argv) {
    int stack_dummy;
    // Initialization block taken from led_fade.c
    {
        // Make sure the HFROSC is on before the next line:
        PRCI_REG(PRCI_HFROSCCFG) |= ROSC_EN(1);
        // Run off 16 MHz Crystal for accuracy. Note that the
        // first line is 
        PRCI_REG(PRCI_PLLCFG) = (PLL_REFSEL(1) | PLL_BYPASS(1));
        PRCI_REG(PRCI_PLLCFG) |= (PLL_SEL(1));
        // Turn off HFROSC to save power
        PRCI_REG(PRCI_HFROSCCFG) &= ~(ROSC_EN(1));
        
        // Configure UART to print
        GPIO_REG(GPIO_OUTPUT_VAL) |= IOF0_UART0_MASK;
        GPIO_REG(GPIO_OUTPUT_EN)  |= IOF0_UART0_MASK;
        GPIO_REG(GPIO_IOF_SEL)    &= ~IOF0_UART0_MASK;
        GPIO_REG(GPIO_IOF_EN)     |= IOF0_UART0_MASK;

        // 115200 Baud Rate
        UART0_REG(UART_REG_DIV) = 138;
        UART0_REG(UART_REG_TXCTRL) = UART_TXEN;
        UART0_REG(UART_REG_RXCTRL) = UART_RXEN;

        // Wait a bit to avoid corruption on the UART.
        // (In some cases, switching to the IOF can lead
        // to output glitches, so need to let the UART
        // reciever time out and resynchronize to the real 
        // start of the stream.
        volatile int i=0;
        while(i < 10000){i++;}
    }
    
    stack_top = (char*)&stack_dummy;
    	
	printf(sifive_msg);

    gc_init(heap, heap + sizeof(heap));
    mp_init();
    pyexec_friendly_repl();
    mp_deinit();
    return 0;
}

void gc_collect(void) {
	printf("gc_collect\n");
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_raise_OSError(MP_ENOENT);
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val) {
    while (1);
}

void NORETURN __fatal_error(const char *msg) {
    while (1);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif

