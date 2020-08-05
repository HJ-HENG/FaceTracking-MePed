// ==================================== Information ==============================================
//
//  Program to enable face tracking in mePed Robot (camera required)
//
//  The mePed is an open source quadruped robot designed by Scott Pierce of
//  Spierce Technologies (www.meped.io & www.spiercetech.com)
//
//  This program is written by Heng Hao Jun (hhj@hotmail.my)
//
// ================================== Libraries Used =============================================

#include <Servo.h>   

// ===================================== Globals =================================================

// Constants
#define NEUTRAL_SERVO_ANGLE 90
#define MAX_PIVOT_ANGLE 85
#define MAX_LIFT_DOWNWARDS_ANGLE 26
#define MAX_LIFT_UPWARDS_ANGLE 60
#define MAX_DISPLACEMENT_FROM_CENTER_OF_CAMERA 250

// Servos used
Servo servo1; // Front Left  Pivot Servo
Servo servo2; // Front Left  Lift  Servo
Servo servo3; // Back  Left  Pivot Servo
Servo servo4; // Back  Left  Lift  Servo
Servo servo5; // Back  Right Pivot Servo
Servo servo6; // Back  Right Lift  Servo
Servo servo7; // Front Right Pivot Servo
Servo servo8; // Front Right Lift  Servo

// ====================================== Setup ==================================================

void setup()
{
  // Attaches the servos to the Arduino Pins
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  servo5.attach(6);
  servo6.attach(7);
  servo7.attach(8);
  servo8.attach(9);
  // Sets the data rate (baud)
  Serial.begin (9600);
}//setup

// ======================================= Loop ===================================================

void loop()
{  
  reset_servo_positions(); 
  // Repositions the meped based on the serial data being read
  while (true) {
     if(Serial.available() > 0)
     {
        if(Serial.read() == 'X')
        {
          int x_displacement = Serial.parseInt();
          if(Serial.read() == 'Y')
          {
            int y_displacement = Serial.parseInt(); 
            reposition_meped(x_displacement, y_displacement);
          }
        }
        // Helps to avoid overloading the arduino with too many instructions
        while(Serial.available() > 0)
        {
          Serial.read();
        }
        delay(200);
     }
  }
}//loop

// ============================ Repositions MePed =============================

void reposition_meped(int x, int y) {
  if (x != 0) {
    turn(x);
  }
  if (y != 0) {
    tilt(y);
  }
}

// ================== Tilts camera using MePed's lift servos ===================

void tilt(int y) {
  int frontLiftAngle;
  int backLiftAngle;
  if (y > 0) {
    frontLiftAngle = scale_to_servo_degrees(y, MAX_LIFT_UPWARDS_ANGLE);
    backLiftAngle = scale_to_servo_degrees(y, MAX_LIFT_DOWNWARDS_ANGLE);
  } else {
    frontLiftAngle = scale_to_servo_degrees(y, MAX_LIFT_DOWNWARDS_ANGLE);
    backLiftAngle = scale_to_servo_degrees(y, MAX_LIFT_UPWARDS_ANGLE);
  }
  servo2.write(NEUTRAL_SERVO_ANGLE - frontLiftAngle);
  servo4.write(NEUTRAL_SERVO_ANGLE + backLiftAngle);
  servo6.write(NEUTRAL_SERVO_ANGLE + backLiftAngle);
  servo8.write(NEUTRAL_SERVO_ANGLE - frontLiftAngle);
  delay(300);
}

// ================= Pans camera using MePed's lift and pivot servos =============

void turn(int x) {
  x = scale_to_servo_degrees(x, MAX_PIVOT_ANGLE);
  servo1.write(NEUTRAL_SERVO_ANGLE + x);
  servo3.write(NEUTRAL_SERVO_ANGLE + x);
  servo5.write(NEUTRAL_SERVO_ANGLE + x);
  servo7.write(NEUTRAL_SERVO_ANGLE + x);
  delay(300);
  // Comment out the line below if camera does not rotate together with meped
//  reset_lift_servo_positions();   
}

// ======== Scales displacement data to match servo's rotational angle =========

int scale_to_servo_degrees(int value, int multiplier) {
  return value * multiplier / MAX_DISPLACEMENT_FROM_CENTER_OF_CAMERA;
}

// =========================== Reset Servos Positions =========================== 

void reset_servo_positions()
{
  servo1.write(NEUTRAL_SERVO_ANGLE);
  servo2.write(NEUTRAL_SERVO_ANGLE);
  servo3.write(NEUTRAL_SERVO_ANGLE);
  servo4.write(NEUTRAL_SERVO_ANGLE);
  servo5.write(NEUTRAL_SERVO_ANGLE);
  servo6.write(NEUTRAL_SERVO_ANGLE);
  servo7.write(NEUTRAL_SERVO_ANGLE);
  servo8.write(NEUTRAL_SERVO_ANGLE);
}

// ================ Used to turn the body of the MePed Robot ==================== 

void reset_lift_servo_positions() 
{
  servo2.write(NEUTRAL_SERVO_ANGLE);
  servo4.write(NEUTRAL_SERVO_ANGLE);
  servo6.write(NEUTRAL_SERVO_ANGLE);
  servo8.write(NEUTRAL_SERVO_ANGLE);
  delay(200);
  servo2.write(MAX_LIFT_UPWARDS_ANGLE);
  delay(100);
  servo1.write(NEUTRAL_SERVO_ANGLE);
  servo2.write(NEUTRAL_SERVO_ANGLE);
  delay(200);
  servo4.write(MAX_LIFT_UPWARDS_ANGLE);
  delay(100);
  servo3.write(NEUTRAL_SERVO_ANGLE);
  servo4.write(NEUTRAL_SERVO_ANGLE);
  delay(200);
  servo6.write(MAX_LIFT_UPWARDS_ANGLE);
  delay(100);
  servo5.write(NEUTRAL_SERVO_ANGLE);
  servo6.write(NEUTRAL_SERVO_ANGLE);
  delay(200);
  servo8.write(MAX_LIFT_UPWARDS_ANGLE);
  delay(100);
  servo7.write(NEUTRAL_SERVO_ANGLE);
  servo8.write(NEUTRAL_SERVO_ANGLE);
  delay(200);
}
