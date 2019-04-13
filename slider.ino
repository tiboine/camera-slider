
/*
  slider
  Go from A to B in x time
  trigger camera

  1. set starting point
  2. set end point
  3. set interval


*/
//#include <SPI.h>
#include <Wire.h>
#define OLED_RESET 9
#define OLED_address 0x3c
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(OLED_RESET);
const int dirPin1 = 7;
const int stepPin1 = 6;

int button = 8;
int delayy = 500;
int X_pin = A3; // analog pin connected to X output
int Y_pin = A2; // analog pin connected to Y output
int x = 0;
int y;
int counter = 0;
int prevCounter = 0;
int buttonState = 0;
int a;
int b;
void setup()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c); // initialize with the I2C addr 0x3D (for the 128x64)
  display.clearDisplay(); // clears the screen and buffer
  display.setTextSize(2);
  display.setTextColor(WHITE);
  //attachInterrupt(digitalPinToInterrupt(interruptPin), isr, CHANGE) ;
  Serial.begin(9600);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(2,INPUT_PULLUP);

  Serial.println("hardware serial OK");
  
}
void loop()
{

  int sensorVal = digitalRead(2);
  if (sensorVal == LOW){
    Serial.println(sensorVal);
  }
  
y = analogRead(Y_pin);
  x = analogRead(X_pin);
  if (y > 700) {
    a = counter;
  }
  if (y < 300) {
    b = counter;
  }

  // lage ny steppermotor funksjon


  if (prevCounter != counter) {
    Serial.println(counter);
    prevCounter = counter;
  }
  if (x > 700)
  {
    steppermotor(HIGH);

  }
  if (x < 300)
  {
    steppermotor(LOW);

  }

  //oled();
}


void steppermotor(int dir)
{
  for (int i = 0; i < 100; i++)
  {
    digitalWrite(dirPin1, dir);
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(50);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(50);
    if (dir == HIGH) {
      counter++;
    }
    else {
      counter--;
    }
    //Serial.println(counter);

  }
  prevCounter = prevCounter + counter;
}

void oled()
{
  display.display();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(counter);
  //
}
