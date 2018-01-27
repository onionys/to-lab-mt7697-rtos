#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hal.h"
#include "system_mt7687.h"
#include "top.h"

static hal_eint_number_t irq_num;

// ------------------------------------------
// --- UART0 初始化用的程式碼 --- START 
// ------------------------------------------
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif
{
    hal_uart_put_char(HAL_UART_0, ch);
    return ch;
}

static void config_uart(void)
{
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
}
// ------------------------------------------
// --- UART0 初始化用的程式碼 --- END
// ------------------------------------------

// ------------------------------------------
// -- 中斷發生時要呼叫的 function 
// ------------------------------------------
static void my_irq_handler(void *data)
{
#ifdef HAL_EINT_FEATURE_MASK
    hal_eint_mask(irq_num);
#endif

	printf("What the fuck\r\n");
    printf("Received eint: %d !\r\n", irq_num);

#ifdef HAL_EINT_FEATURE_MASK
    hal_eint_unmask(irq_num);
#endif
}


int main(void)
{
    /* -- Clock Config -- */
    top_xtal_init();
    SystemCoreClockUpdate();

    /* 初始化 UART0 */
    config_uart();

	/* -- 設定 interrupt -- */
    __enable_irq();
    __enable_fault_irq();

    hal_eint_config_t eint_config;
    eint_config.trigger_mode = HAL_EINT_EDGE_RISING;
    eint_config.debounce_time = 5;

	/*
	 * 設定 GPIO 37
	 * 功能: GPIO 打開中斷功能
	 * I/O 方向: INPUT
	 * 拉伸電阻: disable
	 * */
    hal_gpio_init(HAL_GPIO_37);
    hal_pinmux_set_function(HAL_GPIO_37, HAL_GPIO_37_EINT20);
    hal_gpio_set_direction(HAL_GPIO_37, HAL_GPIO_DIRECTION_INPUT);
    hal_gpio_disable_pull(HAL_GPIO_37);

	// 全域變數記錄 Interrupt Number
    irq_num = HAL_EINT_NUMBER_20;

#ifdef HAL_EINT_FEATURE_MASK
    hal_eint_mask(irq_num);
#endif
    hal_eint_init(irq_num, &eint_config);
    hal_eint_register_callback(irq_num, my_irq_handler, NULL);
#ifdef HAL_EINT_FEATURE_MASK
    hal_eint_unmask(irq_num);
#endif

	printf("----- START -----\r\n");
    while (1);
}
