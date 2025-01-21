/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#define BUTTON_PIN BUTTON1
#define LED_PIN LED1

#define MAXIMUM_BUFFER_SIZE  
BufferedSerial serial_port(USBTX, USBRX);

int main()
{
    // Initialise the digital pin LED1 as an output
    #ifdef LED1
        DigitalOut led(LED1);
    #else
        bool led;
    #endif
    // Initialise the digital pin BUTTON1 as an input
    #ifdef LED1
        DigitalIn button(BUTTON1);
    #else
        bool button;
    #endif

    // Set desired properties (9600-8-N-1).
    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );

    // Application buffer to receive the data
    uint32_t text;

    while (true) {
        if(button == 1){
            led = 1;
            serial_port.write("Button Pressed\n", 16);
        } else {
            led = 0;
            serial_port.write("Button Released\n", 17);
        }
        ThisThread::sleep_for(100ms);
    }
}
