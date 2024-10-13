
#include <stdio.h>
// freeRTOS Libraries
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ESP32 Libraries
#include "driver/gpio.h"
#include "esp_log.h"
#include "driver/ledc.h"

//possible pins map
#define LED1_PIN 21     //GPIO21
#define LED2_PIN 33     //GPIO33
#define BUTTON_PIN 2    //GPIO2

#define LED3_PIN 25     //GPIO25
#define BUTTON2_PIN 34    //GPIO34

const static char *TAG = "BUTTON TEST";   //for Log messages

//main function
void app_main(void)
{
    //configure LED_PIN_1 GPIO as input
    gpio_reset_pin(LED1_PIN);                          //reset pin and set as GPIO
    gpio_set_direction(LED1_PIN, GPIO_MODE_OUTPUT);   //set LED_PIN_1 as output
    //configure LED_PIN_1 GPIO as input
    gpio_reset_pin(LED2_PIN);                          //reset pin and set as GPIO
    gpio_set_direction(LED2_PIN, GPIO_MODE_OUTPUT);   //set LED_PIN_1 as output
    //configure LED_PIN_1 GPIO as input
    gpio_reset_pin(LED3_PIN);                          //reset pin and set as GPIO
    gpio_set_direction(LED3_PIN, GPIO_MODE_OUTPUT);   //set LED_PIN_1 as output

    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);   //set pull-up resistor
    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON2_PIN);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON2_PIN, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON2_PIN, GPIO_PULLUP_ONLY);   //set pull-up resistor
    
    // CONFIGURA O TIMER E CANAL PARA O LED3
    ledc_timer_config_t timer_conf = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 1,
            .duty_resolution = LEDC_TIMER_10_BIT,
            .clk_cfg = LEDC_AUTO_CLK
        };
    ledc_timer_config(&timer_conf);
    ledc_channel_config_t channel_config = {
        .channel = LEDC_CHANNEL_1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LED3_PIN,
        .duty = 511
    };    
    ledc_channel_config(&channel_config);
    
    // DEFINICAO DE VARIAVEIS AUXILIARES
    int led_status = 0; // define o estado do led1
    int led2_status = 0; // define o estado do led2
    bool can_change = true; // permite ou n alterar o valor do led1
    bool can_change2 = true; // permite ou n alterar o valor do led2

    while(true) //infinite loop
    {
        // FAZ A LEITURA DOS BOTOES
        int new_state = gpio_get_level(BUTTON_PIN);   //read button state and save in new_state variable
        int new_state2 = gpio_get_level(BUTTON2_PIN);
        
        // VERIFICA SE TEM OU NAO QUE ALTERAR O ESTADO DO LED
        if(new_state == 1 && can_change){
            led_status = (led_status == 0) ? 1 : 0;
            can_change = false;
        }
        if(new_state == 0){can_change = true;}

        if(new_state2 == 1 && can_change2){
            led2_status = (led2_status == 0) ? 1 : 0;
            can_change2 = false;
        }
        if(new_state2 == 0){can_change2 = true;}
        
        // SETA OS ESTADOS DOS LEDS
        gpio_set_level(LED1_PIN, led_status);
        gpio_set_level(LED2_PIN, led2_status);

        ESP_LOGI(TAG, "Bt1: %d | Bt2: %d | LED1: %d | LED2: %d", new_state, new_state2, led_status, led2_status);

        vTaskDelay(10 / portTICK_PERIOD_MS);       // delay for 100 ms
    }
}
