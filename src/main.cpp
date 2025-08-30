/*
Super Robo Time 2020 Robot Code

Install the libraries listed below and add the esp32 board profiles from this link:
  https://dl.espressif.com/dl/package_esp32_index.json
  (paste into the additional board managers spot in File > Preferences, then go to
    Tools > Board > Boards Manager... and search for esp32)

Designed for use on the NodeMCU-32S

Author: Dustin Richards <dustin.richards@mines.sdsmt.edu>
Contributors:
  Heath Buer, fixed a very annoying crash by finding that running the motor driver
    on pins TX0 and RX0 == bad time
  Josiah Huntington, removed claw controls and added code for a kicker
    pin 13 is the lucky pin

This code has no copyright license, do whatever you want with it
*/

#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Arduino.h>
#include <DabbleESP32.h> // https://github.com/STEMpedia/DabbleESP32
#include <L289N.h>       // https://github.com/sdsmt-robotics/L298N
#include <batterySense.h>// https://github.com/sdsmt-robotics/srt2020-battery-sense
#include <analogWrite.h> // https://github.com/ERROPiX/ESP32_AnalogWrite
     // https://github.com//
#include <ESP32Servo.h>       // https://github.com/RoboticsBrno/ServoESP32

#define LED_BUILTIN 2

#include "kicker.h"

//motor driver setup
L289N rMotor(23, 22, 21, true);
L289N lMotor(19, 18, 5,  true);
int lVel, rVel;


//status LED!
const int BLINK_PERIOD = 200; //ms between blinks
bool ledState = 0;
uint32_t prevTimeLED = 0;

//battery voltage sensor
SRTBatterySense battery(A0, A3, A6);
void stopRobot();
const int calibrationBridgePin = 35;
const int calibrationBridgePin2 = 32;

//Kicker
SRTKicker kicker(13);

//infrared line sensor

void setup()
{
  Serial.begin(115200);
  Dabble.begin("Dean M"); //change the name inside the quotes, this will appear in your Bluetooth menu
  
  //analogWriteFrequency(2000);
  lMotor.init();
  rMotor.init();
  battery.init();
  kicker.init();

  pinMode(LED_BUILTIN, OUTPUT);


  //
  pinMode(calibrationBridgePin2, OUTPUT);
  digitalWrite(calibrationBridgePin2, HIGH); 
  pinMode(calibrationBridgePin, INPUT);
  if (digitalRead(calibrationBridgePin))
  {
    battery.calibrate();
  }
}

void loop() {

  
  //stop the bot if the battery is low
  if (battery.getRollingAverage() < 7)
  {
    stopRobot(); 
  }

  //sample the ultrasonic sensor, this needs to run very frequently
  Dabble.processInput();

  
  /*********************
   * Use 
   *  if( GamePad.isTriaglePressed() ) 
   * or 
   *  if( GamePad.isCirclePressed() )
   * to add more functions for the kicker
   */
  //Turn the kicker on
  if(GamePad.isSquarePressed())
  {
    kicker.kickerOn();
    Serial.println("KickerOn");
  }
  //Turn the kicker off
  if (GamePad.isCrossPressed())
  {
    kicker.kickerOff();   
    Serial.println("kickerOff");
  }
  
  /*
  float xRaw = GamePad.getXaxisData();
  float yRaw = GamePad.getYaxisData();
  float xBias = -abs(xRaw) / 7 + 1;
  int yMap = sqrt(pow(xRaw, 2) + pow(yRaw, 2));
  yMap = map(yMap, 0, 7, 0, 255);
  */

  double Left = (GamePad.getYaxisData() + (GamePad.getXaxisData()));
  double Right = (GamePad.getYaxisData() - (GamePad.getXaxisData()));
  Left = map(Left, -7, 7, -254, 254);
  Right = map(Right, -7, 7, -254, 254);

  if(abs(GamePad.getXaxisData()) > 0.1 || abs(GamePad.getYaxisData()) > 0.1)
  {
    //set the motor speeds
    lMotor.setSpeedDirection(Left, false);
    rMotor.setSpeedDirection(Right, false);
  }
  else
  {
    lMotor.setSpeedDirection(0, false);
    rMotor.setSpeedDirection(0, false);
  }

  //handle blinking the ESP32's built-in LED
  if (millis() > prevTimeLED + BLINK_PERIOD)
  {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    prevTimeLED = millis();
  }

  static unsigned long printTime = millis();
}

void stopRobot()
{
  lMotor.setSpeedDirection(0);
  rMotor.setSpeedDirection(0);

}
 