#include <stdio.h>
// #include "tracing.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

/****   Definindo TAGS para as Tarefas      ****/
static const char *TAGADC = "Test Task1";
static const char *TAGPWM = "Test Task2";

void vTaskADC(void *pvParameter);      /*Prototipo da TaskADC*/
TaskHandle_t xTaskADCHandle = NULL;    /*Handle da TaskADC*/

void vTaskPWM(void *pvParameter);      /*Prototipo da TaskPWM*/
TaskHandle_t xTaskPWMHandle = NULL;    /*Handle da TaskPWM*/

void app_main(void)
{
    xTaskCreate(vTaskADC, "Task ADC", 2048, NULL, 3, &xTaskADCHandle);
    xTaskCreate(vTaskPWM, "Task PWM", 2048, NULL, 3, &xTaskPWMHandle);
    traceTASK_CREATE(xTaskGetCurrentTaskHandle());
}

/***    Definicoes da TaskADC      ***/
void vTaskADC(void *pvParameter)
{
    while(1){
       // ESP_LOGI(TAGADC, "Fazendo Leitura ADC!!!");
        vTaskDelay(1000/portTICK_PERIOD_MS);
        // ESP_LOGI(TAGADC, "TO RODANDO");
        // for(int i = 0; i < 1000000; i++){
        //     /*Lendo ADC*/
        // }
        int64_t start_time = esp_timer_get_time();
        ESP_LOGI(TAGADC, "Tempo 1: %lld us", start_time);
    }
}

/***    Definicoes da TaskADC      ***/
void vTaskPWM(void *pvParameter)
{
    while(1){
        //ESP_LOGI(TAGPWM, "Atualizando PWM!!!");
        vTaskDelay(1000/portTICK_PERIOD_MS);
        int64_t start_time = esp_timer_get_time();
        ESP_LOGI(TAGPWM, "Tempo 2: %lld us", start_time);
        // for(int i = 0; i < 1000000; i++){
        //     /*Atualizando PWM*/
        // }
        // ESP_LOGI(TAGPWM, "TO RODANDO");
    }
}