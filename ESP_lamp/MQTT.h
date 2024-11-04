#include <PubSubClient.h>
#include <cstring>
#include "Arduino.h"
String topic = "lab/demid/esp_lamp";
PubSubClient mqtt_cli(wifiClient);

extern int current_state;

void callback(char *topic, byte *payload, unsigned int length) {
  current_state = STATE_MQTT_BLINK;
  String data="";
  for(int i=0;i<length;i++){
    data=data+(char) payload[i];
  }
  if (data=="turn__on"){
    digitalWrite(LED_PIN, LOW);
  }
  if (data=="turn_off"){
    digitalWrite(LED_PIN, HIGH);
  }
}

void MQTT_init(){
  mqtt_cli.setServer(mqtt_broker, mqtt_port);
  while (!mqtt_cli.connected()) {
      String client_id = "esp8266-" + String(WiFi.macAddress());
      Serial.print("The client " + client_id);
      Serial.println(" connects to the public mqtt broker\n");
      if (mqtt_cli.connect(client_id.c_str())){
          Serial.println("MQTT Connected");
          Serial.print("I am listening ");
          Serial.println(topic);
      } else {
          Serial.print("failed with state ");
          Serial.println(mqtt_cli.state());
          delay(2000);
      }
  }
  mqtt_cli.setCallback(callback);
}