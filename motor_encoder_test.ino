/*
   Using the YYH-1 h-bridge, the ETONM Motor and a 16x2 LCD display
*/

// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>

//LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//PINS
const int ENC_A = 2; //Encoder Output A
const int ENC_B = 3 ; //Encoder Output B
const int HB_IN1 = 51; //H-Bridge
const int HB_IN2 = 50; //H-Bridge
const int HB_IN3 = 49; //H-Bridge
const int HB_IN4 = 48; //H-Bridge

const int FWD_BUTTON = 8; //Manual Forward
const int BWD_BUTTON = 9; //Manual Backward
const int MODE_BUTTON = 10; //Mode button
const int FLUSH_BUTTON = 11; //Flush button

const int FLUSH_LENGTH = 3000;

const int ledPin = 13;

int ForwardButtonState = 0;
int BackwardButtonState = 0;
int ModeButtonState = 0;
int FlushButtonState = 0;

long pulseCount = 0;
long flushCount = 0;


Encoder myEncoder(ENC_A, ENC_B);

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Test:");

  //outputs
  pinMode(HB_IN1, OUTPUT);
  pinMode(HB_IN2, OUTPUT);
  pinMode(HB_IN3, OUTPUT);
  pinMode(HB_IN4, OUTPUT);
  pinMode(ledPin, OUTPUT);

  //inputs
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);

  pinMode(FWD_BUTTON, INPUT);
  pinMode(BWD_BUTTON, INPUT);
  pinMode(MODE_BUTTON, INPUT);
  pinMode(FLUSH_BUTTON, INPUT);



  //LCD
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("Ywrobot Arduino!");
  lcd.setCursor(0, 2);
}
void MotorForwards() {
  digitalWrite(HB_IN1, LOW);
  digitalWrite(HB_IN2, HIGH);
  digitalWrite(HB_IN3, HIGH);
  digitalWrite(HB_IN4, LOW);
}
void MotorStop() {
  digitalWrite(HB_IN1, LOW);
  digitalWrite(HB_IN2, LOW);
  digitalWrite(HB_IN3, LOW);
  digitalWrite(HB_IN4, LOW);
}
void MotorBackwards() {
  digitalWrite(HB_IN1, HIGH);
  digitalWrite(HB_IN2, LOW);
  digitalWrite(HB_IN3, LOW);
  digitalWrite(HB_IN4, HIGH);
}

void Flush() {
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("FLUSH PRESSED");
  flushCount = myEncoder.read();
  Serial.print("FLush COunt");
  Serial.println(flushCount);
  long fc = flushCount;
  while ((fc - flushCount) < FLUSH_LENGTH) {
    lcd.setCursor(0,1);
    lcd.print(myEncoder.read());
    fc = myEncoder.read();
    MotorForwards();
   }
   Serial.print("fc = ");
   Serial.println(fc);
   Serial.print("flushCount = ");
   Serial.println(flushCount);
   Serial.print(" FLUSH length and count = ");
   Serial.println(flushCount + FLUSH_LENGTH);
   MotorBackwards();
   delay(10);
   MotorStop();
   lcd.setCursor(0,0);
   lcd.print("                    ");
}
void loop() {
  delay(300);
  ForwardButtonState = digitalRead(FWD_BUTTON);
  BackwardButtonState = digitalRead(BWD_BUTTON);
  ModeButtonState = digitalRead(MODE_BUTTON);
  FlushButtonState = digitalRead(FLUSH_BUTTON);

  if (ForwardButtonState == HIGH) {
    MotorForwards();
    Serial.println(myEncoder.read());
  } else if (BackwardButtonState == HIGH) {
    MotorBackwards();
    Serial.println(myEncoder.read());
  } else if (ModeButtonState == HIGH) {
    MotorStop();
    Serial.println("Mode button pressed");
  } else if (FlushButtonState == HIGH) {
    Serial.println("Flush button pressed");
    Flush();
  } else {
    MotorStop();
  }
}
