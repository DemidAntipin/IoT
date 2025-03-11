#include <SoftwareSerial.h>

#define MODE_IDLE 0
#define MODE_WHEELS 1
#define MODE_DIRECTION 2
#define MODE_SPEED 3
#define MODE_ROTATION 4

#define TRIANGLE 'T'
#define CROSS 'X'
#define CIRCLE 'C'
#define SQUARE 'S'
#define START 'A'
#define PAUSE 'P'
#define RELEASE '0'
#define UP 'F'
#define DOWN 'B'
#define RIGHT 'R'
#define LEFT 'L'

#define LEFT_SPEED_PIN 6
#define RIGHT_SPEED_PIN 5

int LEFT_DIR_PIN = 7;
int RIGHT_DIR_PIN = 4;
bool LEFT_SIDE_FORWARD = HIGH;
bool LEFT_SIDE_BACKWARD = LOW;
bool RIGHT_SIDE_FORWARD = HIGH;
bool RIGHT_SIDE_BACKWARD = LOW;

double speed_coef = 1.0;

int speed = 150;
double time_millis = 1000;
long int start = 0;

int wheel_mode_speed = speed;

int mode = MODE_IDLE;

SoftwareSerial BTSerial(10, 11);
void setup() {
  Serial.begin(9600);
  while(!Serial){}
  BTSerial.begin(9600);
  for (int i=4;i<=7;i++){
    pinMode(i,OUTPUT);
    digitalWrite(i,LOW);
  }
}

void move(bool dir_left,int speed_left,bool dir_right,int speed_right){
  if (speed_left>255){
    speed_left=255;
  }
  if (speed_right>255){
    speed_right=255;
  }
  digitalWrite(LEFT_DIR_PIN,dir_left);
  analogWrite(LEFT_SPEED_PIN,speed_left);
  digitalWrite(RIGHT_DIR_PIN,dir_right);
  analogWrite(RIGHT_SPEED_PIN,speed_right);
}

void stop(){
  move(0,0,0,0);
}
void turn_left_onspot(int speed){
  move(LEFT_SIDE_BACKWARD,speed,RIGHT_SIDE_FORWARD,int(speed*speed_coef));
}
void turn_right_onspot(int speed){
  move(LEFT_SIDE_FORWARD,speed,RIGHT_SIDE_BACKWARD,int(speed*speed_coef));
}
void go_forward(int speed){
  move(LEFT_SIDE_FORWARD,speed,RIGHT_SIDE_FORWARD,int(speed*speed_coef));
}
void go_backward(int speed){
  move(LEFT_SIDE_BACKWARD,speed,RIGHT_SIDE_BACKWARD,int(speed*speed_coef));
}

void turn_left(int speed, int time){
  start=millis();
  while (true) {
    if (millis()>start+time) {
      stop();
      return;
    }
    turn_left_onspot(speed);
  }
}

void turn_right(int speed, int time){
  start=millis();
  while (true) {
    if (millis()>start+time) {
      stop();
      return;
    }
    turn_right_onspot(speed);
  }
}

void loop() {
  if (mode == MODE_ROTATION){
    if (millis()>start+time_millis) stop();
  }
  if (BTSerial.available()){
    byte cmd=BTSerial.read();
    switch (cmd) {
      case TRIANGLE:
        mode = MODE_WHEELS;
        break;
      case CIRCLE:
        mode = MODE_SPEED;
        break;
      case CROSS:
        mode = MODE_ROTATION;
        break;
      case SQUARE:
        mode = MODE_DIRECTION;
        break;
      case START:
        mode = MODE_IDLE;
        break;
      default:
        break;
    }
    if (mode == MODE_WHEELS){
      // wheel_mode_speed может принимать только 2 значения: 0 или совпадать с значением переменной speed
      move(LEFT_SIDE_FORWARD, wheel_mode_speed, RIGHT_SIDE_FORWARD, speed-wheel_mode_speed);
      if (cmd == RIGHT || cmd == LEFT){
        int save = LEFT_DIR_PIN;
        LEFT_DIR_PIN=RIGHT_DIR_PIN;
        RIGHT_DIR_PIN=save;
	      wheel_mode_speed=speed-wheel_mode_speed;
      }
    }
    else if (mode == MODE_SPEED){
      switch (cmd) {
        case UP:
          go_forward(speed);
          break;
        case DOWN:
          go_backward(speed);
          break;
        case RELEASE:
          stop();
          break;
        case RIGHT:
          speed_coef += 0.05;
          break;
        case LEFT:
          speed_coef -= 0.05;
          if (speed_coef < 0) speed_coef = 0;
          break;
        default:
          break;
      }
    }
    else if (mode == MODE_DIRECTION){
      go_forward(speed);
      switch (cmd) {
        case UP:
        case DOWN:
          LEFT_SIDE_FORWARD = !LEFT_SIDE_FORWARD;
          LEFT_SIDE_BACKWARD = !LEFT_SIDE_BACKWARD;
          break;
        case LEFT:
        case RIGHT:
          RIGHT_SIDE_FORWARD = !RIGHT_SIDE_FORWARD;
          RIGHT_SIDE_BACKWARD = !RIGHT_SIDE_BACKWARD;
          break;
        default:
          break;
      }
    }
    else if (mode == MODE_ROTATION){
      switch (cmd) {
        case UP:
          time_millis += 100;
          break;
        case DOWN:
          time_millis -= 100;
          if (time_millis < 0) time_millis = 0;
          break;
        case LEFT:
          turn_left_onspot(speed);
          start = millis();
          break;
        case RIGHT:
          turn_right_onspot(speed);
          start = millis();
          break;
        default:
          break;
      }
    }
    else if (mode == MODE_IDLE) {
      switch (cmd) {
        case UP:
          go_forward(speed);
          break;
        case DOWN:
          go_backward(speed);
          break;
        case LEFT:
          turn_left(speed, time_millis);
          break;
        case RIGHT:
          turn_right(speed, time_millis);
          break;
        case RELEASE:
          stop();
        default:
          break;
      }
    }
  }
  if (Serial.available()){
    BTSerial.write(Serial.read());
  }
}