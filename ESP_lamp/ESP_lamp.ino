#include "Config.h"
#include "WIFI.h"
#include "MQTT.h"
#include "Server.h"

int long last_check_connection=0;
bool connected=false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  EEPROM.begin(EEPROM_SIZE);

  EEPROM.get(0, ssidCLI);
  EEPROM.get(EEPROM_SIZE/2, passwordCLI);
  if (ssidCLI[0] == '\0' || passwordCLI[0] == '\0') {
    WIFI_init(true);
    Serial.println("WiFi initialized in AP mode.");
    ap_started=true;
    server_init();
    blink_interval=SLOW_BLINK;
    current_state=STATE_SLOW_BLINK;
    blink_delay = false;
  } 
  else {
    WIFI_init(false);
    ap_started=false;
    current_state=STATE_FAST_DELAY;
    blink_interval=FAST_BLINK;
    blink_delay = true;
    checkInternet();
    last_check_connection=millis();
    if (connected){
      last_check_connection=millis();
      connected=true;
      MQTT_init();
      current_state=STATE_FAST_BLINK;
      blink_delay = false;
      mqtt_cli.subscribe(topic.c_str());
    }
  }
}

void loop() {
  if (current_state==STATE_SLOW_BLINK || current_state==STATE_FAST_DELAY){
    server.handleClient();
    blink();
  }
  else if (current_state==STATE_FAST_BLINK || current_state==STATE_MQTT_BLINK){
    mqtt_cli.loop();
    if (current_state==STATE_FAST_BLINK){
      blink();
    }
  }
  
  if (!connected){
    if (current_state==1){
      return;
    }
    if (WiFi.status() != WL_CONNECTED){
      if (!ap_started){
        WIFI_init(true);
        ap_started=true;
        server_init();
        blink_interval=SLOW_BLINK;
        current_state=STATE_SLOW_BLINK;
        blink_delay = false;
      }
    }
    else{
      blink_interval=FAST_BLINK;
      current_state=STATE_FAST_DELAY;
      blink_delay = true;
    }
  }
  else if (connected){
    if (current_state==STATE_FAST_DELAY){
      MQTT_init();
      blink_interval=FAST_BLINK;
      current_state=STATE_FAST_BLINK;
      blink_delay = false;
      mqtt_cli.subscribe(topic.c_str());
    }
  }
  if (current_state!=1 && millis()>last_check_connection+30000){
    Serial.println("Checking Internet connection");
    checkInternet();
    last_check_connection=millis();
  }
}

void blink(){
  if (millis()>=blink_start+blink_interval && millis()<blink_start+2*blink_interval){
    if (led_status==LOW){
      digitalWrite(LED_PIN, LOW);
      led_status=HIGH;
    }
  }
  if (millis()>blink_start+2*blink_interval){
    if (led_status==HIGH){
      digitalWrite(LED_PIN, HIGH);
      led_status=LOW;
    }
    if (blink_delay){
      blinks++;
      blink_start=millis();
      if (blinks==2){
        blinks=0;
        blink_start=millis()+BLINK_DELAY;
      }
    }
    else{
      blink_start=millis();
    }
  }
}

void checkInternet() {
    // Попробуйте подключиться к Google
    if (!httpClient.connect("www.google.com", 80)) {
        Serial.println("Failed to connect to Wi-Fi.");
        connected = false; // Нет доступа к интернету
        return; // Не удалось подключиться
    }

    // Отправка HTTP-запроса
    httpClient.print(String("GET ") + "/ HTTP/1.1\r\n" +
                     "Host: www.google.com\r\n" +
                     "Connection: close\r\n\r\n");

    // Таймер для проверки ответа
    unsigned long startTime = millis();
    while (httpClient.connected() && millis() - startTime < 5000) { // Таймаут 5 секунд
        if (httpClient.available()) {
            String line = httpClient.readStringUntil('\n');
            if (line.startsWith("HTTP/1.1 200")) {
                Serial.println("Connected to the internet!");
                connected = true; // Успешный ответ
                return;
            }
        }
    }

    Serial.println("No internet access.");
    connected = false; // Нет доступа к интернету
}
