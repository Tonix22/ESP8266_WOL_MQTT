#ifndef MQTT_H
#define MQTT_H

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"


#define BROKER         "mqtt://io.adafruit.com"
#define IO_USERNAME    "EmilioTonix"
#define IO_KEY         "8281440a417b4e16be3b67ba126247d0"
#define BOARD          "MyFirstMQTT"
#define TOPIC_HUMIDITY "EmilioTonix/feeds/humidity"
#define TOPIC_SPRINKLER "EmilioTonix/feeds/sprinkler"
#define PORT 1883

void mqtt_app_start(void);

#endif