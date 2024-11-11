#include <stdio.h>
#include <string.h>
// #include "tracing.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"

/****   Definindo TAGS para as Tarefas      ****/

// TAGS DE LOG
static const char *TAG_READER = "Test READER";
static const char *TAG_WRITER = "Test WRITER";
static const char *TAGLOG = "LOG GERAL";

// SEMAFOROS
SemaphoreHandle_t semaphore_counting;
SemaphoreHandle_t semaphore_mutex;

// DADOS
char data_matrix[10][20] = {"", "", ""};

// METODOS AUXILIARES
void read_data();

// TAREFAS DE LEITURA
void vTask_reader(void *pvParameter);      
TaskHandle_t xTask_readerHandle = NULL;
void vTask_reader2(void *pvParameter);      
TaskHandle_t xTask_readerHandle2 = NULL;

// TAREFAS DE ESCRITA
void vTask_writer_temp(void *pvParameter);      
TaskHandle_t xTask_writerHandle_temp = NULL;
void vTask_writer_umi(void *pvParameter);      
TaskHandle_t xTask_writerHandle_umi = NULL;
void vTask_writer_vel(void *pvParameter);      
TaskHandle_t xTask_writerHandle_vel = NULL;
void vTask_writer_pes(void *pvParameter);      
TaskHandle_t xTask_writerHandle_pes = NULL;
void vTask_writer_dis(void *pvParameter);      
TaskHandle_t xTask_writerHandle_dis = NULL;    

void app_main(void)
{
    semaphore_counting = xSemaphoreCreateCounting( 10, 0 );
    semaphore_mutex = xSemaphoreCreateMutex();

    xTaskCreate(vTask_reader, "Task reader", 2048, NULL, 2, &xTask_readerHandle);
    xTaskCreate(vTask_reader2, "Task reader2", 2048, NULL, 2, &xTask_readerHandle2);
    xTaskCreate(vTask_writer_temp, "Task writer_temp", 2048, NULL, 2, &xTask_writerHandle_temp);
    xTaskCreate(vTask_writer_umi, "Task writer_umi", 2048, NULL, 2, &xTask_writerHandle_umi);
    xTaskCreate(vTask_writer_vel, "Task writer_vel", 2048, NULL, 2, &xTask_writerHandle_vel);
    xTaskCreate(vTask_writer_pes, "Task writer_pes", 2048, NULL, 2, &xTask_writerHandle_pes);
    xTaskCreate(vTask_writer_dis, "Task writer_dis", 2048, NULL, 2, &xTask_writerHandle_dis);
    traceTASK_CREATE(xTaskGetCurrentTaskHandle());
}

void show_data(){
    ESP_LOGI(TAGLOG, "-----------------------");
    for (int i = 0; i < 10; i++){
        ESP_LOGI(TAGLOG, "%s", data_matrix[i]);
    }
    ESP_LOGI(TAGLOG, "-----------------------");
}

