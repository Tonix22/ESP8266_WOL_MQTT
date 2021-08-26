/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "protocol_examples_common.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#define PORT 9
#define MAC_ADDR 0x24,0x4B,0xFE,0x8d,0x81,0x3D
#define IP_ADDR "192.168.0.255"

static const char *TAG = "example";


static void udp_client_task(void *pvParameters)
{    
    int i;
    unsigned char toSend[102];
    unsigned char mac[6] = {MAC_ADDR};
    struct sockaddr_in udpClient, udpServer;
    int broadcast = 1 ;


    ESP_LOGI(TAG, "Creating socket...");
    // UDP Socket creation
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // Manipulating the Socket
    if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST,
                &broadcast, sizeof broadcast) == -1)
    {
        printf("error: setsockopt\r\n");
        goto end;
    }
    udpClient.sin_family = AF_INET;
    udpClient.sin_addr.s_addr = INADDR_ANY;
    udpClient.sin_port = 0;

    //Binding the socket
    bind(udpSocket, (struct sockaddr*)&udpClient, sizeof(udpClient));

    for (i=0; i<6; i++)
        toSend[i] = 0xFF;
    
    for (i=1; i<=16; i++)
        memcpy(&toSend[i*6], &mac, 6*sizeof(unsigned char));

    udpServer.sin_family = AF_INET;

    // Braodcast address
    udpServer.sin_addr.s_addr = inet_addr(IP_ADDR);
    udpServer.sin_port = htons(PORT);

    for(int i=0;i<3;i++)
    {
        sendto(udpSocket, &toSend, sizeof(unsigned char) * 102, 0,
            (struct sockaddr*)&udpServer, sizeof(udpServer));
    }

    ESP_LOGI(TAG, "Packet sent sucessfully");
    end:
    vTaskDelete(NULL);
}

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
}
