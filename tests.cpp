#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include "clock.h"
#include "leds.h"
// static void clock_setup(void)
// {
// //     /* Enable high-speed clock at 120MHz */
// //     rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_120MHZ]);

// //     /* Enable GPIOD clock for LED & USARTs. */

// //     rcc_periph_clock_enable(RCC_GPIOD);
// //     rcc_periph_clock_enable(RCC_GPIOA);

// //     /* Enable clocks for USART2. */
// //     rcc_periph_clock_enable(RCC_USART2);
// }

// static void usart_setup(void)
// {
//     /* Setup USART2 parameters. */
//     usart_set_baudrate(USART2, 38400);
//     usart_set_databits(USART2, 8);
//     usart_set_stopbits(USART2, USART_STOPBITS_1);
//     usart_set_mode(USART2, USART_MODE_TX);
//     usart_set_parity(USART2, USART_PARITY_NONE);
//     usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

//     /* Finally enable the USART. */
//     usart_enable(USART2);
// }

// static void gpio_setup(void)
// {
//     /* Setup GPIO pin GPIO12 on GPIO port D for LED. */
//     gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

//     /* Setup GPIO pins for USART2 transmit. */
//     gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

//     /* Setup USART2 TX pin as alternate function. */
//     gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
// }


int main(int argc, char const *argv[])
{
    rcc_periph_clock_enable(RCC_GPIOF);
    int i = 0;
    bool ledsOn = true;
    while(true)
    {

        statusLed.set(ledsOn);
        activeLed.set(!ledsOn);
        i = (i+1)%1001;

        // led_status_set(ledsOn);
        // led_active_set(!ledsOn);
        ledsOn = !ledsOn;
        delay_ms(100);
    }

    // float scale = 0.25f, centerX = -0.5f, centerY = 0.0f;

    // clock_setup();
    // gpio_setup();
    // usart_setup();

    // while (1) {
    //     /* Blink the LED (PD12) on the board with each fractal drawn. */
    //     gpio_toggle(GPIOD, GPIO12);     /* LED on/off */
    //     // mandel(centerX, centerY, scale);    /* draw mandelbrot */

    //     /* Change scale and center */
    //     centerX += 0.175f * scale;
    //     centerY += 0.522f * scale;
    //     scale   *= 0.875f;

    //     usart_send_blocking(USART2, '\r');
    //     usart_send_blocking(USART2, '\n');
    // }
    while(1);
    return 0;
}
