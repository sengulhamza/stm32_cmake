#ifndef _IO_H_
#define _IO_H_

typedef enum {
    LED_1,
    LED_2,
    LED_3,
    LED_4,
    LED_MAX
} leds_e;

void io_init(void);
void io_toggle_leds(leds_e led_num);

#endif
