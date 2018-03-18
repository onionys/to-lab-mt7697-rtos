# MTK HAL UART 的筆記

HAL_UART_0 --> GPIO_2 RX, GPIO_3 TX

	hal_gpio_init(HAL_GPIO_2);
	hal_pinmux_set_function(HAL_GPIO_2, HAL_GPIO_2_UART1_RX_CM4);

	hal_gpio_init(HAL_GPIO_3);
	hal_pinmux_set_function(HAL_GPIO_3, HAL_GPIO_3_UART1_RX_CM4);


