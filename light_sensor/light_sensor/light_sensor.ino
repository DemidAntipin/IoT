#define PHOTO_RESISTOR_LEFT A4
#define PHOTO_RESISTOR_RIGHT A3
#define PHOTO_RESISTOR_CALIBRATION 0.724;
#define LED_PIN_LEFT 12
#define LED_PIN_RIGHT 11
#define STATE_IDLE 0
#define STATE_LED_OFF 1
#define STATE_LED_ON 2
#define STATE_LED_BLINK 3
#define BLINK_INTERVAL 100
#define PHOTO_RESISTOR_ERROR 12

int p_val_left = 0;
int p_val_right = 0;
int state_left = 0;
int state_right = 0;
bool led_status_left = LOW;
bool led_status_right = LOW;
long int blink_start = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN_LEFT, OUTPUT);
  pinMode(LED_PIN_RIGHT, OUTPUT);
}

void read_resistors(){
  p_val_left = analogRead(PHOTO_RESISTOR_LEFT);
  p_val_right = analogRead(PHOTO_RESISTOR_RIGHT) * PHOTO_RESISTOR_CALIBRATION;
}

void update_status(){
  if (abs(p_val_left - p_val_right)<PHOTO_RESISTOR_ERROR && p_val_left>50 && p_val_right>50){
    state_left = STATE_LED_ON;
    state_right = STATE_LED_ON;
  }
  else if (p_val_left>p_val_right && p_val_left>50){
    state_right = STATE_LED_BLINK;
    state_left = STATE_LED_OFF;
  }
  else if (p_val_left < p_val_right && p_val_right>50){
    state_right = STATE_LED_OFF;
    state_left = STATE_LED_BLINK;
  }
  else if (p_val_left<50 && p_val_right<50){
    state_right = STATE_LED_OFF;
    state_left = STATE_LED_OFF;
  }
}

void state_process(){
  if (state_left == STATE_LED_ON and led_status_left==LOW){
    digitalWrite(LED_PIN_LEFT, HIGH);
    led_status_left=HIGH;
  }
  else if (state_left == STATE_LED_OFF and led_status_left==HIGH){
    digitalWrite(LED_PIN_LEFT, LOW);
    led_status_left=LOW;
  }
  else if (state_left == STATE_LED_BLINK){
    if (millis() <= blink_start + BLINK_INTERVAL and led_status_left==LOW){
      digitalWrite(LED_PIN_LEFT, HIGH);
      led_status_left=HIGH;
    }
    if (millis() >= blink_start + BLINK_INTERVAL and millis() <= blink_start + 2*BLINK_INTERVAL and led_status_left==HIGH){
      digitalWrite(LED_PIN_LEFT, LOW);
      led_status_left=LOW;
    }
    if (millis() >= blink_start + 2*BLINK_INTERVAL){
      blink_start=millis();
    }
  }
  if (state_right == STATE_LED_ON and led_status_right==LOW){
    digitalWrite(LED_PIN_RIGHT, HIGH);
    led_status_right=HIGH;
  }
  else if (state_right == STATE_LED_OFF and led_status_right==HIGH){
    digitalWrite(LED_PIN_RIGHT, LOW);
    led_status_right=LOW;
  }
  else if (state_right == STATE_LED_BLINK){
    if (millis() <= blink_start + BLINK_INTERVAL and led_status_right==LOW){
      digitalWrite(LED_PIN_RIGHT, HIGH);
      led_status_right=HIGH;
    }
    if (millis() >= blink_start + BLINK_INTERVAL and millis() <= blink_start + 2*BLINK_INTERVAL and led_status_right==HIGH){
      digitalWrite(LED_PIN_RIGHT, LOW);
      led_status_right=LOW;
    }
    if (millis() >= blink_start + 2*BLINK_INTERVAL){
      blink_start=millis();
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  read_resistors();
  update_status();
  state_process();
}
