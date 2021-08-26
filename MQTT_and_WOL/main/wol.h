#ifndef WOL_H
#define WOL_H

#define WOL_PORT 9
#define MAC_ADDR 0x24,0x4B,0xFE,0x8d,0x81,0x3D
#define WOL_IP_ADDR "192.168.0.255"

void udp_wol_client_task(void *pvParameters);

#endif