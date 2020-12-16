/*........................
  Firmware  System for Emergency Lung Ventilator v 0.0.7
  2020/09/21
  Set-up: 4 STEPPER Motors & 2 BT6600 drivers / 2 limitswitchers / 1 I2C 16x2 Monochromatic display / 3x analog potenciometer / 2x standard switches
  Written by: DEX Innovation Centre
*/

/*
  Current version includes:
  - Homing of both Arms, but not separately 
  - Pressing bag by very basic code, without any adjustments to the real inputs
  - I2C Monochramic display integrated
  - Analog potentiometer control integrated
  - Set-up of operation values including visualisation at Display
  - Controls of the System, basic functionality (depends on the readiness of the prototype)
  - READY TO SET-UP FOR PRESSIRNG REAL TIDAL VOLUME  
  - 2 Arms management - independent homing of both arms - DONE & TESTED
  - 2nd version of the potentiometers (664 max) - DONE & TESTED
  - control of the system movements - calibrating during the main loop, if limit switch is pressed - DONE& alpha tested 
  - Tidal volume function corrected and finalized based on the empirical testing 
  - Check of Confirmation Button at the star-up
  - Running of the initial set-up till the Confirmation Button is not activated 
*/


/* ADDED FROM LAST RELEASE
  - serious change of the whole code 
  - it's possible to switch off the operation by Confirmation button any time and set-up new 3 key values (Tidal Volume, RR, I/E)
*/


/*Next version: 

MAXIMAL PERFORMANCE TEST UTILITY
- it will be used to check, if the 


V 0.0.8
- Add the Homing message to the Display
- Add the message about setting up of the Ventilator
- test Maximum Tidal Volume with new function, I think, it can be an issue

V 0.0.9
- automatic calibration of speed of the closing and opening the arms based on the RR and I/E ratio. Algorithm: 
  - the first we will set-up target position of the movement, based on the TIDAL volume function
  - based on I/E ratio and RR we will calculate, how much miliseconds we have for one breath
  - we will do the algorithm based on the measurement of the real time for achieving the TIDAL volume
  - as first we will run the system in two, rather bordering conditions - lets' say speed of 150 and speed of 4000 (to be tested in real) - these values to be find
by the empricial testing - the system SHALL NEVER move out from given borders
  - this gives us two rather limit values for the time, which system needed for the pressing the needed TIDAL VOLUME. The maximum performance test can be also used as
  maximum performance test. 
  - as next we will find the ratio ( where is the time we need between these two values)
  - as first we will implement the simple algorithm, while speed is going to be adjusted by factor of 2 (it means if we find then the requested time is twice smaller then real time
  then we will change it not by 100 % but by 50% (1.5 instead of 2) )
  - the precision of algorithm: for simplification, we will focus on 0.1 sec precision at the begining ( 100 miliseconds). In other words: if the difference between the measured 
  time and real time is bellow 100 ms, we keep it as it's 
  
  

*/


#include <AccelStepper.h>

/*DEFINE THE PIN'S WIRING FOR THE WHOLE SYSTEM
  DIR for BT6600 drivers (connected in series) - PIN 2
  STEP for BT6600 drivers (connected in series) - PIN 3
  LEFT LIMIT SWITCH (looking from the head of Bag - head is the side connected to lung/patient) - PIN 8
  RIGHT LIMIT SWITCH (looking from the head of Bag - head is the side connected to lung/patient) - PIN 9
  I2C Display SCL connection - SCL PIN 
  I2C Display SDA connection - SDA PIN 
  Left / AirVolume Potentiometer - A1 PIN
  Middle / RRValue Potentiometer - A2 PIN
  Rigth / IERatio Potentiometer - A3 PIN 
  */
  
#define Right_DirPin 2
#define Right_StepPin 3
#define Left_DirPin 4
#define Left_StepPin 5
#define Left_Limit_Switch 7
#define Right_Limit_Switch 8
#define AirVolume_PIN  A1
#define RRValue_PIN A2
#define IERatio_PIN A3
#define ConfirmationButton 13

