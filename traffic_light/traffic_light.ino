#define LED_PIN_RED 11
#define LED_PIN_BLUE 12
#define LED_PIN_GREEN 13
#define SET_COLOR_RED 'r'
#define SET_COLOR_GREEN 'g'
#define SET_MODE_MANUAL 'm'
#define STATE_LED_OFF 1
#define STATE_LED_ON 2
#define STATE_LED_BLINK 3
#define MODE_AUTO 3
#define MODE_MANUAL 4
#define COLOR_RED 11
#define COLOR_BLUE 12
#define COLOR_GREEN 13
#define COLOR_GREEN_PERIOD 10000
#define COLOR_RED_PERIOD 7000
#define COLOR_BLUE_PERIOD 1000
#define BLINK_PERIOD 2000
#define BLINK_INTERVAL 100
#define MODE_MANUAL_PERIOD 60000

int state = 2;
bool led_status = LOW;
int current_pin = 13;
int mode = 3;
int color = COLOR_GREEN;
long int color_start = 0;
long int blink_start_period = 0;
long int blink_start = 0;
int current_period = COLOR_GREEN_PERIOD;
int colors[4] = {COLOR_GREEN, COLOR_BLUE, COLOR_RED, COLOR_BLUE};
int periods[4] = {COLOR_GREEN_PERIOD, COLOR_BLUE_PERIOD, COLOR_RED_PERIOD, COLOR_BLUE_PERIOD};
int index = 0;
int time_multiplier = 1;
long int mode_start = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN_RED, OUTPUT);
  pinMode(LED_PIN_BLUE, OUTPUT);
  pinMode(LED_PIN_GREEN, OUTPUT);
}

void state_update() {
  if (millis() > color_start+current_period*time_multiplier){
    if (color==COLOR_GREEN && state==STATE_LED_ON){
      state=STATE_LED_BLINK;
      blink_start_period=millis();
    }
    if (state==STATE_LED_BLINK && millis()>blink_start_period+BLINK_PERIOD){
      state=STATE_LED_ON;
    }
    if (state==STATE_LED_ON){
      index=(index+1)%4;
    }
  }
  if(Serial.available() > 0){
    char cmd = Serial.read();
    if (cmd==SET_MODE_MANUAL){
      Serial.println("Manual mode is active");
      mode = MODE_MANUAL;
      mode_start=millis();
      return;
    }
    if (mode==MODE_MANUAL && cmd!=SET_MODE_MANUAL){
      if (cmd==SET_COLOR_RED){
        if (color!=COLOR_RED){
          time_multiplier = 3;
          index=1;
        }
      }
      else if (cmd==SET_COLOR_GREEN){
        if (color!=COLOR_GREEN){
          time_multiplier = 3;
          index=3;
        }
      }
      else {
        Serial.println("Unknown command");
      }
    }
    else {
      Serial.println("Access denied. Manual mode is inactive.");
    }
  }
}

void switch_color() {
  if (colors[index] != current_pin){
    state = STATE_LED_OFF;
    state_proccess();
    current_pin = colors[index];
    color=colors[index];
    current_period=periods[index];
    state = STATE_LED_ON;
    state_proccess();
    color_start=millis();
  }
  time_multiplier = 1;
}

void state_proccess() {
  if (state == STATE_LED_ON and led_status==LOW){
    digitalWrite(current_pin, HIGH);
    led_status=HIGH;
  }
  else if (state == STATE_LED_OFF and led_status==HIGH){
    digitalWrite(current_pin, LOW);
    led_status=LOW;
  }
  else if (state == STATE_LED_BLINK){
    if (millis() <= blink_start + BLINK_INTERVAL and led_status==LOW){
      digitalWrite(current_pin, HIGH);
      led_status=HIGH;
    }
    if (millis() >= blink_start + BLINK_INTERVAL and millis() <= blink_start + 2*BLINK_INTERVAL and led_status==HIGH){
      digitalWrite(current_pin, LOW);
      led_status=LOW;
    }
    if (millis() >= blink_start + 2*BLINK_INTERVAL){
      blink_start=millis();
    }
  }
}

void switch_mode(){
  if (mode == MODE_MANUAL && millis()>mode_start+MODE_MANUAL_PERIOD){
    mode=MODE_AUTO;
    Serial.println("Time is out. Returning AUTO_MODE");
  }
}

void loop() {
  state_update();
  switch_color();
  state_proccess();
  switch_mode();
}
