#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

static const char *TAG = "QUEUE_EXAMPLE";

QueueHandle_t queue;  // Handle da fila

// Tarefa que envia dados para a fila
void sender_task(void *pvParameters)
{
    int count = 0;
    while (1) {
        count++;
        // Envia o valor de 'count' para a fila
        if (xQueueSend(queue, &count, portMAX_DELAY) == pdPASS) {
            ESP_LOGI(TAG, "Enviado para a fila: %d", count);
        } else {
            ESP_LOGE(TAG, "Falha ao enviar para a fila");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));  // Aguarda 1 segundo
    }
}

// Tarefa que recebe dados da fila
void receiver_task(void *pvParameters)
{
    int received_value;
    while (1) {
        // Espera receber um valor da fila
        if (xQueueReceive(queue, &received_value, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "Recebido da fila: %d", received_value);
        } else {
            ESP_LOGE(TAG, "Falha ao receber da fila");
        }
    }
}

void app_main(void)
{
    // Cria uma fila que pode conter até 10 inteiros
    queue = xQueueCreate(10, sizeof(int));

    if (queue == NULL) {
        ESP_LOGE(TAG, "Falha ao criar a fila");
        return;
    }

    // Cria as tarefas de envio e recepção
    xTaskCreate(sender_task, "Sender Task", 2048, NULL, 5, NULL);
    xTaskCreate(receiver_task, "Receiver Task", 2048, NULL, 5, NULL);
}
