#define LED_PIN1 3
#define LED_PIN2 4
#define LED_PIN3 5
#define LED_PIN4 6
byte OFF = 0b00000000;
byte ON = 0b11000000;
byte ST = 0b10010000;
byte ND = 0b10100000;
byte RD = 0b01010000;
byte TH = 0b01100000;
byte ST_ND = 0b10000000;
byte ND_TH = 0b11100000;
byte RD_TH = 0b01000000;
byte ST_RD = 0b11010000;
byte ST_TH = 0b10010110;
byte ND_RD = 0b10100101;
byte ST_ND_RD = 0b10000101;
byte ST_ND_TH = 0b11101001;
byte ST_RD_TH = 0b01001001;
byte ND_RD_TH = 0b01001010;

byte states[16]={OFF, ON, ST, ND, RD, TH, ST_ND, ND_TH, RD_TH, ST_RD, ST_TH, ND_RD, ST_ND_RD, ST_ND_TH, ST_RD_TH, ND_RD_TH};

int long start_blink=0;
int long start=0;
int index = 0;
int pins[4]={LED_PIN1, LED_PIN2, LED_PIN3, LED_PIN4};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i=0; i<sizeof(pins); i++){
    pinMode(pins[i], OUTPUT);
  }
  // широтно импульсная модуляция (ШИМ - PWM) - изменение заполнения в const периоде T
  // Fast blink - увеличение периода T между свечением и несвечением
}

void loop() {
  if (millis()>start+2000){
    start=millis();
    index=(index+1)%16;
  }
  else {
    cross_blink(pins, states[index], 4);
  }
}

void cross_blink(int* pins, byte state, int period){
  if (millis()<=start_blink+period/2){
    for (int i=0; i<4; i++){
      byte bit=(state>>(7-i))&1;
      digitalWrite(pins[i], bit);
    }
  }
  else if (millis()<start_blink+period){
    for (int i=0; i<4; i++){
      byte bit=(state>>3-i)&1;
      digitalWrite(pins[i], bit);
    }
  }
  else {
    start_blink=millis();
  }
}