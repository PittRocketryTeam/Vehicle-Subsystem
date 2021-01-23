#ifndef __IMU_HPP__
#define __IMU_HPP__

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "Sensor.hpp"

#define IMU_DIMENIONS 3

class IMU : public Sensor
{
    private:
        Adafruit_BNO055 sensor;
        sensors_event_t event;
        bool verbose;
        //Data *last_data;
        //Data last_data;

        float ax, ay, az;
        float ox, oy, oz;
        imu::Vector<3> o;
        imu::Vector<3> a;

    public:

        IMU();
        ~IMU();

        virtual void init();
        virtual void poll(state*);
        virtual void read(state*);

        virtual void enable();
        virtual void disable();
};

#endif
// __IMU_HPP__