#ifndef GLOBALS_H
#define GLOBALS_H

#include <ESP32Servo.h>

#include "servo_control.h"
#include "navi_control.h"

extern Servo servoH;
extern Servo servoV;

extern ServoControl servoControl;
extern NaviControl naviControl;

#define PIN_SERVO_H 14
#define PIN_SERVO_V 15
#define PIN_NAVI_L 12
#define PIN_NAVI_R 13
#define PIN_DIR_L 2
#define PIN_DIR_R 4 // Flashlight
#define PIN_FLASH 4
#define PIN_LED_BUILTIN 33

#define PWM_CHANNEL_L 5
#define PWM_CHANNEL_R 6

#endif