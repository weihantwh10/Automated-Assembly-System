// Include the AccelStepper Library
#include <AccelStepper.h>
#include <Servo.h>
#include "CytronMotorDriver.h"

//Define Servo Motor
#define SERVO_NUT 7
#define SERVO_PART 8
#define SERVO_LIFT 9

//Define Pneumatics
#define CYLINDER_OUT 24
#define CYLINDER_EJECT 25

//Define Button
#define START 30
#define STOP 2

//Define LED
#define LED_GREEN 22
#define LED_RED 32
#define LED_YELLOW 16


//Define limit switches
#define LIMIT1 26
#define LIMIT2 27
#define LIMIT3 28
#define LIMIT4 14
#define LIMIT5 29

//Define IR
#define IR1 A0
#define IR2 A1
//#define buzzer A15

//Define Debouncing Delay
#define DEBOUNCE 50

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 4
#define stepPin 3
#define motorInterfaceType 1

// Define step constants
#define FULLSTEP 4
#define HALFSTEP 8

// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
AccelStepper Partstepper(FULLSTEP, 50, 52, 51, 53);
AccelStepper Reloadstepper(FULLSTEP, 18, 20, 19, 21);
AccelStepper NEMAstepper(motorInterfaceType, stepPin, dirPin);

CytronMD motor1(PWM_DIR, 10, 11);
CytronMD motor2(PWM_DIR, 12, 13);

Servo Nutservo;
Servo Partservo;
Servo Liftservo;


int Sequence = 0;
int NUT_pos = 0;    // variable to store the servo position
int LIFT_pos = 0;

void pinSetup(){
  pinMode(LIMIT1, INPUT_PULLUP);
  pinMode(LIMIT2, INPUT_PULLUP);
  pinMode(LIMIT3, INPUT_PULLUP);
  pinMode(LIMIT4, INPUT_PULLUP);
  pinMode(LIMIT5, INPUT_PULLUP);
  pinMode(CYLINDER_OUT, OUTPUT);
  pinMode(CYLINDER_EJECT, OUTPUT);
  attachInterrupt(0, pause, FALLING);
  pinMode(START, INPUT_PULLUP);
  pinMode(STOP, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);


}

void initStepper(){
  Partstepper.setMaxSpeed(650.0);
	Partstepper.setAcceleration(400.0);
  Partstepper.setCurrentPosition(0);
  Reloadstepper.setMaxSpeed(550.0);
	Reloadstepper.setAcceleration(500.0);
  Reloadstepper.setCurrentPosition(0);
  NEMAstepper.setMaxSpeed(7000);
  NEMAstepper.setAcceleration(20000);
  NEMAstepper.setCurrentPosition(0);
}

void initServo(){
  Nutservo.attach(SERVO_NUT);
  Partservo.attach(SERVO_PART);
  Liftservo.attach(SERVO_LIFT);
  Nutservo.write(NUT_pos);
  Liftservo.write(LIFT_pos);
  //delay(2000);
}

void pause(){
  delayMicroseconds(5000);
  if (digitalRead(STOP) == LOW){
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    while(digitalRead(START) == HIGH);
    delayMicroseconds(5000);
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
  }
}

void nutServo(){
  for (NUT_pos = 0; NUT_pos <= 119; NUT_pos += 1) { 
    // in steps of 1 degree
    Nutservo.write(NUT_pos);              
    delay(10);                       
  }
  delay(500);
  for (NUT_pos = 119; NUT_pos >= 0; NUT_pos -= 1) { 
    Nutservo.write(NUT_pos);              
    delay(10);                       
  }
  //delay(5000);
}

void nutServoBW(){
  for (NUT_pos = 0; NUT_pos <= 119; NUT_pos += 1) { 
    // in steps of 1 degree
    Nutservo.write(NUT_pos);              
    delay(10);                       
  }
}

void nutServoFW(){
   for (NUT_pos = 119; NUT_pos >= 0; NUT_pos -= 1) { 
    Nutservo.write(NUT_pos);              
    delay(10);                       
  }
}

void partStepperFW(){
  Partstepper.moveTo(-200000);

  while(digitalRead(LIMIT1) == HIGH){
    Partstepper.run();
  }
}

void partStepperBW(){
  Partstepper.moveTo(200000);
  while(digitalRead(LIMIT2) == HIGH){
    Partstepper.run();
  }
}

void partServoFW(){
  while(digitalRead(LIMIT3) == HIGH){
    Partservo.write(135);
  }
  //delay(510);
  Partservo.write(90);
  //delay(500);
}

void partServoBW(){
  while(digitalRead(LIMIT5) == HIGH){
    Partservo.write(45);
  }
  //delay(510);
  Partservo.write(90);
  //delay(500);
}


void reloadStepper(){
  Reloadstepper.moveTo(1019);
  Reloadstepper.runToPosition();
  //delay(3000);
  Reloadstepper.setCurrentPosition(0);

}

void liftServoUP(){
  for (LIFT_pos = 0; LIFT_pos <= 70; LIFT_pos += 1) { 
    // in steps of 1 degree
    Liftservo.write(LIFT_pos);              
    delay(10);                       
  }
}

