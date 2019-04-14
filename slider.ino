#include <A4988.h>
#include <BasicStepperDriver.h>
#include <DRV8825.h>
#include <DRV8834.h>
#include <DRV8880.h>
#include <MultiDriver.h>
#include <SyncDriver.h>




/*
  slider
  Go from A to B in x time
  trigger camera

  1. set starting point
  2. set end point
  3. set interval

*/

#include <Wire.h>
#define OLED_RESET 9
#define OLED_address 0x3c
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(OLED_RESET);

const int dirPin1 = 7;
const int stepPin1 = 6;
const int X_pin = A3; // analog pin connected to X output
const int Y_pin = A2; // analog pin connected to Y output
const int button = 2;
const int led = 13;

int x = 0;
int y;
int counter = 0;
int prevCounter = 0;
int buttonState = 0;
int lastButtonState = 0;
int buttonCounter = 0;
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
  pinMode(led, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Serial.println("hardware serial OK");

}
void loop()
{
 
  switch (buttonCounter) {
    case 0:
      a = counter;
      
      break;

    case 1:
      stepper(-a, 500);
      Serial.print("moved case 1 :");
      Serial.println(a);
      buttonCounter++;
      delay(10);
      break;

    case 2:
      stepper(a, 150);
      Serial.print("moved case 2 :");
      Serial.println(a);
      buttonCounter++;
      break;


  }




  int buttonState = digitalRead(button);
  if (buttonState != lastButtonState) {
    if (!buttonState) {
      digitalWrite(led, HIGH);
      buttonCounter++;
      if (buttonCounter > 2) {
        buttonCounter = 0;
      }
      //Serial.println(buttonCounter);
    }
  }

  else {
    digitalWrite(led, LOW);
  }
  lastButtonState = buttonState;
  y = analogRead(Y_pin);
  x = analogRead(X_pin);
  if (y > 700) {
  }
  if (y < 300) {
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

   if (prevCounter != counter) {
    
    
  display.display();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(counter);
  prevCounter = counter;
   } 
  //
}

//stepper
void stepper(int steps, int speed) {
  int dir = steps > 0 ? HIGH : LOW;

  digitalWrite(dirPin1, dir);
  if ( steps > 0 ) {
    for (int i = 0; i < steps; i++) {
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(speed);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(speed);
    }
  }
  else {
    for (int i = 0; i > steps; i--) {
      digitalWrite(stepPin1, HIGH);
      delayMicroseconds(50);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(50);
    }
  }
}
