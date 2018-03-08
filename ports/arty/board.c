#include <stdint.h>
#include <stdio.h>
#include "board.h"
#include "platform.h"   

int led_index[] = { RED_LED_OFFSET, GREEN_LED_OFFSET, BLUE_LED_OFFSET };

void led_init(void) {
  GPIO_REG(GPIO_INPUT_EN)    &= ~((0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET) | (0x1 << BLUE_LED_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_EN)   |=  ((0x1<< RED_LED_OFFSET)| (0x1<< GREEN_LED_OFFSET) | (0x1 << BLUE_LED_OFFSET)) ;
  GPIO_REG(GPIO_OUTPUT_VAL)  &=  ~((0x1 << BLUE_LED_OFFSET) | (0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET)) ;                                                                                           
}

void led_state(int led, int state) {
    // LED on
    if (state)
        GPIO_REG(GPIO_OUTPUT_VAL) |= (0x1 << led_index[led-1]);
    else 
        GPIO_REG(GPIO_OUTPUT_VAL) &= ~(0x1 << led_index[led-1]);
}

void led_toggle(int led) {
  GPIO_REG(GPIO_OUTPUT_VAL) ^= (0x1 << led_index[led-1]);
}

