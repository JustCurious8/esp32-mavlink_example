# ESP32-Mavlink Example

This is a simple MAVLink to UART interface example for ESP32 and its variants build for ESP-IDF framework that can allow communication between ESP32 and autopilot hardware(like pixhawk).

This example will send a SET_MESSAGE_INTERVAL mavlink command to autopilot when ESP32 boots up, which will then set the time interval between incoming messages from autopilot in microseconds, in this example, Mavlink Message ID 33 has been used to get only GLOBAL_POSITION_INT messages and just relative altitude of the autopilot will show up on ESP32 UART0 serial monitor.

For building instructions, simply follow the [get started](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) documentation of ESP-IDF for building this example, this example has been build using ESP-IDF version 4.2, tested on ESP32 NodeMCU and inspired from 
[c_uart_interface_example](https://github.com/mavlink/c_uart_interface_example), a simple MAVLink to UART interface example for *nix systems 

Note: This example has not been properly optimised, make sure to power-up the autopilot before and then power-up/reboot ESP32 to get relative altitude messages from autopilot on the serial monitor. Also take care of the UART pinouts according to ESP32 variant.

