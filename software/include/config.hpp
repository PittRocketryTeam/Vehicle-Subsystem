#ifndef PINS_HPP
#define PINS_HPP

#include <Arduino.h>

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

#define IMU_ADDR 0x00 // TODO

#endif /* PINS_HPP */