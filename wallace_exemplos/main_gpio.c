
#include <stdio.h>
// freeRTOS Libraries
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ESP32 Libraries
#include "driver/gpio.h"
#include "esp_log.h"

//possible pins map
#define LED1_PIN 21     //GPIO21
#define LED2_PIN 33     //GPIO33
#define BUTTON_PIN 2    //GPIO2

const static char *TAG = "BUTTON TEST: ";   //for Log messages

//main function
void app_main(void)
{
    //configure LED_PIN_1 GPIO as input
    gpio_reset_pin(LED1_PIN);                          //reset pin and set as GPIO
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);   //set LED_PIN_1 as output

    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);   //set pull-up resistor

    while(true) //infinite loop
    {
        int new_state = gpio_get_level(BUTTON_PIN);   //read button state and save in new_state variable

        //---------------------
        // Start the input pin treatment
        //---------------------


        vTaskDelay(100 / portTICK_PERIOD_MS);       // delay for 100 ms
    }
}
