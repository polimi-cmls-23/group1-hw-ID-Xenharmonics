// # Editor    :Holiday from DFRobot
// # Data      09.10.2013

// # Product name:Triple Axis Accelerometer MMA7361
// # Product SKU:SEN0143
// # Version :  2.0

// # Description:
// # This sample shows how to measure the angle value using two axis value


#include<math.h>
#include<stdio.h>

#define A_X  5
#define A_Y  4
#define A_Z  3

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

const int xpin = A2;                  // x-axis of the accelerometer

const int ypin = A1;                  // y-axis

const int zpin = A0;                  // z-axis

int val_x,val_y,val_z;
double b;
void setup()
{
//  pinMode(A_X,INPUT);
//  pinMode(A_Y,INPUT);
//  pinMode(A_Z,INPUT);
  Serial.begin(9600);
}
void loop()
{
 // print the sensor values:

 // Convert raw values to acceleration values

//    float xAcc = analogRead(xpin);
//    float yAcc = analogRead(ypin);
//    float zAcc = analogRead(zpin);
  
  float xAcc = mapf(analogRead(xpin), 0, 1023, -1.0, 1.0); 
  float yAcc = mapf(analogRead(ypin), 0, 1023, 0.0f, 5.0f); 
  float zAcc = mapf(analogRead(zpin), 0, 1023, 0.0, 1.0); 

//  float xAcc = analogRead(xpin);
//  float yAcc = analogRead(ypin);
//  float zAcc = analogRead(zpin);


  Serial.print(xAcc);

  // print a tab between values:

  Serial.print(" ");

  Serial.print(yAcc);

  // print a tab between values:

  Serial.print(" ");

  Serial.print(zAcc);

  Serial.println();

  // delay before next reading:

  delay(200);

}
