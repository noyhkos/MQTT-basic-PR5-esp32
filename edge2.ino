#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "EspMQTTClient.h"

#define DHTPIN 13
#define DHTTYPE DHT11
#define LED1 17

EspMQTTClient client(
  "MULTI_GUEST",    //SSID
  "guest1357",      //password
  "70.12.227.227",    // MQTT Broker server ip
  "Minjae",   
  "",   
  "room2",       //device name, 장치를 구별하는 용도
  1883             
);

DHT_Unified dht(DHTPIN, DHTTYPE);
char *topic1 = "floor2/room2/pwm";
char *topic2 = "floor2/room1/btn";
uint32_t delayMS;


void rx(){  
  client.subscribe(topic2, [](const String & payload) {
    Serial.println(payload);
    if( payload=="1" ){
      digitalWrite(LED1, HIGH);
    }
    else if ( payload=="0" ){
      digitalWrite(LED1, LOW);
    }
  });
}

void tx(){
  sensors_event_t event;

  double humid;
  double temper;

  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    temper = event.temperature;
    Serial.print(F("Temperature: "));
    Serial.print(temper);
    Serial.println(F("°C"));
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    humid = event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(humid);
    Serial.println(F("%"));
  }

  double val = ((9/5) * temper - 0.55 * (1 - humid/100) * ((9/5) * temper - 26) + 32) / 100 * 255;
  
  client.publish(topic1, val);
}

void setup()
{
  Serial.begin(115200);

  client.enableDebuggingMessages(); 
  client.enableHTTPWebUpdater(); 
  client.enableOTA(); 
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");

  dht.begin();
  pinMode(LED1, OUTPUT);

  delayMS = sensor.min_delay / 1000;
}

void onConnectionEstablished(){
  // client.loop() 가 호출하는 API
}

void loop()
{
  rx();
  tx();
  client.loop();
  delay(500);
}