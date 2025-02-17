/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "htu21d/htu21d.h"

#define BUTTON_PIN BUTTON1
#define LED1_PIN LED1
#define LED2_PIN LED2

int flag = 0;
float freq = .2;

DigitalOut led1(LED1);
DigitalOut led2(LED2);
InterruptIn button(BUTTON1);

BufferedSerial serial_port(USBTX, USBRX);

Timer t;
Ticker flipper;

namespace {
#define WAIT 1000ms
}

using namespace sixtron;

void flip()
{
    led2 = !led2;
}

void led_on()
{
    t.start();
    led1 = 1;
    flag = 1;
    freq += 0.1;
    flipper.detach();
    flipper.attach(&flip, freq);
}

void led_off()
{
    t.stop();
    led1 = 0;
    flag = 1;
}

int main()
{
    HTU21D::ErrorType err;
    htud21d_user_reg_t reg;
    HTU21D sensor(P1_I2C_SDA, P1_I2C_SCL);
    double result;

    sensor.soft_reset();

    printf("\n\n----------------------\n");
    printf("--- HTU21D(F) Demo ---\n");

    err = sensor.read_user_register(&reg);

    if (err == HTU21D::ErrorType::Ok) {
        printf("User register status:\n");
        printf("Battery low: %s\nHeater: %s\nOTP reload: %s\n\n",
                reg.end_of_battery ? "TRUE" : "FALSE",
                reg.enable_heater ? "ON" : "OFF",
                reg.disable_otp_reload ? "DISABLED" : "ENABLED");

        reg.resolution = MeasurementResolution::RH_12_Temp_14;
        sensor.write_user_register(&reg);
    }

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
        printf("-----\n");
        err = sensor.read_humidity(&result);
        if (err == HTU21D::ErrorType::Ok) {
            printf("Relative humidity: %f%%\n", result);
        }

        if (led1 == 1)
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
