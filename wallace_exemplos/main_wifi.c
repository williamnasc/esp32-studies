#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

/*----------------------------------------------------------------------------------------*
------------------------ PROGRAMA PARA WI-FI SCAN UTILIZANDO COMO BASE O -----------------*
---------------------------O EXEMPLO WI-FI SCAN DO GITHUB DA ESPRESSIF--------------------*
------------------------------------------------------------------------------------------*/

#define DEFAULT_SCAN_LIST_SIZE 10       //Define o tamanho da lista para realizar o scan

static const char *TAG = "WiFi Scan";   //TAG para verificação na hora do lOG

char *getAuthModeName(wifi_auth_mode_t wifi_auth_mode){
    switch(wifi_auth_mode){
        case WIFI_AUTH_OPEN:
            return "WIFI_AUTH_OPEN";
        case WIFI_AUTH_WEP:
            return "WIFI_AUTH_WEP";
        case WIFI_AUTH_WPA_PSK:
            return "WIFI_AUTH_WPA_PSK";
        case WIFI_AUTH_WPA2_PSK:
            return "WIFI_AUTH_WPA2_PSK";
        case WIFI_AUTH_WPA_WPA2_PSK:
            return "WIFI_AUTH_WPA_WPA2_PSK";
        case WIFI_AUTH_WPA2_ENTERPRISE:
            return "WIFI_AUTH_WPA2_ENTERPRISE";
        case WIFI_AUTH_WPA3_PSK:
            return "WIFI_AUTH_WPA3_PSK";
        case WIFI_AUTH_WPA2_WPA3_PSK:
            return "WIFI_AUTH_WPA2_WPA3_PSK";   
        case WIFI_AUTH_WAPI_PSK:
            return "WIFI_AUTH_WAPI_PSK";
        case WIFI_AUTH_OWE:
            return "WIFI_AUTH_OWE";
        case WIFI_AUTH_MAX:
            return "WIFI_AUTH_MAX";  
        case WIFI_AUTH_WPA3_EXT_PSK_MIXED_MODE:
            return "WIFI_AUTH_WPA3_EXT_PSK_MIXED_MODE"; 
        case WIFI_AUTH_WPA3_ENT_192:
            return "WIFI_AUTH_WPA3_ENT_192";
        case WIFI_AUTH_WPA3_EXT_PSK:
            return "WIFI_AUTH_WPA3_EXT_PSK";                                   
    }
    return "NOT FOUND";
}


void wifi_scan(void){
    ESP_ERROR_CHECK(esp_netif_init());                              //Por meio do esp_err_t as funções do ESP-IDF retornam mensagens ESP_OK ou ESP_FAIL. Podemos checar com ESP_ERROR_CHECK
    ESP_ERROR_CHECK(esp_event_loop_create_default());               //Criação de um grupo de eventos padrão
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();   //Criação de uma instancia para netif. Trabalhando no modo station
    assert(sta_netif);                                              //Verificar se foi possível criar

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();   //Criando uma estrutura de configuração para o WiFi. Utilizando a configuração básica indicada pela espressif
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));                  //Inicializando o drive WiFi e checando a inicialização

    uint16_t number = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];   //Array para salvar as redes disponíveis
    uint16_t ap_count = 0;                              //Pegar a contagem de quantos ap foi encontrado
    memset(ap_info, 0, sizeof(ap_info));                //Limpando o array


    wifi_scan_config_t wifi_scan_config = {.show_hidden = true};    //Dentro da estrutura do config indicado pela espressif podemos escolher alguns parametros, entre eles o mostrar redes ocultas

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_wifi_scan_start(&wifi_scan_config, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

    ESP_LOGI(TAG, "TOTAL APS scanned = %u", ap_count);

    for(int i = 0; (i < DEFAULT_SCAN_LIST_SIZE) && (i < ap_count); i++){
        ESP_LOGI(TAG, "SSID \t%s ", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI \t%d", ap_info[i].rssi);
        ESP_LOGI(TAG, "Channel \t%d", ap_info[i].primary);
        ESP_LOGI(TAG, "Authmode \t%s \n", getAuthModeName(ap_info[i].authmode));
    }

    // Ao não utilizar mais o WiFi podemos realizar os seguintes passos
    ESP_ERROR_CHECK(esp_wifi_stop());                                   //Stop WiFi
    ESP_ERROR_CHECK(esp_wifi_deinit());                                 //Deinit WiFi
    ESP_ERROR_CHECK(esp_event_loop_delete_default());                   //Delete default event loop
    esp_netif_destroy(sta_netif);                                       //Destroy default WiFi station netif instance

}

void app_main(void)
{
    //Exemplo de initialização da NVS
    esp_err_t ret = nvs_flash_init();

    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_scan();
}
