#include <unistd.h>
#include "py/mpconfig.h"

#include "machine.h"
/*
 * Core UART functions to implement for a port
 */

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    return mcReadchar();
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
        mcSendchar(*str++);
    }
}
