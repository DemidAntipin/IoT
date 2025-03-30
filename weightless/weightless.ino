#include <Wire.h>

#define LIGHT_PIN 2
#define WEIGHTLESS_COEF 0.2 // коэффициент невесомости, если суммарное ускорение меньше коэффициента - устройство находится в невесомости

int ADXL345=0x53;
float X_out,Y_out,Z_out;
float X1,Y1,Z1;
bool light_on = false;
void setup(){
  Serial.begin(9600);
  pinMode(LIGHT_PIN, OUTPUT);
  PORTD&= ~(1<<LIGHT_PIN);
  Wire.begin();
  Wire.beginTransmission(ADXL345);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
}
void loop(){
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32);
  Wire.endTransmission();
  Wire.requestFrom(ADXL345,6,true);
  X_out=(Wire.read()|Wire.read()<<8);
  X1=X_out/256;
  Y_out=(Wire.read()|Wire.read()<<8);
  Y1=Y_out/256;
  Z_out=(Wire.read()|Wire.read()<<8);
  Z1=Z_out/256;
  float acceleration = abs(X1)+abs(Y1)+abs(Z1);
  if (acceleration <= WEIGHLESS_COEF) {
    if (!light_on){
      PORTD|=(1<<LIGHT_PIN);
      light_on=true;
    }
  }
  else{
    if (light_on){
      PORTD&= ~(1<<LIGHT_PIN);
      light_on=false;
    }
  }
  delay(100);
}