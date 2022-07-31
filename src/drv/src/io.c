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

static io_pins_t buttons[BUTTON_MAX] = {
    {.port = GPIOA, .pin = GPIO_PIN_0},
};

static void io_input_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef input;
    input.Mode = GPIO_MODE_INPUT;
    input.Pull = GPIO_NOPULL;
    for (uint8_t i = 0; i < BUTTON_MAX; i++) {
        input.Pin = buttons[i].pin;
        HAL_GPIO_Init(buttons[i].port, &input);
    }
}

static void io_output_init(void)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();
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

void io_set_led(leds_e led_num)
{
    HAL_GPIO_WritePin(leds[led_num].port,  leds[led_num].pin, GPIO_PIN_SET);
}

void io_reset_led(leds_e led_num)
{
    HAL_GPIO_WritePin(leds[led_num].port,  leds[led_num].pin, GPIO_PIN_RESET);
}

uint8_t io_read_input(buttons_e button_num)
{
    return (uint8_t)HAL_GPIO_ReadPin(buttons[button_num].port, buttons[button_num].pin);
}

void io_init(void)
{
    io_output_init();
    io_input_init();
}
