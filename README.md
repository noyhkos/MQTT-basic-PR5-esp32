# MQTT 통신

## 하드웨어
MQTT Broker: RaspberryPi 5   
MQTT Publisher/Subscriber : ESP32

## 개요
각각의 EXP32 모듈은 floor2의 room1(edge1)과 room2(edge2)를 상징한다.   
room1는 room2에서 보내는 불쾌지수 값을 받아서 LED(PWM)으로 표시하고,   
room2의 a/c(LED)의 ON/OFF여부를 결정한다.

