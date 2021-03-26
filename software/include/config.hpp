#ifndef PINS_HPP
#define PINS_HPP

#include <Arduino.h>

// Teensy Board Version
//#define TEENSY_36
#define TEENSY_41

// Options
#define DEBUG 1
#define VERBOSE 0

// Pin mapping

#define DEBUG_LED_1 41
#define DEBUG_LED_2 40
#define DEBUG_LED_3 39

#define THER A7
#define RDIV A8
#define HALL A9

#define GPS_ENABLE -1
#define GPS_SERIAL Serial1

#define ALT_I2C_ADDR 0x77
#define ALT_I2C_BUS Wire

#define IMU_I2C_ADDR 0x28
#define IMU_I2C_BUS Wire

#endif /* PINS_HPP */
