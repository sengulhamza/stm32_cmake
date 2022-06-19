#include <stdint.h>
#include <stddef.h>
#include "stm32f4xx.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "drv/io.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} io_pins_t;

static io_pins_t leds[LED_MAX] = {
    {.port = GPIOD, .pin = GPIO_PIN_12},
    {.port = GPIOD, .pin = GPIO_PIN_13},
    {.port = GPIOD, .pin = GPIO_PIN_14},
    {.port = GPIOD, .pin = GPIO_PIN_15}
};

static void io_output_init(void)
{
    GPIO_InitTypeDef output;
    output.Mode = GPIO_MODE_OUTPUT_PP;
    output.Pull = GPIO_NOPULL;
    for (uint8_t i = 0; i < LED_MAX; i++) {
        output.Pin = leds[i].pin;
        HAL_GPIO_Init(leds[i].port, &output);
    }
}

void io_toggle_leds(leds_e led_num)
{
    HAL_GPIO_TogglePin(leds[led_num].port,  leds[led_num].pin);
}

void io_init(void)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
    io_output_init();
}
