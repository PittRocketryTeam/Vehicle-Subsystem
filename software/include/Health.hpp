#ifndef __HEALTH_HPP__
#define __HEALTH_HPP__

#define BATT_T A3
#define BATT_V A1
#define REG5_V A8
#define REG3_V A7
#define MOHM 1e6
#define KOHM 1e3
#define THERMISTOR_T0 298.15
#define THERMISTOR_BETA 3380.0
#define ANALOG_MAX 1023

#include "Sensor.hpp"
#include "protocol.hpp"

class Health : public Sensor
{
    public:

        Health();
        ~Health();
            
        virtual void init();
        virtual void read(state*);
        virtual void poll(state*);
        virtual void enable();
        virtual void disable();

    private:
        float calculate_voltage(int, float, float);
        float calculate_temperature(int, float);

        float battery_temperature;
        float battery_voltage;
        float reg5_voltage;
        float reg3_voltage;
        
        bool enabled;
};

#endif /* __HEALTH_HPP__ */

