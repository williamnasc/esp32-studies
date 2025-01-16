#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"
#include "esp_log.h"  // Biblioteca para logs
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_timer.h"

// Definir uma tag para os logs
static const char *TAG = "ADC_EXAMPLE";

void app_main(void)
{   

    int adc_raw;  // Variável para armazenar o valor bruto do ADC
    int ldr_value;  
    int pot_value;
    int temp_value;

    int64_t start_time;
    int64_t step_time;

    bool enable_ldr = false;

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
        .atten = ADC_ATTEN_DB_12,          // Atenuação de 11dB (para leitura de até ~3.6V)
    };

    // CONFIGURA CANAL 6 DO ADC1 (GPIO 34)
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_6, &config));
    // CONFIGURA CANAL 7 DO ADC1 (GPIO 35)
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_7, &config));
    // CONFIGURA CANAL 3 DO ADC1 (GPIO 39)
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_4, &config));

    while (1)
    {
        // LE O CANAL
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_6, &adc_raw));
        ldr_value = adc_raw;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_7, &adc_raw));
        pot_value = adc_raw;
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_4, &adc_raw));
        temp_value = adc_raw;
        
        if (ldr_value < 3000){
            enable_ldr = true;
        }
        if (enable_ldr == true && ldr_value > 3000){
                start_time = esp_timer_get_time();
                ESP_LOGI(TAG, "TEMPO: %lld", start_time);
                enable_ldr = false;
        }


        ESP_LOGI(TAG, "Valor bruto do ADC: %d | %d | %d | %d", adc_raw, ldr_value, pot_value, temp_value);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}