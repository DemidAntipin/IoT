#define pin_2 2 
#define pin_3 3 
#define pin_4 4 
#define IMAGE_DURATION 1000 
 
long int start =0; 
int message_len=16; 
int curr_len=0; 
int index=0; 
int calls=0;
int row=0;
uint16_t img[8] = {
    0b1000000011111111,
    0b0100000010011001,
    0b0010000000000000,
    0b0001000000000000,
    0b0000100010000001,
    0b0000010011000011,
    0b0000001011100111,
    0b0000000111111111
  };

uint16_t img2[8] = {
    0b1000000010111101,
    0b0100000011000011,
    0b0010000010100101,
    0b0001000000000000,
    0b0000100000000000,
    0b0000010010011001,
    0b0000001011011011,
    0b0000000111100111
};

uint16_t* data[2] = {img, img2}; 
 
void setup() {
  Serial.begin(9600);
  pinMode(pin_2,OUTPUT); 
  pinMode(pin_3,OUTPUT); 
  pinMode(pin_4,OUTPUT); 
  cli(); 
  TCCR1A=0; 
  TCCR1B=0; 
  TCNT1=0; 
  OCR1A=512; 
  TCCR1B |= (1 << WGM12); 
  TCCR1B = TCCR1B | (1 << CS10); 
  TIMSK1 |= (1 << OCIE1A); 
  sei(); 
  DDRD |= (1 << 2) | (1 << 3) | (1 << 4);
  PORTD = 0; 
  start=millis(); 
} 
 
ISR(TIMER1_COMPA_vect){ 
  calls++; 
  if (calls==1){ 
    PORTD= ((PORTD & ~(1<<4)) | (((data[index][row] >> (curr_len)) & 1) << 4)); 
    curr_len=curr_len+1; 
  } 
  else if (calls != 3){ 
    PORTD ^= (1 << 2); 
  }; 
  if (curr_len==message_len && calls==4){ 
    PORTD|=(1<<3); 
    curr_len=0;
    row=(row+1)%8;
  } 
  else{ 
    PORTD &= ~(1 << 3);
  } 
  calls=calls%4; 
} 
 
void loop(){
  if (millis()>start+IMAGE_DURATION){ 
    index=(index+1)%2; 
    start=millis(); 
  } 
}