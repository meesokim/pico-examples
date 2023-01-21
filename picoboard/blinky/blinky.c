/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/cyw43_arch.h"

const uint DOT_PERIOD_MS = 100;

const char *morse_letters[] = {
        ".-",    // A
        "-...",  // B
        "-.-.",  // C
        "-..",   // D
        ".",     // E
        "..-.",  // F
        "--.",   // G
        "....",  // H
        "..",    // I
        ".---",  // J
        "-.-",   // K
        ".-..",  // L
        "--",    // M
        "-.",    // N
        "---",   // O
        ".--.",  // P
        "--.-",  // Q
        ".-.",   // R
        "...",   // S
        "-",     // T
        "..-",   // U
        "...-",  // V
        ".--",   // W
        "-..-",  // X
        "-.--",  // Y
        "--.."   // Z
};

void ledled(int ledpin, int on)
{
#ifndef PICO_DEFAULT_LED_PIN
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
#else
    gpio_put(led_pin, on);
#endif
}
void put_morse_letter(uint led_pin, const char *pattern) {
    for (; *pattern; ++pattern) {
        ledled(led_pin, 1);
        if (*pattern == '.')
            sleep_ms(DOT_PERIOD_MS);
        else
            sleep_ms(DOT_PERIOD_MS * 3);
        ledled(led_pin, 0);
        sleep_ms(DOT_PERIOD_MS * 1);
    }
    sleep_ms(DOT_PERIOD_MS * 2);
}

void put_morse_str(uint led_pin, const char *str) {
    for (; *str; ++str) {
        if (*str >= 'A' && *str <= 'Z') {
            put_morse_letter(led_pin, morse_letters[*str - 'A']);
        } else if (*str >= 'a' && *str <= 'z') {
            put_morse_letter(led_pin, morse_letters[*str - 'a']);
        } else if (*str == ' ') {
            sleep_ms(DOT_PERIOD_MS * 4);
        }
    }
}

int main() {
#ifndef PICO_DEFAULT_LED_PIN
// #warning picoboard/blinky example requires a board with a regular LED
    const uint LED_PIN = CYW43_WL_GPIO_LED_PIN;
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
#else
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
#endif
    while (true) {
        put_morse_str(LED_PIN, "Hello world");
        sleep_ms(1000);
    }
}
