# Dedicated Operating Systems Project (DOS)

## GOALS AND SPECIFICATIONS
The project aims to develop a system with the following goals:

• Data acquisition from the brightness sensor and motion sensor by ESP32\
• LED matrix management by ESP32\
• Coordination of tasks managed by FreeRTOS\
• Communication between ESP32 and Raspberry Pi via MQTT protocol

## DESIGN AND DEVELOPMENT
### Development Environment

The project was developed using the [Visual Studio Code](https://code.visualstudio.com/) editor and the [PlatformIO](https://platformio.org/) extension.<br>
The extension provides an IDE for managing libraries and development tests.

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/platform.png" width="550" > 
</p>

### Raspberry Pi Configuration
Communication between RPi and ESP32 is achieved through the MQTT protocol. <br>
MQTT is a standards-based messaging protocol, or a set of rules, used for machine-to-machine communication.
It operates on the principles of the [publish/subscribe](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) model. In traditional network communication, clients and servers communicate directly with each other. Clients request resources or data from the server, which processes them and sends a response. However, MQTT uses a publish/subscribe scheme to decouple the message sender (publisher) from the message receiver (subscriber). Instead, a third component, called a message broker, handles communication between publishers and subscribers. The broker's task is to filter all incoming messages from publishers and distribute them correctly to subscribers.

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mqtt-schema.png" width="550" > 
</p>

To use RPi as a broker, the open-source software [Mosquitto](https://mosquitto.org/) was utilized. <br>
The RPi configuration was performed through the following steps: <br>

• Installation of the Mosquitto broker
 ```
 sudo apt install -y mosquitto mosquitto-clients
 ```
• Enable automatic startup of the broker after RPi boot
 ```
 sudo systemctl enable mosquitto.service
 ```
• Verify the correct installation
 ```
 mosquitto -v
 ```

Additionally, to enable remote access (without authentication), the following procedure was used: <br>
 • Open the mosquitto.conf file
  ```
 sudo nano /etc/mosquitto/mosquitto.conf
 ```
• At the end of the file, add the following two instructions
  ```
 listener 1883
 ```
```
allow_anonymous true
 ```
<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mosquitto-conf.png" width="550" > 
</p>

• Restart Mosquitto
  ```
sudo systemctl restart mosquitto
 ```

#### RPi Usage

For the development of RPi functionalities, the [paho-mqtt](https://pypi.org/project/paho-mqtt/) library was used. <br>
In the **RPi** folder, two main files are present: <br>

• *pub.py*: script with which RPi publishes the colors of the image to be displayed on the LED matrix on the **data/sendImage** topic.<br>
• *sub.py*: script with which RPi acquires the data published by ESP32 on the **data/reqImage** topic.

### Data Acquisition by ESP32
In the implemented system, ESP32 acquires data from the brightness sensor, motion sensor (PIR), and RPi. <br>
The data acquired from the BH1750, which indicates the detected illumination, defines the LED matrix's light intensity. The value obtained from PIR (0 or 1) is used to turn the matrix on or off. The data obtained from RPi represents the colors used to represent the image (pixel art) on the LED matrix. <br>

#### Brightness Sensor

The BH1750 sensor detects illumination and returns a *float* value.<br>
Based on this data, the code will send a request via the **data/reqImage** topic, which means the number identifying the desired image (1, 2, or 3), and RPi, which is subscribed to the topic, will acquire the data and publish it on the **data/sendImage** topic in a string containing the image's colors in the *CRGB* format (the format of the *FastLed* library).

#### Motion Sensor

The motion sensor, directly connected to ESP32, detects movements and reports a value of 1 if movement is detected, or 0 otherwise. <br>
The acquired value indicates whether the matrix is on or off, in addition to allowing the reading of the BH1750 sensor value.

### LED Matrix Management

The matrix composed of 64 LEDs allows displaying an image with LED brightness determined by the BH1750 sensor.<br>
Below is an image representing the operation of the system composed of ESP32 and RPi.

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/mqtt.png" width="550" > 
 </p>
 
### Task Coordination with FreeRTOS

The coordination of various program actions is achieved through the [FreeRTOS](https://www.freertos.org/index.html) library.<br>
Specifically, the following functions are used:<br>

• *xTaskCreate* for task creation, including specifying any parameters and priorities to assign.<br>
• *vTaskDelete* to terminate and delete a task.<br>
• *vTaskDelay* to delay task execution.<br>

Tasks within the code are executed in the following order: <br>

• Detection of the PIR value.<br>
• Detection of the brightness sensor value.<br>
• Activation of the matrix with the correct LED intensity.<br>

<br>

|Task | Components | Priority | 
|---|---|---|
| PIR_STATUS_TASK | ESP32, PIR |1|
| LIGHT_SENSOR_TASK | BH1750, ESP32 |2|
| LEDMATRIX_TASK | ESP32, WS2812B |3|

<br>

<p align="center" style="margin-top: 10px;margin-bottom: 10px">
<img src="https://github.com/alexxdediu/SOD-2023/blob/main/freertos.png" width="550" > 
 </p>

### Hardware Components
• BH1750 brightness sensor\
• HC-SR501 motion sensor\
• WS2812B LED Matrix\
• ESP32\
• Raspberry Pi 3B+