void liftServoDOWN(){
  for (LIFT_pos = 70; LIFT_pos >= 0; LIFT_pos -= 1) { 
    Liftservo.write(LIFT_pos);              
    delay(10);                       
  }
}

void N20_START(){
  motor1.setSpeed(-50);  
  motor2.setSpeed(-50); 
}

void N20_100(){
  motor1.setSpeed(50);  
  motor2.setSpeed(50); 
}

void N20_N100(){
  motor1.setSpeed(-50);  
  motor2.setSpeed(-50); 
}

void N20_FULL(){
  motor1.setSpeed(-255);  
  motor2.setSpeed(-255); 
}

void N20_REVERSE(){
  motor1.setSpeed(150);  
  motor2.setSpeed(150); 
}
void N20_END(){
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  delay(1000);
  motor1.setSpeed(100);   
  motor2.setSpeed(100);  
  delay(500);
  motor1.setSpeed(0);
  motor2.setSpeed(0);
}

void NEMA17_HOMING(){
    NEMAstepper.moveTo(200000);
    // Run to target position with set speed and acceleration/deceleration:
    while (digitalRead(LIMIT4) == HIGH)
    {
      NEMAstepper.run();
    }
    NEMAstepper.setCurrentPosition(0);
}

void NEMA17_DOWN(){
  NEMAstepper.moveTo(-23000);
  //NEMAstepper.moveTo(-25500);
  NEMAstepper.runToPosition();
  NEMAstepper.setCurrentPosition(0);
}

void NEMA17_STEP(){
  NEMAstepper.moveTo(-500);
  //NEMAstepper.moveTo(-25500);
  NEMAstepper.runToPosition();
  NEMAstepper.setCurrentPosition(0);
}

void NEMA17_END(){
  NEMAstepper.moveTo(-1500);
  //NEMAstepper.moveTo(-25500);
  NEMAstepper.runToPosition();
}

void SCREWING(){
  NEMA17_HOMING();
  N20_START();
  NEMA17_DOWN();
    for (int i = 0; i < 2; i++)
  {
    N20_100();
    delay(500);
    N20_N100();
    delay(500);
  }

  NEMA17_STEP();
  for (int i = 0; i < 2; i++)
  {
    N20_100();
    delay(500);
    N20_N100();
    delay(500);
  }

  
  //N20_200();
  NEMA17_STEP();
    for (int i = 0; i < 2; i++)
  {
    N20_100();
    delay(500);
    N20_N100();
    delay(500);
  }

  N20_FULL();
  NEMA17_END();
  delay(4000);
  N20_END();
  NEMA17_HOMING();

}

void OUT(){
  digitalWrite(CYLINDER_OUT, HIGH);
  delay(1000);
}

void EJECT(){
  digitalWrite(CYLINDER_EJECT, HIGH);
  delay(1000);
  digitalWrite(CYLINDER_EJECT, LOW);
  delay(1000);
}

void IN(){
  digitalWrite(CYLINDER_OUT, LOW);
  delay(500);
}
 
void setup(){

  pinSetup();

	initStepper();

  initServo();

  while(digitalRead(START) == HIGH);
  delay(DEBOUNCE);
  digitalWrite(LED_GREEN, HIGH);
}

void loop() {

  switch (Sequence) {
    case 0:
      partStepperBW();
      partServoFW();
      partServoBW();
      nutServo();
      
      // int sensor1 = digitalRead(IR1);
      // if (sensor1 == HIGH){
      //   digitalWrite(buzzer, HIGH);
      //   digitalWrite(LED_YELLOW, HIGH);
        
      // }

      // delay(500);
      // liftServoUP();
      // delay(1000);
      // liftServoDOWN();
      NEMA17_HOMING();
      //reloadStepper();
      Sequence = 10;
      break;
      


    case 10:
      partStepperFW();
      partServoFW();
      //partServoBW();
      
      Sequence = 20;
      break;

    case 20:
      reloadStepper();
      for (int i = 0; i < 3; i++)
      {
        OUT();
        IN();
      }
      Partservo.write(45);
      delay(100);
      Partservo.write(90);
      Sequence = 30;
      break;

    case 30:
      liftServoUP();

      Sequence = 40;
      break;

    case 40:
      SCREWING();
      liftServoDOWN();
      partServoBW();
      nutServoBW();
      Sequence = 50;
      break;

    case 50:
      OUT();
      EJECT();
      Nutservo.write(90);
      delay(1000);
      IN();
      for (NUT_pos = 90; NUT_pos >= 0; NUT_pos -= 1) { 
      Nutservo.write(NUT_pos);              
      delay(10);                       
      }
      //nutServoFW();
      partStepperBW();
      Sequence = 10;
      break;


  }

  // int sensor1 = digitalRead(IR1);
  // if (sensor1 == HIGH){
  //   digitalWrite(buzzer, HIGH);
  //   digitalWrite(LED_YELLOW, HIGH);
  //   break;
  // }

  // int sensor2 = digitalRead(IR2);
  // while
}