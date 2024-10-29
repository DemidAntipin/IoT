#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"

#define PHOTO_RESISTOR A0

int p_val;
bool stream_mode = false;
bool pub_id_sended = false;

long int instant = 0;
long int average = 0;
long int stream = 0;
long int stream_start = 0;
long int min_max = 0;
int mi = 1024;
int ma = 0;

String pub_id;
String topic_get_pub_id;
String topic_confirm;
String topic_photo_min;
String topic_photo_max;
String topic_photo_instant;
String topic_photo_average;
String topic_photo_stream;
String topic_active_stream;

int arr[100] = {NAN};
long arrive_time[100];

void clean_messages() {
    long current = millis();
    int to_delete = 0;
    for (int i = 0; i < 100; i++) {
        if (isnan(arr[i])) {
            continue;
        }
        if (current - arrive_time[i] >= 2000) {  // 2 секунды
            to_delete++;
        } else {
            break;
        }
    }
    for (int i = 0; i < 100 - to_delete; i++) {
        arr[i] = arr[i + to_delete];
        arrive_time[i] = arrive_time[i + to_delete];
    }
    for (int i = 100 - to_delete; i < 100; i++) {
        arr[i] = NAN;
        arrive_time[i] = 0;
    }
}

void setup() {
  Serial.begin(115200);
  WIFI_init(false);
  MQTT_init();
  pub_id = id();
  topic_get_pub_id = "lab/" + pub_id + "/get_pub_id";
  topic_confirm = "lab/" + pub_id + "/confirm";
  topic_photo_min = "lab/" + pub_id + "/photo/min";
  topic_photo_max = "lab/" + pub_id + "/photo/max";
  topic_photo_instant = "lab/" + pub_id + "/photo/instant";
  topic_photo_average = "lab/" + pub_id + "/photo/average";
  topic_photo_stream = "lab/" + pub_id + "/photo/stream";
  topic_active_stream = "lab/" + pub_id + "/photo/active_stream";
  mqtt_cli.setCallback(get_confirm));
  mqtt_cli.subscribe(topic_confirm.c_str());
  while (!pub_id_sended) {
    Serial.println("Trying to send pub_id to subscriber");
    mqtt_cli.publish(topic_get_pub_id.c_str(), pub_id.c_str());
    mqtt_cli.loop();
    delay(500);
  }
}

float calculate_average() {
    float sum = 0;
    int count = 0;
    for (int i = 0; i < 100; i++) {
        if (!isnan(arr[i])) {
            sum += arr[i];
            count++;
        }
    }
    return count > 0 ? sum / count : NAN;
}

void loop() {
    clean_messages();
    p_val = analogRead(PHOTO_RESISTOR);
    memmove(arr, arr + 1, (99) * sizeof(int));
    arr[99] = p_val;
    mi = min(mi, arr[99]);
    ma = max(ma, arr[99]);
    memmove(arrive_time, arrive_time + 1, (99) * sizeof(long));
    arrive_time[99] = millis();
    long current = millis();
    
    if (current - min_max >= 5000) {
        Serial.println("Publish min: " + String(mi));
        mqtt_cli.publish(topic_photo_min.c_str(), String(mi).c_str());
        Serial.println("Publish max: " + String(ma));
        mqtt_cli.publish(topic_photo_max.c_str(), String(ma).c_str());
        min_max = millis();
    }
    
    if (current - instant >= 1000) {
        Serial.println("Publish instant: " + String(arr[99]));
        mqtt_cli.publish(topic_photo_instant.c_str(), String(arr[99]).c_str());
        instant = millis();
    }
    
    if (current - average >= 1000) {
        float avg = calculate_average();
        Serial.println("Publish average: " + String(avg));
        mqtt_cli.publish(topic_photo_average.c_str(), String(avg).c_str());
        average = millis();
    }
    
    if (stream_mode && current - stream >= 100) {
        Serial.println("Publish stream: " + String(arr[99]));
        mqtt_cli.publish(topic_photo_stream.c_str(), String(arr[99]).c_str());
        stream = millis();
    }
    
    if (current - stream_start > 10000) {
        if (stream_mode) {
            Serial.println("Publish active_stream: off\n");
            mqtt_cli.publish(topic_active_stream.c_str(), "off");
            stream_mode = false;
        } else {
            Serial.println("Publish active_stream: on\n");
            mqtt_cli.publish(topic_active_stream.c_str(), "on");
            stream_mode = true;
        }
        stream_start = millis();
    }
    delay(500);
}