// Create a new instance of the AccelStepper class:
AccelStepper Right_CAMArms = AccelStepper(1,Right_StepPin, Right_DirPin);
AccelStepper Left_CAMArms = AccelStepper(1,Left_StepPin, Left_DirPin);

//Arms failure values
int Left_Arm_Failure;
int Right_Arm_Failure; 

//definition of supportive variables
long Long_i = 1; // Used to: i) homing Right_CAMArms ; ii) Supportive variable to run the definition loop for the inputs

// global variables defining key parameters for the ventilation
int TidalVolume = 200; // Tidal Volume (TV) (air volume pushed into lung): between 200 – 800 mL based on patient weight.
int RRValue = 20; // Respiratory Rate (RR) (breaths per minute): between 6 – 40. Note that the low RRs of 6 – 9 are only applicable to Assist Control.
float IERatio = 1.0; // (inspiratory/expiration time ratio): recommended to start around 1:2; best if adjustable between range of 1:1 – 1:3*.

/*
-------------------------
FOR TESTING PURPOSES ONLY 
-------------------------
-------------------------
*/
unsigned long time;


void setup() {

  // just for testing
  Serial.begin(115200);
  Serial.println("init");
  // just for testing 

}


void loop() {
  
    // just for testing
  time=0;
  // just for testing 
  /*
   //start of the reading of the inputs from the Potentiometers
  //while (time<20000) /// BE AWARE FOR CURRENT VERSION THERE IS 20.000 of miliseconds to set-up the system, then it starts automatically. If you need to shorten or prolong, please change the variable here 
  while (!digitalRead(ConfirmationButton))
  {
    int A1Reading = analogRead(AirVolume_PIN);
    //!!! VERSION FOR PROTOTYPE NO.2
    TidalVolume=200+A1Reading/1.705; // HERE CHANGE IF YOU NEED TO CHANGE THE RATIO
    //!!! VERSION FOR THE PROTOTYPE NO.3
    //TidalVolume=200+A1Reading/1.105; // HERE CHANGE IF YOU NEED TO CHANGE THE RATIO
    Serial.print(A V );
    Serial.println(TidalVolume);
    
    //reading of the RR (Respiratory Rate), while linear conversion rate is applied. The limits: 6 - 40 RR per minute 
    lcd.setCursor(3,1);
    
    int A2Reading = analogRead(RRValue_PIN); // HERE CHANGE IF YOU NEED TO CHANGE THE RATIO
    //!!! VERSION FOR PROTOTYPE NO.2
    RRValue=6+A2Reading/30;
    //!!! VERSION FOR PROTOTYPE NO.3
    //RRValue=6+A2Reading/19.5;
    if (RRValue < 10) {
      lcd.print("0");
      lcd.print(RRValue);
    }
    else {
      lcd.print(RRValue); 
    }
    
    // reading of the Expiratory value, while linear conversion rate is applied. The limits of I/E are from 1:1 to 1:3
    lcd.setCursor(13,1);
    int A3Reading = analogRead(IERatio_PIN);
    //!!! VERSION FOR PROTOTYPE NO.2
    IERatio=1.0+A3Reading/510.0; // HERE CHANGE IF YOU NEED TO CHANGE THE RATIO
    //!!! VERSION FOR PROTOTYPE NO.3
    //IERatio=1.0+A3Reading/331.0; // HERE CHANGE IF YOU NEED TO CHANGE THE RATIO
    lcd.print(IERatio);
    
    
   /*
  -------------------------
  FOR TESTING PURPOSES ONLY 
  -------------------------
  -------------------------
  */ 

    time = millis();
  
  
  
  /*
  -------------------------
  //HOMING OF THE SYSTEM
  -------------------------
  -------------------------
  */
  
  pinMode(Left_Limit_Switch, INPUT);
  pinMode(Right_Limit_Switch, INPUT);
  delay(10);
  //  Set Max Speed and Acceleration of each Steppers at startup for homing
  Right_CAMArms.setMaxSpeed(1000);
  Left_CAMArms.setMaxSpeed(1000);

  // Homing of both arms 
  while (!digitalRead(Left_Limit_Switch) && !digitalRead(Right_Limit_Switch)) {
    Serial.println("homing");
    Right_CAMArms.moveTo(Long_i);  // Set the position to move to
    Left_CAMArms.moveTo(Long_i);
    Right_CAMArms.setSpeed(-500);  // Set Speed of Stepper (Slower to get better accuracy)
    Left_CAMArms.setSpeed(500);
    Long_i++;  // Decrease by 1 for next move if needed
    Right_CAMArms.run();  // Stepper does 1 step (possibility to test to decrease Right_StepPing to 1/2 step)
    Left_CAMArms.run();
    delay(5);
  }
  Serial.println("homed");
  delay(1000);
  

  // One ARM is in the home position, but not both. Test which one is homed and moved another there :) 
  if (!digitalRead(Right_Limit_Switch)) {
    // RIGHT Cam ARM has to be still homed 
    while (!digitalRead(Right_Limit_Switch)) {  // CAMArm is opening till it hits the switch
      Right_CAMArms.moveTo(Long_i);  // Set the position to move to
      Right_CAMArms.setSpeed(500);  // Set Speed of Stepper (Slower to get better accuracy)
      Long_i++;  // Decrease by 1 for next move if needed
      Right_CAMArms.run();  // Stepper does 1 step (possibility to test to decrease Right_StepPing to 1/2 step)
      delay(5);
    }
  } 
  else {
      // LEFT Cam ARM has to be still homed 
    while (!digitalRead(Left_Limit_Switch)) {  // CAMArm is opening till it hits the switch
      Left_CAMArms.moveTo(Long_i);  // Set the position to move to
      Left_CAMArms.setSpeed(500);  // Set Speed of Stepper (Slower to get better accuracy)
      Long_i++;  // Decrease by 1 for next move if needed
      Left_CAMArms.run();  // Stepper does 1 step (possibility to test to decrease Right_StepPing to 1/2 step)
      delay(5);
    }
  }
  delay(2000);

  // The both Limit Switches are pressed now
  // So we will be opening both ARMS a bit, we asume that both ARMS are opening freely 
  while (digitalRead(Left_Limit_Switch)) { // Make the Stepper move CW until the switch is deactivated
    Right_CAMArms.moveTo(Long_i);
    Left_CAMArms.moveTo(Long_i);
    Right_CAMArms.setSpeed(-950);  // Set Speed of Stepper (Slower to get better accuracy)
    Left_CAMArms.setSpeed(-950);
    Right_CAMArms.run();
    Left_CAMArms.run();
    Long_i--;
    //delay(5);
  }
  // despite there is no contact in the circuit, the switchers are mechanically still touched, better to move a bit more away
  for (int count = 0; count <200; count++) { // Change CONSTANT 60 to adjust the arms to be just touching limit switch 
    Right_CAMArms.moveTo(Long_i);
    Left_CAMArms.moveTo(Long_i);
    Right_CAMArms.setSpeed(-950);  // Set Speed of Stepper (Slower to get better accuracy)
    Left_CAMArms.setSpeed(-950);
    Right_CAMArms.run();
    Left_CAMArms.run();
    Long_i--;
    delay(5);
  }

  // Arms are in HOME POSITION
  //--------------------------
  Right_CAMArms.setCurrentPosition(0);
  Left_CAMArms.setCurrentPosition(0);
  delay(3000);

  
  /////
  ///
  //
  
  
  
  //Deleting of the possible Failures of both arms:
  Left_Arm_Failure = 0;
  Right_Arm_Failure = 0;  
  
  /* TO BE SET-UP BASED ON THE REAL READINGS FOR THE TIDAL VOLUME 
  The TidalVolume is integral value which is set in the ration from 200 to 800 ml. The objective is that system flows correct amount of the air volume. 
  The current function is linear one, while 200 ml was set-up to be equal position 100 and 800 ml was to be equal to 300
  For changes modify the standard linear mapping equation( values 6 and 60)  =(TidalVolume-200)/3+100 
  */
  int ArmPosition = (TidalVolume-200)/0.3+650;

  
  while (digitalRead(ConfirmationButton))
  {
  
    // This code is CLOSING ARM
    //-------------------------
    // Position of the arm is already managed by the Tidal Volume function, however the equation is NOT TESTED
    while(Right_CAMArms.currentPosition() != -ArmPosition && Left_CAMArms.currentPosition() != -ArmPosition)
    {
      Right_CAMArms.setSpeed(-500);// this number regulates the speed of closing the arms 
      Right_CAMArms.runSpeed();
      Left_CAMArms.setSpeed(-500);// this number regulates the speed of closing the arms 
      Left_CAMArms.runSpeed();
    }
    delay(1000);  
  
    //THIS CODE IS OPENING ARM
    //-------------------------
    while(Right_CAMArms.currentPosition()!= 0 && Left_CAMArms.currentPosition() != 0 && Right_Arm_Failure == 0 && Left_Arm_Failure == 0)  //this number shall be reverse to the function while for opening arm 
    {
      Left_CAMArms.setSpeed(500);// this number regulates the speed of closing the arms 
      Left_CAMArms.runSpeed();
      Right_CAMArms.setSpeed(500); // this number regulates the speed of opening the arms 
      Right_CAMArms.runSpeed();
      if (digitalRead(Right_Limit_Switch)) { Right_Arm_Failure = !Right_Arm_Failure;} 
      if (digitalRead(Left_Limit_Switch)) { Left_Arm_Failure = !Left_Arm_Failure;}    
    }
  
    //RIGT ARM FAILURE => We will let fail also opposite arm
    //------------------------------------------------------
    if (Right_Arm_Failure == 1)
    {
      while (Left_Arm_Failure == 0)
      {
      Left_CAMArms.setSpeed(500);// this number regulates the speed of closing the arms 
      Left_CAMArms.runSpeed();
      if (digitalRead(Left_Limit_Switch)) { Left_Arm_Failure = !Left_Arm_Failure;}     
      }
    }
  
     //LEFT ARM FAILURE => in any case, we have managed to fail Left Arm too. If Right Arm failure was cause, the code will immediately loop out 
    //------------------------------------------------------
    if (Left_Arm_Failure == 1)
    {
      while (Right_Arm_Failure == 0)
      {
      Right_CAMArms.setSpeed(500);// this number regulates the speed of closing the arms 
      Right_CAMArms.runSpeed();
      if (digitalRead(Right_Limit_Switch)) { Right_Arm_Failure = !Right_Arm_Failure;}     
      }
    }
    
    //IN case of any failure we run as quick as possible the homing procedure (note that if ANY FAILURE happened, both Failures (right/left) are already activated)
    // IN-CYCLE HOMING PROCEDURE
    // -------------------------
    if (Right_Arm_Failure == 1)
    {
      Long_i=0;
      // The both Limit Switches are pressed now
      // So we will be opening both ARMS a bit, we asume that both ARMS are opening freely 
      while (digitalRead(Left_Limit_Switch)) { // Make the Stepper move CW until the switch is deactivated
        Right_CAMArms.moveTo(Long_i);
        Left_CAMArms.moveTo(Long_i);
        Right_CAMArms.setSpeed(-950);  // Set Speed of Stepper (Slower to get better accuracy)
        Left_CAMArms.setSpeed(-950);
        Right_CAMArms.run();
        Left_CAMArms.run();
        Long_i--;
      }
      // despite there is no contact in the circuit, the switchers are mechanically still touched, better to move a bit more away
      for (int count = 0; count <60; count++) { // Change CONSTANT 60 to adjust the arms to be just touching limit switch 
        Right_CAMArms.moveTo(Long_i);
        Left_CAMArms.moveTo(Long_i);
        Right_CAMArms.setSpeed(-950);  // Set Speed of Stepper (Slower to get better accuracy)
        Left_CAMArms.setSpeed(-950);
        Right_CAMArms.run();
        Left_CAMArms.run();
        Long_i--;
      }
    Right_CAMArms.setCurrentPosition(0);
    Left_CAMArms.setCurrentPosition(0);
    }
    delay(1000);
  }
}
