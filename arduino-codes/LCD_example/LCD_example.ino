#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

int PUL=10; //define Pulse pin
int DIR=9; //define Direction pin
int ENA=4; //define Enable Pin

int endstop_a = 7;
int endstop_b = 6;
unsigned int count = 0;
bool op = false;
void home(){
  Serial.println("homing");
  while(digitalRead(endstop_a)){
    digitalWrite(ENA,true);
    digitalWrite(DIR, false);
  }
  digitalWrite(ENA, false);
  Serial.println("succesfull homed");
}

void setup() {

  TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz only PWM D9 and D10
  Serial.begin(115200);
  Serial.println("init");
  lcd.begin();
  lcd.backlight();
  lcd.print("Loading Init");
  lcd.setCursor ( 0, 1 );
  lcd.print("github.com/VPetras");
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
  pinMode (endstop_a, INPUT);
  pinMode (endstop_b, INPUT);

  digitalWrite(ENA, false);
  analogWrite(PUL, 127);
  home();
}

void loop() {
  digitalWrite(ENA, true);
  if (digitalRead(endstop_a) == false) {
    digitalWrite(DIR, true);
  }
  else if (digitalRead(endstop_b) == false) {
    digitalWrite(DIR, false);
  }
}