void vTask_reader(void *pvParameter)
{
    while(1){
        ESP_LOGI(TAG_READER, "Leitura 1 !!!");
        
        UBaseType_t teste = uxSemaphoreGetCount(semaphore_counting);
        ESP_LOGI(TAG_READER, "VALOR: %u", teste);
        
        if (xSemaphoreTake(semaphore_counting, portMAX_DELAY)) {
            ESP_LOGI(TAG_READER, "PEGANDO VALOR DO SEMAFORO!!!");

            if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                
                show_data();
                strcpy(data_matrix[uxSemaphoreGetCount(semaphore_counting)], "LIDO");

                xSemaphoreGive(semaphore_mutex);    
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask_reader2(void *pvParameter)
{
    while(1){
        ESP_LOGI(TAG_READER, "Leitura 2 !!!");
        
        UBaseType_t teste = uxSemaphoreGetCount(semaphore_counting);
        ESP_LOGI(TAG_READER, "VALOR: %u", teste);
        
        if (xSemaphoreTake(semaphore_counting, portMAX_DELAY)) {
            ESP_LOGI(TAG_READER, "PEGANDO VALOR DO SEMAFORO!!!");

            if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                
                show_data();
                strcpy(data_matrix[uxSemaphoreGetCount(semaphore_counting)], "LIDO");

                xSemaphoreGive(semaphore_mutex);    
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask_writer_temp(void *pvParameter)
{
    int total_escritas = 0;
    while(1){
        // ESP_LOGI(TAG_WRITER, "Tarefa Escrita !!!");

        // SO ATUA ENQUANTO N TIVER ESCRITO TUDO 
        if (total_escritas < 5){
            // VERIFICA SE TEM ESPACO LIVRE NO ARRAY PELO SEMAFORO CONTADOR
            if (uxSemaphoreGetCount(semaphore_counting) < 10){

                // TODO > SO CONTINUAR SE O MUTEX TIVER HABILITADO
                if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                    // PEGA A CONTAGEM DO SEMAFORO
                    UBaseType_t count_value = uxSemaphoreGetCount(semaphore_counting);
                    // ESP_LOGI(TAG_WRITER, "CONT INDEX: %u", count_value);

                    // ESCREVE NO ARRAY
                    strcpy(data_matrix[count_value], "TEMPERATURA");
                    // ADICIONA PRO SEMAFORO COUNTING
                    xSemaphoreGive(semaphore_counting);
                    total_escritas += 1;
                    // SOLTA O MUTEX
                    xSemaphoreGive(semaphore_mutex);
                }
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask_writer_umi(void *pvParameter)
{
    int total_escritas = 0;
    while(1){
        // ESP_LOGI(TAG_WRITER, "Tarefa Escrita !!!");

        // SO ATUA ENQUANTO N TIVER ESCRITO TUDO 
        if (total_escritas < 5){
            // VERIFICA SE TEM ESPACO LIVRE NO ARRAY PELO SEMAFORO CONTADOR
            if (uxSemaphoreGetCount(semaphore_counting) < 10){

                // TODO > SO CONTINUAR SE O MUTEX TIVER HABILITADO
                if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                    // PEGA A CONTAGEM DO SEMAFORO
                    UBaseType_t count_value = uxSemaphoreGetCount(semaphore_counting);
                    // ESP_LOGI(TAG_WRITER, "CONT INDEX: %u", count_value);

                    // ESCREVE NO ARRAY
                    strcpy(data_matrix[count_value], "UMIDADE");
                    // ADICIONA PRO SEMAFORO COUNTING
                    xSemaphoreGive(semaphore_counting);
                    total_escritas += 1;
                    // SOLTA O MUTEX
                    xSemaphoreGive(semaphore_mutex);
                }
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask_writer_vel(void *pvParameter)
{
    int total_escritas = 0;
    while(1){
        // ESP_LOGI(TAG_WRITER, "Tarefa Escrita !!!");

        // SO ATUA ENQUANTO N TIVER ESCRITO TUDO 
        if (total_escritas < 5){
            // VERIFICA SE TEM ESPACO LIVRE NO ARRAY PELO SEMAFORO CONTADOR
            if (uxSemaphoreGetCount(semaphore_counting) < 10){

                // TODO > SO CONTINUAR SE O MUTEX TIVER HABILITADO
                if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                    // PEGA A CONTAGEM DO SEMAFORO
                    UBaseType_t count_value = uxSemaphoreGetCount(semaphore_counting);
                    // ESP_LOGI(TAG_WRITER, "CONT INDEX: %u", count_value);

                    // ESCREVE NO ARRAY
                    strcpy(data_matrix[count_value], "VELOCIDADE");
                    // ADICIONA PRO SEMAFORO COUNTING
                    xSemaphoreGive(semaphore_counting);
                    total_escritas += 1;
                    // SOLTA O MUTEX
                    xSemaphoreGive(semaphore_mutex);
                }
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask_writer_pes(void *pvParameter)
{
    int total_escritas = 0;
    while(1){
        // ESP_LOGI(TAG_WRITER, "Tarefa Escrita !!!");

        // SO ATUA ENQUANTO N TIVER ESCRITO TUDO 
        if (total_escritas < 5){
            // VERIFICA SE TEM ESPACO LIVRE NO ARRAY PELO SEMAFORO CONTADOR
            if (uxSemaphoreGetCount(semaphore_counting) < 10){

                // TODO > SO CONTINUAR SE O MUTEX TIVER HABILITADO
                if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                    // PEGA A CONTAGEM DO SEMAFORO
                    UBaseType_t count_value = uxSemaphoreGetCount(semaphore_counting);
                    // ESP_LOGI(TAG_WRITER, "CONT INDEX: %u", count_value);

                    // ESCREVE NO ARRAY
                    strcpy(data_matrix[count_value], "PESO");
                    // ADICIONA PRO SEMAFORO COUNTING
                    xSemaphoreGive(semaphore_counting);
                    total_escritas += 1;
                    // SOLTA O MUTEX
                    xSemaphoreGive(semaphore_mutex);
                }
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask_writer_dis(void *pvParameter)
{
    int total_escritas = 0;
    while(1){
        // ESP_LOGI(TAG_WRITER, "Tarefa Escrita !!!");

        // SO ATUA ENQUANTO N TIVER ESCRITO TUDO 
        if (total_escritas < 5){
            // VERIFICA SE TEM ESPACO LIVRE NO ARRAY PELO SEMAFORO CONTADOR
            if (uxSemaphoreGetCount(semaphore_counting) < 10){

                // TODO > SO CONTINUAR SE O MUTEX TIVER HABILITADO
                if(xSemaphoreTake(semaphore_mutex, portMAX_DELAY)){
                    // PEGA A CONTAGEM DO SEMAFORO
                    UBaseType_t count_value = uxSemaphoreGetCount(semaphore_counting);
                    // ESP_LOGI(TAG_WRITER, "CONT INDEX: %u", count_value);

                    // ESCREVE NO ARRAY
                    strcpy(data_matrix[count_value], "DISTANCIA");
                    // ADICIONA PRO SEMAFORO COUNTING
                    xSemaphoreGive(semaphore_counting);
                    total_escritas += 1;
                    // SOLTA O MUTEX
                    xSemaphoreGive(semaphore_mutex);
                }
            }
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}