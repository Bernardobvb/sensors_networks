/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"

#define BUTTON_PIN BUTTON1
#define LED_PIN LED1

int flag = 0;

DigitalOut led(LED1);
InterruptIn button(BUTTON1);

BufferedSerial serial_port(USBTX, USBRX);

Timer t;

void led_on()
{
    t.start();
    led = 1;
    flag = 1;
}

void led_off()
{
    t.stop();
    led = 0;
    flag = 1;
}

int main()
{
    // Set desired properties (9600-8-N-1).
    serial_port.set_baud(9600);
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    button.rise(&led_on);
    button.fall(&led_off);

    while (true) {
        if (led == 1)
        {
            if (flag == 1)
            {
                serial_port.write("Button Pressed\n", 16);
                flag = 0;
            }

        }
        else
        {
            if (flag == 1)
            {
                serial_port.write("Button Released\n", 17);
                ThisThread::sleep_for(100ms);
                printf("Pressed duration: %llu ms\n", std::chrono::duration_cast<std::chrono::milliseconds>(t.elapsed_time()).count());
                flag = 0;
                t.reset();
            }
        }
        ThisThread::sleep_for(100ms);
    }
}
