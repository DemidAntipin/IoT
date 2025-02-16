#define ROW1 2
#define ROW2 3
#define ROW3 4

#define COL1 5
#define COL2 6
#define COL3 7

#define NROWS 3
#define NCOLS 3

const int row_pins[NROWS] = {2, 3, 4};
const int col_pins[NCOLS] = {5, 6, 7};
bool btn_pressed[NCOLS] = {false, false, false};
long int start_time[3][3] = {{0, 0, 0},
                          	 {0, 0, 0},
                          	 {0, 0, 0}};
bool released_buttons[3][3] = {{false, false, false},
							                 {false, false, false},
                               {false, false, false}};
bool report_to_user = false;
int calls = 0;
void setup()
{
  Serial.begin(9600);
  DDRD |= (1<<ROW1) | (1<<ROW2) | (1<<ROW3);
  for(int i = 0;i < NCOLS; i++) {
    pinMode(col_pins[i], INPUT_PULLUP);
  }
  cli();
  TCCR1A=0;
  TCCR1B=0;
  TCNT1=0;
  OCR1A=40000;
  TCCR1B |= (1 << WGM12);
  TCCR1B = TCCR1B | (1 << CS11) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
  PORTD |= 0b00011100;
}

ISR(TIMER1_COMPA_vect) {
  PORTD = (PORTD & ~((1 << 2) | (1 << 3) | (1 << 4))) | ((1 << 2) | (1 << 3) | (1 << 4)) & ~(1 << (calls + 2));
  // В tinkercad INPUT_PULLUP почему-то при чтении через регистры всегда HIGH, даже если подключен к земле.
  btn_pressed[0] = !digitalRead(COL1); // ~(PORTD & (1 << COL1));
  btn_pressed[1] = !digitalRead(COL2);//~(PORTD & (1 << COL2));
  btn_pressed[2] = !digitalRead(COL3);//~(PORTD & (1 << COL3));
  for (int i=0; i<3; i++){
    if (btn_pressed[i] && start_time[calls][i]==0){
    	start_time[calls][i]=millis();
        report_to_user=true;
    }
    else if (!btn_pressed[i] && start_time[calls][i]>0){
        released_buttons[calls][i]=true;
    	report_to_user=true;
    }
  }
  calls++;
  if (report_to_user && calls==3) { report(); }
  calls=calls%3;
}

void report() {
  String pressed_buttons = "";
  int pressed_count = 0;

  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < 3; col++) {
      if (released_buttons[row][col]) {
        Serial.print("Button ");
        Serial.print(1 + row * 3 + col);
        Serial.print(" released. You held it for - ");
        Serial.print(millis() - start_time[row][col]);
        Serial.print(" ms. Button was pressed at ");
        Serial.print(start_time[row][col]);
        Serial.println(" ms.");
        
        start_time[row][col] = 0;
        released_buttons[row][col] = false;
      }
      if (start_time[row][col] > 0) {
        if (pressed_count > 0) {
          pressed_buttons += ", ";
        }
        pressed_buttons += String(1 + row * 3 + col);
        pressed_count++;
      }
    }
  }
  if (pressed_count > 0) {
    Serial.print("Button pressed. Now you hold: ");
    Serial.print(pressed_count);
    Serial.print(pressed_count > 1 ? " buttons: " : " button: ");
    Serial.println(pressed_buttons);
  }
  else {
  	Serial.println("You released all buttons.");
  }
  report_to_user = false;
}

void loop(){}