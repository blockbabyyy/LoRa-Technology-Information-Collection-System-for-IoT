# Описание проекта
Разработка сенсорного узла на базе микроконтроллера STM32, позволяющего собирать и передавать данные в "облачный сервис".
- LoRa_Sensor_Unit.ino - код программы сенсорного узла
- LoRa_Gateway.ino - код программы приемного шлюза

ДЕМОНСТРАЦИЯ ПРОКЕТА: https://youtu.be/xiB7jbwgHNI
ССЫЛКА НА СХОДНИК ВИДЕО: https://disk.yandex.ru/i/myJq9jM-QWkTTA

## Аппаратная часть:
  - Микроконтрллер: STM32 NUCLEO-F446RE
  - Датчик состояния окружающей среды: SHT31
  - Автономное питание: Battery Shield 3200 mAh
  - GPS-модуль: U-blox NEO-6M
  - LoRa-модуль: Ai-Thinker RA-02 433 MHz
## Программная часть:
  ### Библиотеки:
    - Adafruit_SHT31.h
    - SPI.h
    - RH_RF95.h
    - Wire.h
    - stdint.h
    - TinyGPS++
 В качестве среды разработки использовалась ArduinoIDE с добавлением плат STM32 путем, описанным проектом STM32duino. 
 
 ## Приемный шлюз
 В качестве приемного шлюза выступает готовое решение Dragino LG-01. Взаимодействие с сетью Интернет осуществляется либо через 4G, либо через Ethernet кабель. Код программы указан в файле LoRa_Gateway_LG-01.ino.
 ## Облачная платформа
 В качестве облачной платформы используется открытая платформа ThingsBoard. Взаимодействие с облаком осуществляется по протоколу MQTT.
 
 Проект выполнен в качестве ВКР для Национального Исследовательского Университета "МЭИ", а так же в рамках выпускной работы IT-Academy Samsung. Научный руководитель: к.т.н., доцент кафедры Основ радиотехники (ОРТ) ИРЭ им. В.А. Котельникова НИУ "МЭИ", член IEEE, Стрелков Н.О.
 **********
 Разрбаотка осуществлялась с упором на работу "Application of Wi-Fi and LoRa Technologies for Wireless Measurement of Physical Quantities" за авторством Стрелкова Н.О. и Михайлова М.С.
***********************
# About project
Development of sensor unit based on STM32 microcontroller for collection and transmission data to Cloud services.
- LoRa_Sensor_Unit.ino - sensor unit code
- LoRa_Gateway.ino - LoRa/4G/Wi-Fi Gateway code

Demonstation on YouTube: https://youtu.be/xiB7jbwgHNI
Demonstration on Cloud Storage: https://disk.yandex.ru/i/myJq9jM-QWkTTA

## Hardware:
  - MCU board: STM32 NUCLEO-F446RE
  - Ambient sensor: SHT31
  - Standalone power: Battery Shield 3200 mAh
  - GPS: U-blox NEO-6M
  - LoRa : Ai-Thinker RA-02 433 MHz
## Software:
  ### Librаries:
    - Adafruit_SHT31.h
    - SPI.h
    - RH_RF95.h
    - Wire.h
    - stdint.h
    - TinyGPS++
 Develop by ArduinoIDE framework with STM32duino project. 
 
 ## LoRa/4G/Wi-Fi Gateway
Dragino LG-01 acts as a receiving gateway. Interaction with the Internet is carried out either via 4G or Ethernet cable. The code of the program is specified in the file LoRa_Gateway_LG-01.ino.
 ## Cloud Platform
 ThingsBoard.ip acts as a cloud paltform. Connection used MQTT protocol.
 
 The project was performed as a final qualification work for the National Research University "MPEI" within the framework of IT-Academy Samsung project. Supervisor: Candidate of Technical Sciences, Associate Professor of the Department of Basic Radio Engineering (ВRE) of Kotelnikov Institute of Radio Engineering of National Research University "MPEI", IEEE member, N.O. Strelkov.
 **********
The development was inspired by the work "Application of Wi-Fi and LoRa Technologies for Wireless Measurement of Physical Quantities" by N.O. Strelkov and M.S. Mikhailov.

