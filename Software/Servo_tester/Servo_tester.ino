//http://www.arduino.cc/en/Tutorial/Debounce
/*
Code by Dhamdhawach  Horsuwan
https://www.facebook.com/menglabfanpage
Bangkok , Thailand
17 NOV 2018
Use with Hardware
https://github.com/Menginventor/Servo_tester
*/
#include <Servo.h>
#define MAX_CHANEL 7
#define BTN_PIN 2
#define POT_PIN A7
#define debounceDelay  50
uint8_t LED_PIN[] = {3, 5, 7, 9, 11, A3, A1};
uint8_t SERVO_PIN[] = {4, 6, 8, 10, 12, A2, A0};
uint8_t count = 0;
uint8_t btn_state;
uint8_t ctrl_ch = 0;

Servo servo_arr[MAX_CHANEL];
void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT);
  LED_pin_init();
  start_blink();

  servo_init();
}
void servo_init() {
  for (uint8_t i = 0; i < MAX_CHANEL; i++) {
    servo_arr[i].attach(SERVO_PIN[i]);
  }
}
void servo_write_all(uint16_t microsecs) {
  for (uint8_t i = 0; i < MAX_CHANEL; i++) {
    servo_arr[i].writeMicroseconds(microsecs);
  }
}
void LED_pin_init() {
  for (uint8_t i = 0; i < MAX_CHANEL; i++) {
    digitalWrite(LED_PIN[i], HIGH);
    pinMode(LED_PIN[i], OUTPUT);
  }
}
void start_blink() {
  for (int8_t i = 0; i < MAX_CHANEL; i++) {
    for (int8_t j = 0; j < MAX_CHANEL; j++) {
      if (i == j) {
        digitalWrite(LED_PIN[j], LOW);
      }
      else {
        digitalWrite(LED_PIN[j], HIGH);
      }
    }
    delay(25);
  }
  for (int8_t i = MAX_CHANEL - 1 ; i >= 0; i--) {
    for (int8_t j = 0; j < MAX_CHANEL; j++) {
      if (i == j) {
        digitalWrite(LED_PIN[j], LOW);
      }
      else {
        digitalWrite(LED_PIN[j], HIGH);
      }
    }
    delay(25);
  }
  ctrl_ch = 0;
}
void show_led_ctrl_ch(uint8_t ch) {
  for (int8_t j = 0; j < MAX_CHANEL; j++) {
    if (ch == j) {
      digitalWrite(LED_PIN[j], LOW);
    }
    else {
      digitalWrite(LED_PIN[j], HIGH);
    }
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  btn_loop();
  //Serial.println(analogRead(POT_PIN));
  uint16_t servo_val = map(analogRead(POT_PIN), 0, 1023, 500, 2500);
  servo_arr[ctrl_ch].writeMicroseconds(servo_val);


}

void btn_pressed() {

}
void btn_released(bool holding) {
  Serial.println(holding);

  if (holding)
    ctrl_ch = 0;
  else {
    ctrl_ch ++;
    if (ctrl_ch >= MAX_CHANEL)ctrl_ch = 0;

  }
  show_led_ctrl_ch(ctrl_ch);
}


void btn_loop() {
  static uint8_t lastButtonState = LOW;
  static uint8_t holding = false;
  static uint32_t hold_btn = 0;
  static uint32_t   lastDebounceTime = 0;
  uint8_t reading = digitalRead(BTN_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != btn_state) {
      btn_state = reading;
      //Serial.println(btn_state);
      if (btn_state == HIGH) {
        btn_pressed();
        hold_btn = millis();
        holding = false;
      }
      else {
        if (millis() - hold_btn > 500)holding = true;
        btn_released(holding);

      }
    }

  }

  lastButtonState = reading;
}

