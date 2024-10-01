#define LED_PIN 12
#define SET_LED_ON 'u'
#define SET_LED_OFF 'd'
#define STATE_IDLE 0
#define STATE_LED_OFF 1
#define STATE_LED_ON 2

int state = 0;
bool led_status = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);

}

void state_update() {
  if(Serial.available() > 0){
    char cmd = Serial.read();
    if (cmd==SET_LED_ON){
      state = STATE_LED_ON;
      Serial.print("LED_ON");
    }
    else if (cmd==SET_LED_OFF){
      state = STATE_LED_OFF;
      Serial.print("LED_OFF");
    }
    else {
      Serial.print("Unknown command");
    }
  }
}

void state_proccess() {
  if (state == STATE_LED_ON and led_status==LOW){
    digitalWrite(LED_PIN, HIGH);
    led_status=HIGH;
  }
  else if (state == STATE_LED_OFF and led_status==HIGH){
    digitalWrite(LED_PIN, LOW);
    led_status=LOW;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  state_update();
  state_proccess();
}
