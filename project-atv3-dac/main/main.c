
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/dac_oneshot.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define BUTTON_PIN1 36    //GPIO32
#define BUTTON_PIN2 35    //GPIO35
#define BUTTON_PIN3 34    //GPIO34


static const char *TAG = "dac_example";     

void app_main(void)
{
    uint32_t val = 0;
    uint32_t output = 0;
    double resolucao = 255.0;         
    
    int valor_b1 = 0;
    int valor_b2 = 0;
    int valor_b3 = 0; 

    bool can_change1 = true;
    bool can_change2 = true;

    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN1);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN1, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN1, GPIO_PULLDOWN_ONLY);   //set pull-up resistor
    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN2);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN2, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN2, GPIO_PULLDOWN_ONLY);   //set pull-up resistor
    //configure button_PIN GPIO as input
    gpio_reset_pin(BUTTON_PIN3);                         //reset pin and set as GPIO
    gpio_set_direction(BUTTON_PIN3, GPIO_MODE_INPUT);    //set BUTTON_PIN as input
    gpio_set_pull_mode(BUTTON_PIN3, GPIO_PULLDOWN_ONLY);   //set pull-up resistor
    

    /* DAC oneshot init */
    dac_oneshot_handle_t chan0_handle;      
    dac_oneshot_config_t chan0_cfg = {      
        .chan_id = DAC_CHAN_0,              
    };
    ESP_ERROR_CHECK(dac_oneshot_new_channel(&chan0_cfg, &chan0_handle));    
    
    ESP_LOGI(TAG, "DAC oneshot example start.");                            

   while(1){
        
        // FAZ A LEITURA DOS BOTOES
        int read_bt1 = gpio_get_level(BUTTON_PIN1);   //read button state and save in new_state variable
        int read_bt2 = gpio_get_level(BUTTON_PIN2);   //read button state and save in new_state variable
        int read_bt3 = gpio_get_level(BUTTON_PIN3);   //read button state and save in new_state variable
        
        if (read_bt1){
            if (can_change1){
                valor_b1 = 1 + valor_b1;
                can_change1 = false;
                vTaskDelay(pdMS_TO_TICKS(10));
                }
        }else{
            can_change1 = true;
            if (valor_b1 >=4){valor_b1 = 0;}
        }


        if (read_bt2){
            if (can_change2){valor_b2 = 1 + valor_b2;}
        }else{can_change2 = true;
            if (valor_b2 >=10){valor_b2 = 1;}
        }

        if (read_bt3){
            valor_b3 = 1 + valor_b2;
        }else{
            if (valor_b3 >=4){
                valor_b3 = 0;
            }
        }

        // valor_b1 = 2;
        // valor_b2 = 3;

        // Criando uma rampa - inclinacao positiva
        for(val = 0; val<resolucao; val++){
            // ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, val));
            // int valor = val;
            // ESP_LOGI(TAG, "VAL: %d", valor);

            double valor_normalizado = val/resolucao;
            
            double rampa = valor_normalizado;
            double rad = 2.0 * M_PI * valor_normalizado;
            double seno = 0.5 + sin(rad)/2;
            double quadrada = 0.0;
            if (valor_normalizado > 0.50){
                quadrada = 1.0;
            }else{
                quadrada = 0.0;
            }
            double triangular = 0.0;
            if (valor_normalizado > 0.50){
                triangular = 0.5 + (1-valor_normalizado);
            }else{
                triangular = 0.5 + valor_normalizado ;
            }
            

            switch (valor_b1) {
                case 1:
                    output = quadrada*255;
                    break;
                case 2:
                    output = seno*255;
                    break;
                case 3:
                    output = triangular*255;
                    break;
                case 0:
                    output = rampa*255;
                    break;
                default:
                    break;
            }

            
            // ESP_LOGI(TAG, "BTs: %d  %d  %d | ramp: %.5f | seno: %.5f | quad: %.5f | triang: %.5f | V1: %d", read_bt1, read_bt2, read_bt3, rampa, seno, quadrada, triangular, valor_b1);
            

            ESP_ERROR_CHECK(dac_oneshot_output_voltage(chan0_handle, output));

            vTaskDelay(pdMS_TO_TICKS(10));
        }
        ESP_LOGI(TAG, "BTs: %d  %d  %d | V1: %d | V2: %d",read_bt1, read_bt2, read_bt3, valor_b1, valor_b2);
        // vTaskDelay(pdMS_TO_TICKS(10));
        /*
            Terminar as lógicas para geração das ondas
        */
   }
}
