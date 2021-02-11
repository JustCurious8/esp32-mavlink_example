#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "mavlink/common/mavlink.h"
#include "interface.h"

#define ECHO_TEST_TXD GPIO_NUM_17
#define ECHO_TEST_RXD GPIO_NUM_16
#define ECHO_TEST_RTS UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS UART_PIN_NO_CHANGE

#define ECHO_UART_PORT_NUM      UART_NUM_2
#define ECHO_UART_BAUD_RATE     115200
#define ECHO_TASK_STACK_SIZE    10240

#define BUF_SIZE (1024)


extern "C" {
	void app_main(void);
}

void app_main(void)
{

    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));


    Autopilot_Interface api=Autopilot_Interface();

    api.set_message_interval();

    while (1) {

    api.read_messages();

    }

} 
