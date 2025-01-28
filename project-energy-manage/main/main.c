#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "driver/uart.h"

// Driver da GPIO com funções estendidas para o modo sleep
#include "driver/rtc_io.h"

#define BOTAO 0

void button_check();      /*Prototipo da TaskADC*/

void app_main()
{
    // Configuração do pino como entrada
    gpio_config_t config = {
        .pin_bit_mask = BIT64(BOTAO),               //gpio pin to use for wakeup
        .mode = GPIO_MODE_INPUT,                    //set gpio pin to input mode
        .pull_down_en = false,                      //disable pull down resistor
        .pull_up_en = false,                        //disable pull up resistor
        .intr_type = GPIO_INTR_DISABLE              //disable interrupt
    };
    gpio_config(&config);
    gpio_wakeup_enable(BOTAO, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();

    // Configurando o Sleep Timer (em microsegundos)
    // uint64_t tempo_sono = 2 * (60 * 1000000); // VALOR * (1 MINUTO)
    uint64_t tempo_sono = (10 * 1000000); // VALOR * (1 MINUTO)
    esp_sleep_enable_timer_wakeup(tempo_sono);

    int64_t sleep_time_ms = 0;
    int64_t sleep_time_total = 0;

  while(true)
  {

    button_check();

    printf("Entrando em modo Light Sleep\n");
    
    // Configura o modo sleep somente após completar a escrita na UART para finalizar o printf
    uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM);

    int64_t tempo_antes_de_dormir = esp_timer_get_time();
    // Entra em modo Light Sleep
    esp_light_sleep_start();
    int64_t tempo_apos_acordar = esp_timer_get_time();
    
    sleep_time_ms = (tempo_apos_acordar - tempo_antes_de_dormir) / 1000;
    sleep_time_total += sleep_time_ms;
    
    printf("Dormiu por %lld ms\n", sleep_time_ms);
    
    // Pega a causa
    esp_sleep_wakeup_cause_t causa = esp_sleep_get_wakeup_cause();

    if (causa == ESP_SLEEP_WAKEUP_TIMER){
        printf("FAZ A MEDIDA E SALVA LOCAL\n");   
    }else{
        printf("SOBRE O SERVIDOR HTTP\n");
    }

    printf("Sono total %lld ms\n", sleep_time_total);
    if (sleep_time_total >= 40000){
        printf("MANDA PRA O MQTT \n");
        sleep_time_total = 0;
    }

  }

}

void button_check(){
    if (rtc_gpio_get_level(BOTAO) == 0)
    {
        // printf("Aguardando soltar o botão ... \n");
        do
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        } while (rtc_gpio_get_level(BOTAO) == 0);
    }
}
