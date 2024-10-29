#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"


int p_val;
bool stream_mode = false;

int mi = 256;
int ma = 0;

String pub_id = "";
String topic_get_pub_id = "lab/+/get_pub_id";
String topic_confirm;
String topic_photo_min;
String topic_photo_max;
String topic_photo_instant;
String topic_photo_average;
String topic_photo_stream;
String topic_active_stream;

void setup() {
  Serial.begin(115200);
  WIFI_init(false);
  MQTT_init();
  mqtt_cli.subscribe(topic_get_pub_id.c_str());
  mqtt_cli.setCallback(callback2);
  while(pub_id==""){
    mqtt_cli.loop();
  }
  mqtt_cli.unsubscribe(topic_get_pub_id.c_str());
  topic_photo_min = "lab/" + pub_id + "/photo/min";
  topic_photo_max = "lab/" + pub_id + "/photo/max";
  topic_photo_instant = "lab/" + pub_id + "/photo/instant";
  topic_photo_average = "lab/" + pub_id + "/photo/average";
  topic_photo_stream = "lab/" + pub_id + "/photo/stream";
  topic_active_stream = "lab/" + pub_id + "/photo/active_stream";
  mqtt_cli.setCallback(callback);
  mqtt_cli.subscribe(topic_photo_min.c_str());
  mqtt_cli.subscribe(topic_photo_max.c_str());
  mqtt_cli.subscribe(topic_photo_instant.c_str());
  mqtt_cli.subscribe(topic_photo_average.c_str());
  mqtt_cli.subscribe(topic_active_stream.c_str());
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  mqtt_cli.loop();
}