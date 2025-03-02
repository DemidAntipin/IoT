#define speed_pin 5
#define trans_1_4 7
#define trans_2_3 6
#define clockwise LOW
#define not_clockwise HIGH

int speed = 0;

void setup()
{
  Serial.begin(9600);
  DDRD=0b11100000;
  PORTD=~(1<<speed_pin)|(1<<trans_1_4)|(1<<trans_2_3);
}

void configure(bool direction, int speed) {
  if (direction) {
    PORTD = (PORTD & ~(1 << trans_1_4)) | (1 << trans_2_3);
  } else {
    PORTD = (PORTD & ~(1 << trans_2_3)) | (1 << trans_1_4);
  }
  analogWrite(speed_pin, speed);
}

void loop()
{
  Serial.println("Clockwise rotation, accelerating.");
  for (int i=0; i<256; i++){
  	configure(clockwise, i);
    delay(100);
  }
  Serial.println("Max speed.");
  delay(5000);
  Serial.println("Clockwise rotation, slowing.");
  for (int i=255; i>=0; i--){
  	configure(clockwise, i);
    delay(100);
  }
  Serial.println("Speed = 0 rpm.");
  delay(5000);
  Serial.println("Not_clockwise rotation, accelerating.");
  for (int i=0; i<256; i++){
  	configure(not_clockwise, i);
    delay(100);
  }
  Serial.println("Max speed.");
  delay(5000);
  Serial.println("Not_clockwise rotation, slowing.");
  for (int i=255; i>=0; i--){
  	configure(not_clockwise, i);
    delay(100);
  }
  Serial.println("Speed = 0 rpm.");
  delay(5000);
}