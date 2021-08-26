#include <string.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "wol.h"
#include "mqtt.h"

static const char *TAG = "WOL";
extern SemaphoreHandle_t WOL_In_process;

void udp_wol_client_task(void *pvParameters)
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
    udpServer.sin_addr.s_addr = inet_addr(WOL_IP_ADDR);
    udpServer.sin_port = htons(WOL_PORT);

    for(int i=0;i<3;i++)
    {
        sendto(udpSocket, &toSend, sizeof(unsigned char) * 102, 0,
            (struct sockaddr*)&udpServer, sizeof(udpServer));
    }

    ESP_LOGI(TAG, "Packet sent sucessfully");
    end:

    xSemaphoreGive(WOL_In_process);
    vTaskDelete(NULL);
}