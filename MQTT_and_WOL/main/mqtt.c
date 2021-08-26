#include "mqtt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "wol.h"
#include <string.h>

const char *TAG = "MQTT_EXAMPLE";

SemaphoreHandle_t WOL_In_process;
char msg_str[20];

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:

            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_publish(client, TOPIC_HUMIDITY, "50", 4, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            msg_id = esp_mqtt_client_subscribe(client, TOPIC_SPRINKLER, 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            //clear string
            memset(msg_str,0,sizeof(msg_str));
            //copy area of interest
            memcpy(msg_str,event->data,event->data_len);

            if(strcmp(msg_str,"ON")==0)
            {
                if(xSemaphoreTake( WOL_In_process, ( TickType_t ) 10 ) == pdTRUE)
                {
                    printf("Lets Do WOL\r\n");
                    xTaskCreate(udp_wol_client_task, "udp_client", 4096, NULL, 5, NULL);
                }   
            }
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
    }
    return ESP_OK;
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = BROKER,
        .port = PORT,
        .client_id = BOARD,
        .event_handle = mqtt_event_handler,
        .username = IO_USERNAME,
        .password = IO_KEY,
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);

    WOL_In_process = xSemaphoreCreateBinary();
    if(WOL_In_process != NULL ){xSemaphoreGive(WOL_In_process);}
    else                   {printf("semaphore not created\r\n");}

    esp_mqtt_client_start(client);
}