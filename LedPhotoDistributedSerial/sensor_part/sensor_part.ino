#define PHOTO_RESISTOR A3
#define READ_RESISTOR 'p'

unsigned char p_val;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    char cmd = Serial.read();
    if (cmd==READ_RESISTOR){
      p_val = analogRead(PHOTO_RESISTOR)/4;
      Serial.print(p_val);
    }
  }
}
