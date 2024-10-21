
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_oneshot.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define BUTTON_PIN1 14    //GPIO32
#define BUTTON_PIN2 35    //GPIO35
#define BUTTON_PIN3 34    //GPIO34


static const char *TAG = "dac_oneshot_example";     

void app_main(void)
{
    uint32_t val = 0;         
    
    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN1);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN1, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN1, GPIO_PULLUP_ONLY);   //set pull-up resistor
    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN2);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN2, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN2, GPIO_PULLUP_ONLY);   //set pull-up resistor
    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN3);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN3, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN3, GPIO_PULLUP_ONLY);   //set pull-up resistor
    

    /* DAC oneshot init */
    dac_oneshot_handle_t chan0_handle;      
    dac_oneshot_config_t chan0_cfg = {      
        .chan_id = DAC_CHAN_0,              
    };
    ESP_ERROR_CHECK(dac_oneshot_new_channel(&chan0_cfg, &chan0_handle));    
    
    ESP_LOGI(TAG, "DAC oneshot example start.");                            

   while(1){

        
        // Criando uma rampa - inclinacao positiva
        for(val = 0;val<255;val++){
            ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, val));
            // int valor = val;
            // ESP_LOGI(TAG, "VAL: %d", valor);

            // FAZ A LEITURA DOS BOTOES
            int read_bt1 = gpio_get_level(BUTTON_PIN1);   //read button state and save in new_state variable
            int read_bt2 = gpio_get_level(BUTTON_PIN2);   //read button state and save in new_state variable
            int read_bt3 = gpio_get_level(BUTTON_PIN3);   //read button state and save in new_state variable
            
            ESP_LOGI(TAG, "BTs: %d  %d  %d", read_bt1, read_bt2, read_bt3);


            vTaskDelay(pdMS_TO_TICKS(100));
        }
        /*
            Terminar as lógicas para geração das ondas
        */
   }
}
