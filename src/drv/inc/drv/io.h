#ifndef _IO_H_
#define _IO_H_

#include <stdbool.h>

typedef enum {
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_MAX
} leds_e;

typedef enum {
    BUTTON_1,
    BUTTON_MAX
} buttons_e;

void io_init(void);
void io_toggle_leds(leds_e led_num);
void io_set_led(leds_e led_num);
void io_reset_led(leds_e led_num);
uint8_t io_read_input(buttons_e button_num);

#endif
