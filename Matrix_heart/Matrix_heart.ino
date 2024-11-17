#define LED_PIN0 0
#define LED_PIN1 1
#define LED_PIN2 2
#define LED_PIN3 3
#define LED_PIN4 4
#define LED_PIN5 5
#define LED_PIN6 6
#define LED_PIN7 7
#define LED_PIN8 8
#define LED_PIN9 9
#define LED_PIN10 10
#define LED_PIN11 11
#define LED_PIN12 12
#define LED_PIN13 13
#define LED_PINA0 A0
#define LED_PINA1 A1
byte FIRST = 0b11111111;
byte SECOND = 0b10011001;
byte THIRD = 0b00000000;
byte FOURTH = 0b00000000;
byte FIFTH = 0b10000001;
byte SIXTH = 0b11000011;
byte SEVENTH = 0b11100111;
byte EIGHTH = 0b11111111;

byte rows[8]={FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH, EIGHTH};

int long start_blink=0;
int long start=0;
int index = 0;
int pins[16]={LED_PIN0,LED_PIN1, LED_PIN2, LED_PIN3, LED_PIN4,LED_PIN5, LED_PIN6, LED_PIN7, LED_PIN8,LED_PIN9, LED_PIN10, LED_PIN11, LED_PIN12,LED_PIN13, LED_PINA0, LED_PINA1};

void setup() {
  for (int i=0; i<sizeof(pins); i++){
    pinMode(pins[i], OUTPUT);
  }
  // широтно импульсная модуляция (ШИМ - PWM) - изменение заполнения в const периоде T
  // Fast blink - увеличение периода T между свечением и несвечением
}

void loop() {
  cross_blink(pins, 40);
}

void cross_blink(int* pins, int period){
  if (millis()<=start_blink+period/8){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[0], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[0]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<=start_blink+period/8*2){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[1], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[1]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<=start_blink+period/8*3){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[2], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[2]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<=start_blink+period/8*4){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[3], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[3]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<=start_blink+period/8*5){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[4], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[4]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<=start_blink+period/8*6){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[5], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[5]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<=start_blink+period/8*7){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[6], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[6]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else if (millis()<start_blink+period){
    for (int i=0; i<8; i++){
      digitalWrite(pins[i], LOW);
    }
    digitalWrite(pins[7], HIGH);
    for (int i=0; i<8; i++){
      byte bit=(rows[7]>>(7-i))&1;
      digitalWrite(pins[8+i], bit);
    }
  }
  else {
    start_blink=millis();
  }
}