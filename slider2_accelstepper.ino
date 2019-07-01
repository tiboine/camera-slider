
const int numberOfPieces = 4;
String pieces[numberOfPieces];
String input = "";

// Keep track of current position in array
int counter = 0;

// Keep track of the last comma so we know where to start the substring
int lastIndex = 0;

#include <AccelStepper.h>
#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11 //for software serial
#define stepPin1 6
#define dirPin1 5
SoftwareSerial SerialBT(rxPin, txPin);
AccelStepper stepperX(1, stepPin1, dirPin1);
bool test = false;
bool startCapture = true;


int state  = 0;

int camera = 9;
int frames = 10;
int interval = 1;
int hours;
int minutes;
int exposure = 1;
String command;
int stepperNow;
int stepperA;
int stepperB;
int stepperDiff;
int stepsToTake;
int currentPos = stepperX.currentPosition();
//int VoltValue = 0;

bool captureStart = true;

void setup() {
  stepperX.setMaxSpeed(4000);
  stepperX.setAcceleration(2000.0);
  stepperX.setSpeed(2000);
  // u8x8.begin();
  // u8x8.setFont(u8x8_font_chroma48medium8_r);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(camera, OUTPUT);
  Serial.begin(9600);
  SerialBT.begin(9600); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  //VoltValue = analogRead(system_voltage_level);

}

void loop() {
  //SerialBT.println("print");
  // SerialBT.write("write");
  // delay(500);
  stepperX.run();
  if (test == true) {
    Serial.println("test data");
    Serial.print("exposure: ");
    Serial.println(exposure, 4);
    Serial.print("Frames: ");
    Serial.println(frames);
    Serial.print("hours: ");
    Serial.println(hours);
    Serial.print("minutes: ");
    Serial.println(minutes);
    Serial.print("interval: ");
    Serial.println(interval);
    Serial.print("stepperNow: ");
    Serial.println(stepperNow);
    Serial.print("A: ");
    Serial.println(stepperA) ;
    Serial.print("B: ");
    Serial.println(stepperB);
    Serial.print("stepperDiff: ");
    Serial.println(stepperDiff);
    Serial.print("CurrentPosition: ");
    Serial.println(stepperX.currentPosition());
    Serial.println("-----------------");
    test = false;
  }


  if (SerialBT.available())
  {
    char c = SerialBT.read();

    if ( c == '\n')
    {
      parseCommand(command);
      command = "";
    }
    else
    {
      command += c;
    }
  }
}

void stepper2() {
  digitalWrite(dirPin1, HIGH);
  for (int i = 2000; i > 250; i--)  {
    //for(int j = 1500; j > 250; j--) {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(i);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(250);
  }
}


void stepper(int steps, int fart) {
  int dir = steps > 0 ? HIGH : LOW;

  digitalWrite(dirPin1, dir);
  if ( steps > 0 ) {
    for (int i = 0; i < steps; i++) {
      digitalWrite(stepPin1, HIGH);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(fart);
      stepperNow++;
    }
  }
  else {
    for (int i = 0; i > steps; i--) {
      digitalWrite(stepPin1, HIGH);
      digitalWrite(stepPin1, LOW);
      delayMicroseconds(fart);
      stepperNow--;
    }
  }

}

void parseCommand(String com)
{
  String part1;
  String part2;

  part1 = com.substring(0, com.indexOf(" "));
  part2 = com.substring(com.indexOf(" ") + 1);

  if (part1.equalsIgnoreCase("interval"))
  {
    interval = part2.toInt();
    test = true;
  }
  if (part1.equalsIgnoreCase("hours")) {
    hours = part2.toInt();
    test = true;
  }
  if (part1.equalsIgnoreCase("minutes")) {
    minutes = part2.toInt();
    test = true;
  }
  if (part1.equalsIgnoreCase("exposure")) {
    exposure = part2.toFloat();
    test = true;
  }
  if (part1.equalsIgnoreCase("frames")) {
    frames = part2.toInt();
    test = true;
  }

  if (part1.equalsIgnoreCase("left")) {
    stepperX.move(2000);
    stepperNow += 2000;
    stepperX.run();
    //stepper(1000, 250);
    test = true;
  }
  if (part1.equalsIgnoreCase("right")) {
    //stepper(-1000, 250);
    stepperX.move(-2000);
    stepperNow -= 2000;
    stepperX.run();
    test = true;
  }
  if (part1.equalsIgnoreCase("ding")) {
    //stepper(-1000, 250);
    Serial.println("yolo");
  }
  if (part1.equalsIgnoreCase("a")) {
    stepperA = stepperX.currentPosition();

    test = true;
  }
  if (part1.equalsIgnoreCase("b")) {
    stepperB = stepperX.currentPosition();

    test = true;
  }
  if (part1.equalsIgnoreCase("cancel")) {
    startCapture = false;
  }
  if (part1.equalsIgnoreCase("go")) {

    if (stepperA > stepperB) {
      stepperDiff = stepperA - stepperB;
    } else {
      stepperDiff = stepperB - stepperA;
    }
    // move to position A:


    stepperX.runToNewPosition(stepperA);
    stepperX.run();

    stepperNow = stepperNow + (stepperA - stepperNow);



    photoStart();
    test = true;

  }
}
void delayCaptureStart() {
  if (captureStart == true) {
    delay(1000);
    captureStart = false;
  }
}
void photoStart() {
  delayCaptureStart();
  stepsToTake = (int)stepperDiff / frames;
  int delayExposure;
  exposure != 1 ? delayExposure = exposure : delayExposure = 2; // set a minimum delay of 2

  for (int i = stepsToTake; i <=  stepperDiff; i = i + stepsToTake ) {

    digitalWrite(camera, HIGH);
    delay(delayExposure);
    digitalWrite(camera, LOW);
    delay(interval * 1000 - 500);

    if (stepperA < stepperB) {
      stepperX.runToNewPosition(stepperX.currentPosition() + stepsToTake);
      stepperX.run();
      // stepper(stepsToTake , 1000);
    } else {
      stepperX.runToNewPosition(stepperX.currentPosition() + stepsToTake * -1);
      stepperX.run();
      // stepper(stepsToTake * -1 , 1000);
    }

    delay(500);

    Serial.print("steps pr move: ");
    Serial.println(i);
    Serial.print("Steps to take: ");
    Serial.println(stepsToTake);
    Serial.print("stepperdiff: ");
    Serial.println(stepperDiff);

  }
}
