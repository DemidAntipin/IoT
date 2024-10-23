#define PHOTO_RESISTOR A3
#define READ_RESISTOR 'p'
#define SWITCH_STREAM 's'

char p_val;
bool mode = false;


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
    if (cmd=SWITCH_STREAM){
      if (mode){
        mode=false;
        char answer = 'n';
        Serial.print(answer);
      }
      else{
        mode=true;
        char answer = 'f';
        Serial.print(answer);
      }
    }
  }
}
