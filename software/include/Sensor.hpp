#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

#include <vector>
#include "Wire.h"
#include "Data.hpp"

class Sensor
{
public:

    void init()
    {

    }

    void poll(state* st)
    {
        // ask the sensor for new data
        // library calls to communicate with the sensor are made here
        // 
    }

    void read(state* st)
    {

    }
};

#endif
// __SENSOR_HPP__