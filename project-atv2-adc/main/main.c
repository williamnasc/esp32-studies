#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "esp_log.h"  // Biblioteca para logs
#include "driver/ledc.h"
#include "driver/gpio.h"

// Definir uma tag para os logs
static const char *TAG = "ADC_EXAMPLE";

#define LED_PIN 25     //GPIO25

void app_main(void)
{   
    gpio_reset_pin(LED_PIN);                          //reset pin and set as GPIO
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);   //set LED_PIN_1 as output

    // CONFIGURA O TIMER E CANAL PARA O LED3
    ledc_timer_config_t timer_conf = {
            .speed_mode = LEDC_LOW_SPEED_MODE,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 60,
            .duty_resolution = LEDC_TIMER_10_BIT,
            .clk_cfg = LEDC_AUTO_CLK
        };
    ledc_timer_config(&timer_conf);
    ledc_channel_config_t channel_config = {
        .channel = LEDC_CHANNEL_1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LED_PIN,
        .duty = 511
    };    
    ledc_channel_config(&channel_config);

    int adc_raw;  // Variável para armazenar o valor bruto do ADC
    int new_duty;

    // Configurações do ADC One-Shot
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,  // Configura a unidade ADC1
    };

    // INICIA UNIDADE ADC1
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    // CONFIGURA O CANAL ADC
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,  // Resolução do ADC
        .atten = ADC_ATTEN_DB_11,          // Atenuação de 11dB (para leitura de até ~3.6V)
    };

    // CONFIGURA CANAL 6 DO ADC1
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config));

    while (1)
    {
        // LE O CANAL
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &adc_raw));

        new_duty = (adc_raw * 1024) / 4096;

        ESP_LOGI(TAG, "Valor bruto do ADC: %d, Duty: %d", adc_raw, new_duty);

        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, new_duty);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
