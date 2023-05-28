#include "pico/stdlib.h"
#include <stdio.h>

// State machine states
enum states {
    RED,
    GREEN,
};

// State machine control variables
bool onEntry = true;
enum states state = RED;

// Hardware definitions
const uint LED_PIN_RED = 17;
const uint LED_PIN_GREEN = 16;
const uint BUTTON_1 = 14;

// Function prototypes
void stateMachine();
void SetState(enum states s);

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN_RED);
    gpio_init(LED_PIN_GREEN);
    gpio_init(BUTTON_1);

    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_set_dir(BUTTON_1, GPIO_IN);

    gpio_pull_up(BUTTON_1);

    while (true) {
        stateMachine();
        sleep_ms(25); // Guarantee cheap button debouncing
    }
}

void stateMachine()
{
    static uint64_t timeout_us = 0;

    switch (state) {
    case RED: {
        if (onEntry) {
            onEntry = false;
            // Turn red led on and green led off
            gpio_put(LED_PIN_RED, 1);
            gpio_put(LED_PIN_GREEN, 0);

            printf("Entry RED\n");
        }

        if (!gpio_get(BUTTON_1)) {
            // If button is pressed go to state GREEN
            SetState(GREEN);

            printf("Button pressed\n");
        }
        break;
    }
    case GREEN: {
        if (onEntry) {
            onEntry = false;
            // Turn green led on, red led off and set timeout to 3 seconds in the future
            gpio_put(LED_PIN_RED, 0);
            gpio_put(LED_PIN_GREEN, 1);
            timeout_us = time_us_64() + 3000000;

            printf("Entry GREEN\n");
        }

        if (time_us_64() >= timeout_us) {
            // Time has expired - go back to state RED
            SetState(RED);

            printf("Timeout\n");
        }
        break;
    }
    }
}

void SetState(enum states s)
{
    state = s;
    onEntry = true;
}