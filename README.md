# Remote WAKE ON LAN SOLUTION

**Author: Emilio Tonix**
**Review Date: 26/08/2021** 
**Open Source**

# Docker instalation
You can skip this step if you already have a docker instalation.


https://gist.github.com/Tonix22/5c046b171b697bac2c0b4f5110a19656

# Main scope and overview

This code assumes that you are familiar with Wake-On-LAN and can perform it on your PC. The goal of this project is to enable Wake-On-LAN outside of your local network, as an alternative to using a virtual private network (VPN) at home or in locations where you do not have access to a router to set one up.

The main idea is to use an ESP8266 to send a WOL packet throught udp with port 9 and lwipsockets.

<img src="/Diagrams/esp8266.jpg" alt="WIFIPLUG" alt="drawing" width="200"/>

For remote wake up there are two ways to this.

### Easier but extra cost

Use the [WOL_only](/WOL_only) project to set up the wifi settings and MAC address in the header file,then just power on the esp8266 device and it will run a sript to excute the WOL. The extra cost is because you need a wifi remote wifi plug to this. So the main idea to power up remotely the esp8266 once is ready it sends magic packet to PC.

<img src="/Diagrams/smatplug.png" alt="WIFIPLUG" alt="drawing" width="200"/>


### Cheaper with extra work

Use the [MQTT_and_WOL](/MQTT_and_WOL) project to set up and MQTT remote server. The main idea is that the device is subscribed to a topic, we could say that this is like to continous listening certain signal,the code wait to recieve "ON" as the trigger of WOL. 

The MQTT parameters are accesible from the code and the most frequently parameters are in the header file with defines. In my case I used [adafruit.io](https://io.adafruit.com) as an MQTT server, but you could use other suppliers as [CloudMqtt](https://www.cloudmqtt.com/plans.html) or [AWS](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt.html). I used adafruit because is free, but don't hope high bandwith and 100% guaranteed responses it fails sometimes when server sleep for some seconds. 

<img src="/Diagrams/General_View.jpg" alt="GeneralOverview" alt="drawing" width="500"/>

# Install enviorment

To do as faster as possible the esp8266 development we used a docker image with esp8266 RTOS SDK, enviromental paths, python tools, compiler configurations. **As requisite install docker in your os.**

For windows my advice is to use wsl, however windows has native docker, but I didn't try it before. 

If you don't have docker follow this procedure till **step 4**

[Install docker if you don't have it](https://github.com/Tonix22/LuxFlux_Esp8266_LighStick/wiki/1.-Docker-Setup)


Pull this docker image tonix22/esp8266-idf

```console
user@user:~$ sudo docker pull tonix22/esp8266-idf
```

Run docker image, open a terminal in the source folder with the device esp8266 pluged. 

```console
user@user:~$ ./mount_docker.sh
root@c6106fe485ce:/home/esp# cd /project
```
## Set up WIFI credentials

Got to your project folder [WOL_only](/WOL_only) or [MQTT_and_WOL](/MQTT_and_WOL) it is the same process for both of them. Then configure the WIFI_SSID and PASSWORD

**Example**

```console
root@c6106fe485ce:/home/esp# cd WOL_only/
root@c6106fe485ce:/home/esp# make menuconfig
```
Choose the option Example Connection Configuration and then
setup your wifi credentials Wifi SSID and Wifi Password

<img src="https://user-images.githubusercontent.com/8690843/131026597-22a9d280-6229-40a9-ba7b-7c445e369fdf.png" alt="GeneralOverview" alt="drawing" width="300"/> <img src="https://user-images.githubusercontent.com/8690843/131026896-c2cd9784-26d3-4b23-97f9-142e10ee070a.png" alt="GeneralOverview" alt="drawing" width="300"/>

**NOTE**
In this same menu you can configure your USB port, for linux users it may be also setup. Check step 4 from this link 
[USB_DEVICE SETTINGS](https://github.com/Tonix22/LuxFlux_Esp8266_LighStick/wiki/1.-Docker-Setup#4run-the-container)


## Configure MAC addres

Go to **MQTT_and_WOL/main/wol.h** or **WOL_only/main/udp_client.c** ,and modify the #define MAC_ADDR with your MAC. 
```C
#define MAC_ADDR 0x24,0x4B,0xFE,0x8d,0x81,0x3D
```
## Configure MQTT

Go to **MQTT_and_WOL/main/mqtt.h** and then check all paramters requerided for your project. If there remains any field just deleted in mqtt_cfg in mqtt.c

# Download project to board
 The last step is to compile and save the code in the device.
 
 **ALL** compile folder source files
 
 **Flash** download code to board
 
 **Monitor** It is optional, it shows device debug info. 
 
 ```console
root@c6106fe485ce:/home/esp/project/WOL_only# make all flash monitor
```


