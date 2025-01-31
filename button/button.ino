#define BUTTON_PIN 2 
#define BOUNCE_TIME 50
#define PRESSED HIGH
volatile long int press_time = 0;
volatile bool pressed_candidate = false;
volatile bool released_candidate = false;
volatile long int hold_time = 0;
volatile int press_count = 0;
volatile bool report_to_user_press = false;
volatile bool report_to_user_release = false;
int calls=0;
bool button_pressed = false;

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, process_button_click, RISING);
  pinMode(BUTTON_PIN, INPUT);
  cli(); 
  TCCR1A=0; 
  TCCR1B=0; 
  TCNT1=0; 
  OCR1A=10000; 
  TCCR1B |= (1 << WGM12); 
  TCCR1B = TCCR1B | (1 << CS11); 
  TIMSK1 |= (1 << OCIE1A); 
  sei();
}

ISR(TIMER1_COMPA_vect) {
  if (button_pressed) {
    if (!(PIND & (1 << BUTTON_PIN))) {
      if (released_candidate) {
        calls++;
        if (calls == 10) {
          button_pressed = false;
          pressed_candidate = false;
          hold_time = millis() - press_time;
          report_to_user_release = true;
          released_candidate = false;
          calls = 0;
        }
      }
      else {
        released_candidate = true;
        calls=0;
      }
    }
  }
  else if (pressed_candidate) {
    calls++;
    if (calls == 10) {
      button_pressed = PIND & (1 << BUTTON_PIN);
      if (button_pressed) {
        press_count++;
        report_to_user_press = true;
      }
      pressed_candidate = false;
      calls = 0;
    }
  }
}

void loop() {
    if (report_to_user_press) {
        Serial.print("Button pressed. It is ");
        Serial.print(press_count);
        Serial.println(" press.");
        report_to_user_press = false;
    }
    if (report_to_user_release) {
        Serial.print("Button released. You held it ");
        Serial.print(hold_time);
        Serial.println(" milliseconds.");
        report_to_user_release = false;
    }
}

void process_button_click() {
  if (pressed_candidate == false) {
    press_time = millis();
    pressed_candidate = true;
    hold_time = 0;
    calls=0;
  }
}
