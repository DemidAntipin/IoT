#define IMAGE_DURATION 3000

byte img1[8] = {
    0b11111111,
    0b10011001,
    0b00000000,
    0b00000000,
    0b10000001,
    0b11000011,
    0b11100111,
    0b11111111
};

byte img2[8] = {
    0b10111101,
    0b11000011,
    0b10100101,
    0b00000000,
    0b00000000,
    0b10011001,
    0b11011011,
    0b11100111
};

int index = 0;
long int start = 0;
byte* images[2] = {img1, img2};
int img_index=0;

byte* rows;

void setup() {
  byte* rows = images[img_index];
  cli();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  OCR1A=16384;
  TCCR1B |= (1 << WGM12);
  TCCR1B = TCCR1B | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
  DDRB |= 0b00111111; // pinMode
  DDRD |= 255;
  DDRC |= 3;
  start=millis();
}

ISR(TIMER1_COMPA_vect){
  PORTD = (1 << index);
  PORTB = rows[index];
  PORTC = (rows[index]>>6);
  index = (index+1)%8;
}

void loop() {
  if (millis()>start+IMAGE_DURATION){
    img_index=(img_index+1)%2;
    rows=images[img_index];
    start=millis();
  }
}