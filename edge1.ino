#include <Adafruit_Sensor.h>
#include "EspMQTTClient.h"

#define LEDC_BASE_FREQ 5000
#define LED_CHANNEL 0
#define LEDC_TIMER_8_BIT 8
#define LED_PIN 17
int btn = 18;

EspMQTTClient client(
  "MULTI_GUEST",    //wifi SSID
  "guest1357",      //wifi password
  "70.12.227.227",    // MQTT Broker server ip
  "Noyhkos",          // MQTT Username
  "",     // MQTT pw
  "room1",     //device name, 장치를 구별하는 용도
  1883             
);
char *pwm_topic = "floor2/room2/pwm";
char *btn_topic = "floor2/room1/btn";

int state = 0;
int btn_val = 1;
int pwm_val = 0;
int clk = 0;

void tx(){
  client.publish(btn_topic, String(state));
}

void rx(){
  client.subscribe(pwm_topic, [](const String & payload) {
    Serial.print("Received: ");
    Serial.println(payload);
    pwm_val = payload.toInt();
  });
}

void setup() {
  Serial.begin(115200);

  client.enableDebuggingMessages(); 
  client.enableHTTPWebUpdater(); 
  client.enableOTA(); 
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");

  pinMode(btn, INPUT);
  ledcAttach(LED_PIN, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
}

void onConnectionEstablished(){
  //client.loop() 에 의해 호출되는 API
}

void loop() {
  Serial.println("=============================");

  int n_btn_val = digitalRead(btn);
  if(n_btn_val == 1 && btn_val == 0){
    if(state==1)state=0;
    else state=1;
  }
  btn_val = n_btn_val;
  Serial.print("Button State: ");
  Serial.println(state);
  if(clk%10==0){
    tx();
    rx();
  }
  client.loop();

  //pwm_val = map(pwm_val, 80, 180, 0, 255);
  int mapped_pwm;
  if (pwm_val <= 120) {
    mapped_pwm = 0;
  } else if (pwm_val >= 160) {
    mapped_pwm = 255;
  } else {
    mapped_pwm = (pwm_val-120)*(255/40);
  }
  Serial.print("pwm: ");
  Serial.println(mapped_pwm);
  ledcWrite(LED_PIN, pwm_val);

  clk++;
  delay(100);
}
