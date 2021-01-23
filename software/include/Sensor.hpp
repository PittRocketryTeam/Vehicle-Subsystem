#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__

#include <vector>
#include "Wire.h"
#include "protocol.hpp"

class Sensor
{
public:

    virtual void init()
    {

    }

    virtual void poll(state* st)
    {
        // ask the sensor for new data
        // library calls to communicate with the sensor are made here
        // 
    }

    virtual void read(state* st)
    {

    }

    virtual void enable() = 0;
    virtual void disable() = 0;
};

#endif
// __SENSOR_HPP__