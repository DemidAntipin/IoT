String ssidAP = "ESP8266-DEMID"; // имя контроллера и точки доступа
String passwordAP = "12345678"; // пароль точки доступа

//char* ssidCLI = "realme 9 Pro 5G"; // имя контроллера и точки доступа
//char* passwordCLI = "12345678545"; // пароль точки доступа

const char* mqtt_broker = "broker.emqx.io";

const int httpPort = 80;
const int mqtt_port = 1883;

const int LED_PIN=2;

const int FAST_BLINK = 500;
const int SLOW_BLINK = 3000;
const int BLINK_DELAY = 3000;
const int EEPROM_SIZE = 32;
const int STATE_IDLE = 0;
const int STATE_SLOW_BLINK = 1;
const int STATE_FAST_DELAY = 2;
const int STATE_FAST_BLINK = 3;
const int STATE_MQTT_BLINK = 4;

char ssidCLI[EEPROM_SIZE/2];
char passwordCLI[EEPROM_SIZE/2];

int long blink_start = 0;
int blink_interval = SLOW_BLINK;
bool blink_delay = false;
int blinks=0;
bool led_status = LOW;
int current_state = STATE_IDLE;
bool ap_started = false;


