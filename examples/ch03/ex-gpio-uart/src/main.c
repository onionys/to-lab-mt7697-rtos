#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "hal.h"
#include "system_mt7687.h"

// -- 這裡一定要實作，因為 mtk 的library 中會用到
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    hal_uart_put_char(HAL_UART_0, ch);
    return ch;
}

int main(void)
{
    /* Configure system clock */
    top_xtal_init();
    SystemCoreClockUpdate();

    /* setup UART ---- START -----*/ 
    hal_uart_config_t uart_config;
    hal_pinmux_set_function(HAL_GPIO_0, HAL_GPIO_0_UART1_RTS_CM4);
    hal_pinmux_set_function(HAL_GPIO_1, HAL_GPIO_1_UART1_CTS_CM4);
    hal_pinmux_set_function(HAL_GPIO_2, HAL_GPIO_2_UART1_RX_CM4);
    hal_pinmux_set_function(HAL_GPIO_3, HAL_GPIO_3_UART1_TX_CM4);

    uart_config.baudrate = HAL_UART_BAUDRATE_115200;
    uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    uart_config.parity = HAL_UART_PARITY_NONE;
    hal_uart_init(HAL_UART_0, &uart_config);
    /* setup UART ---- END -----*/ 

    printf("------- START ----\r\n");
	printf("hello world\r\n");
	printf("------- END ------\r\n");

    while (1);
}

