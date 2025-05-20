#include <Arduino.h>
#include <Wire.h>
#include <AFMotor.h>

void setSpeed(AF_DCMotor, int, bool);
void drive(double, double);
int clamp(int, int, int);

AF_DCMotor leftMotor = AF_DCMotor(1);
AF_DCMotor rightMotor = AF_DCMotor(2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char keyboardInput = Serial.read();
    switch (keyboardInput)
    {
    case 'w':
      drive(0.2, 0);
      break;
    case 'a':
      drive(0, 0.2);
      break;
    case 's':
      drive(-0.2, 0);
      break;
    case 'd':
      drive(0, -0.2);
      break;
    default:
      drive(0,0);
      break;
    }
  }
}

// positive linearSpeed [-1, 1] means forward
// positive angularSpeed [-1, 1] means ccw rotation
void drive(double linearSpeed, double angularSpeed) {
  int leftSpeed = (int)(255 * linearSpeed - 255 * angularSpeed);
  leftSpeed = clamp(leftSpeed, -255, 255);
  int rightSpeed = (int)(255 * linearSpeed + 255 * angularSpeed);
  rightSpeed = clamp(rightSpeed, -255, 255);

  setSpeed(leftMotor, leftSpeed, true);
  setSpeed(rightMotor, rightSpeed, false);
}

void setSpeed(AF_DCMotor motor, int speed, bool isReversed) {
  motor.setSpeed(abs(speed) & 0xFF);
  if (speed > 0) {
    motor.run(isReversed ? FORWARD : BACKWARD);
  } else if (speed < 0) {
    motor.run(isReversed ? BACKWARD : FORWARD);
  }
  else {
    motor.run(BRAKE);
  }
}

int clamp(int value, int minValue, int maxValue) {
  return max(min(value, maxValue), minValue);
}