#include <PubSubClient.h>
#include <cstring>
#include "Arduino.h"
extern String topic_confirm;
extern String pub_id;
extern String topic_photo_min;
extern String topic_photo_max;
extern String topic_photo_instant;
extern String topic_photo_average;
extern String topic_photo_stream;
extern String topic_active_stream;
extern int mi;
extern int ma;
PubSubClient mqtt_cli(wifiClient);

void get_pub_id(char *topic, byte *payload, unsigned int length) {
  String pubb_id=pub_id;
  for(int i=0;i<length;i++){
    pubb_id=pubb_id+(char) payload[i];
  }
  pub_id=pubb_id;
  topic_confirm="lab/" + pub_id + "/confirm";
  mqtt_cli.publish(topic_confirm.c_str(), pub_id.c_str());
}
void callback(char *topic, byte *payload, unsigned int length) {
  String data="";
  for(int i=0;i<length;i++){
    data=data+(char) payload[i];
  }
  if(strcmp(topic, topic_photo_min.c_str()) == 0){
    Serial.println("recived message min- "+data);
    mi =atoi(data.c_str());
  }
  if(strcmp(topic, topic_photo_max.c_str()) == 0){
    Serial.println("recived message max- "+data);
    ma = atoi(data.c_str());
  }
  if(strcmp(topic, topic_photo_instant.c_str()) == 0){
    Serial.println("recived message instant- "+data);
    if (atoi(data.c_str()) >(mi+ma)/2){
      digitalWrite(LED_PIN, LOW);
    }
    else{
      digitalWrite(LED_PIN, HIGH);
    }
  }
  if(strcmp(topic, topic_photo_average.c_str()) == 0){
    Serial.println("recived message average- "+data);
    if (atoi(data.c_str()) >(mi+ma)/2){
      digitalWrite(LED_PIN, LOW);
    }
    else{
      digitalWrite(LED_PIN, HIGH);
    }
  }
  if(strcmp(topic, topic_photo_stream.c_str()) == 0){
    Serial.println("recived message photo_stream- "+data);
   if (atoi(data.c_str()) >(mi+ma)/2){
      digitalWrite(LED_PIN, LOW);
    }
    else{
      digitalWrite(LED_PIN, HIGH);
    } 
  }
  if(strcmp(topic, topic_active_stream.c_str()) == 0){
    Serial.println("recived message active_stream- "+data);
    if (data=="on"){
      mqtt_cli.subscribe(topic_photo_stream.c_str());
    }
    else if(data =="off"){
      mqtt_cli.unsubscribe(topic_photo_stream.c_str());
    }
  }  
}

extern bool pub_id_sended;

PubSubClient mqtt_cli(wifiClient);

void get_confirm(char *topic, byte *payload, unsigned int length) {
  if (strcmp(topic, topic_confirm.c_str()) == 0){
    pub_id_sended=true;
    Serial.println("Geted confirmation from subscriber.");
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
      } else {
          Serial.print("failed with state ");
          Serial.println(mqtt_cli.state());
          delay(2000);
      }
  }  
}