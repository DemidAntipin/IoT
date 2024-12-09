#define pin_2 2
#define pin_3 3
#define pin_4 4
#define IMAGE_DURATION 3000

long int start = 0;
int message_len = 8;
int curr_len = 0;
int index = 0;
int calls = 0;

byte data[10] = {
  0b00000010,
  0b01001111,
  0b00010001,
  0b00000101,
  0b01001100,
  0b00100100,
  0b00100000,
  0b00001111,
  0b00000000,
  0b00000100
};

void setup() {
  Serial.begin(9600);
  pinMode(pin_2, OUTPUT);
  pinMode(pin_3, OUTPUT);
  pinMode(pin_4, OUTPUT);
  
  cli(); // Отключение прерываний
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 512; // Установка значения сравнения
  
  TCCR1B |= (1 << WGM12); // Включение режима CTC
  TCCR1B |= (1 << CS10); // Установка предделителя (1)
  
  TIMSK1 |= (1 << OCIE1A); // Включение прерывания по совпадению
  sei(); // Включение прерываний
  
  DDRD |= (1 << pin_2) | (1 << pin_3) | (1 << pin_4); // Установка направлений для портов
  PORTD = 0; // Сброс состояния портов
  start = millis();
}

ISR(TIMER1_COMPA_vect) {
  calls++;
  if (calls == 1) {
    PORTD = ((PORTD & ~(1 << pin_4)) | (((data[index] >> (curr_len)) & 1) << pin_4));
    curr_len++;
  } 
  else if (calls != 3) {
    PORTD ^= (1 << pin_2); // Изменение состояния второго пина
  }
  
  if (curr_len == message_len && calls == 4) {
    PORTD |= (1 << pin_3); // Установка состояния третьего пина в HIGH
    curr_len = 0; // Сброс длины сообщения
  } 
  else {
    PORTD &= ~(1 << pin_3); // Установка состояния третьего пина в LOW
  }
  calls = calls % 4; // Сброс счетчика вызовов
}

void loop() {
  if (Serial.available() > 0) {
  char input = Serial.read();
    if (input >= '0' && input <= '9') {
      index = input - '0';
      start=millis();
    }
  }
  if (millis() > start + IMAGE_DURATION) {
    index = (index + 1) % sizeof(data); // Переход к следующему индексу в массиве данных
    start = millis();
  }
}