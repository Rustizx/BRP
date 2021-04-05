/* Blayone Rocket Project - BONESOFT
      Version 0.02
    October, 18th 2020 */

/* Current version of Bonesoft does not include bluetooth
   as development with bluetooth is impossible for me. To 
   use bluetooth with React Native I would need to edit native
   libraries and for me to export would require an up to date
   Macbook to have access to IOS Development Tools */

/* Current version of Bonesoft also does not contain
   SPI Slave code as there is no need for Bone to know
   the telemetry of Arch at the moment. When BoneApp is fully 
   functional, then I will need it */

/* -------------
    Libraries
---------------- */
#include <Arduino.h>
#include <Servo.h>

/* -----------------
    Definitions
------------------ */
#define VERSION 0.05

//#define chipSelect 1
#define ch5RADIO 12
#define ch9RADIO 13

//#define boneFireMotorPIN 10
//#define boneSupportPIN 11
#define boneLaunchTriggeredPIN 6

#define pyroChannel 11

#define lServoPIN 10
#define rServoPIN 9
#define wireServoPIN 21

#define statusLEDPIN 5

bool launchRec = false;
float channel5, channel9;
int tick, led;

Servo leftServo;
Servo rightServo;

/* --------------------------------
    Main Functions (SETUP, LOOP)
------------------------------------ */

void setup()
{
  pinMode(boneLaunchTriggeredPIN, OUTPUT);
  pinMode(pyroChannel, OUTPUT);
  pinMode(statusLEDPIN, OUTPUT);

  pinMode(ch5RADIO, INPUT);
  pinMode(ch9RADIO, INPUT);
  //pinMode(boneFireMotorPIN, INPUT);
  //pinMode(boneSupportPIN, INPUT);
  //pinMode(chipSelect, INPUT);

  digitalWrite(pyroChannel, LOW);
  digitalWrite(boneLaunchTriggeredPIN, LOW);

  leftServo.attach(lServoPIN);
  rightServo.attach(rServoPIN);

  leftServo.write(0);
  rightServo.write(0);

  tick = 1;
}

void loop()
{
  tick++;

  channel5 = pulseIn(ch5RADIO, HIGH);
  channel9 = pulseIn(ch9RADIO, HIGH);

  // Check for requested launch, then tell Arch
  if (channel5 > 1800)
  {
    digitalWrite(statusLEDPIN, HIGH);
    led = tick;
    digitalWrite(boneLaunchTriggeredPIN, HIGH);
    launchRec = true;
  }
  else
  {
    digitalWrite(boneLaunchTriggeredPIN, LOW);
    launchRec = false;
  }

  // If Arch calls for motor fire and a lanuch is requested, then fire motor
  if ((channel9 > 1800) && launchRec)
  {
    digitalWrite(statusLEDPIN, HIGH);
    led = tick;
    digitalWrite(pyroChannel, HIGH);
  }

  // If support pin pulled high, then move rocket supports
  //if (digitalRead(boneSupportPIN))
  //{
  //  leftServo.write(100);
  //  rightServo.write(100);
  // }

  if (led == (tick + 100))
  {
    led = 0;
    digitalWrite(statusLEDPIN, LOW);
  }

  if ((tick == 1000) && (led == 0))
  {
    tick = 0;
  }
}