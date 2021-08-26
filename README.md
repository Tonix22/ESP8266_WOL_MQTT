# Remote WAKE ON LAN SOLUTION

**Author: Emilio Tonix**
**Review Date: 26/08/2021** 
**Open Source**


# Main scope and overview

For this code it is assumed that you can do Wake-On-LAN in your PC. The aim of the project is to do Wake-On-LAN outside of your local network and provide an effective way to avoid using VNP in your home or in some place that you may not have access to a router to create one.

The main idea is to use an ESP8266 to send a WOL packet throught udp with port 9 and lwipsockets.

For Remote wake up there are two ways to this.

### Easier but extra cost

Use the [WOL_only](/WOL_only) project to set up the wifi settings and MAC address in the header file,then just power on the esp8266 device and it will run a sript to excute the WOL. The extra cost is because you need a wifi remote wifi plug to this. So the main idea to power up remotely the esp8266 once is ready it sends magic packet to PC.

<img src="/Diagrams/smatplug.png" alt="WIFIPLUG" style="height: 50px; width:50px;"/>


### Cheaper with extra work

Use the [MQTT_and_WOL](/MQTT_and_WOL) project to set up and MQTT remote server. The main idea is that the device is subscribed to a topic, we could say that this is like to continous listening certain signal,the code wait to recieve "ON" as the trigger of WOL. 

The MQTT parameters are accesible from the code and the most frequently parameters are in the header file with defines. In my case I used [adafruit.io](https://io.adafruit.com) as an MQTT server, but you could use other suppliers as [CloudMqtt](https://www.cloudmqtt.com/plans.html) or [AWS](https://docs.aws.amazon.com/iot/latest/developerguide/mqtt.html). I used adafruit because is free, but don't hope high bandwith and 100% guaranteed responses it fails sometimes when server sleep for some seconds. 

<img src="/Diagrams/General_View.jpg" alt="GeneralOverview" style="height: 50px; width:50px;"/>

# Install enviorment

To do as faster as possible the esp8266 development we used a docker image with esp8266 RTOS SDK, enviromental paths, python tools, compiler configurations. **As requisite install docker in your os.**

For windows my advice is to use wsl, however windows has native docker, but I didn't try it before. 

If you don't have docker follow this procedure till **step 4**

[Install docker if you don't have it](https://github.com/Tonix22/LuxFlux_Esp8266_LighStick/wiki/1.-Docker-Setup)


Pull this docker image tonix22/esp8266-idf

```console
user@user:~$ sudo docker pull tonix22/esp8266-idf
```

Run docker image, open a terminal where you has acces to work. 

```console
user@user:~$ ./mount_docker.sh
root@c6106fe485ce:/home/esp# cd /project
```








