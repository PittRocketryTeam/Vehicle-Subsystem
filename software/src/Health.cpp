#include "Health.hpp"
#include "config.hpp"
#include <Arduino.h>

#define MIN_CURRENT 2.50f

float calculate_voltage(int raw, float r1, float r2)
{
    float vout = 3.3 * ((float)raw / (float)ANALOG_MAX);
    return (vout * (float)(r1 + r2)) / (float)r2;
}


float calculate_temperature(int raw, float beta)
{
    //float k = 1.0 / ((1.0 / THERMISTOR_T0) + (1.0 / beta) * (log((float)ANALOG_MAX / (float)raw) - 1.0));
    float pv = 3.3f * ((float)(raw) / 4096.f);
    float r = 10'000.f * (3.3f - pv) / pv;
    float temp = 1.f/((log(r / 2'000.f)) / beta + (1.0 / 298.f)) - 273.15;
    return temp;
}


static float calculate_current(int raw)
{
    return ((((float)raw / ANALOG_MAX) * 3.3f) - MIN_CURRENT) * 10'000.f;
}

Health::Health() :
    Sensor(),
    battery_temperature(-1),
    battery_voltage(-1),
    reg5_voltage(-1),
    reg3_voltage(-1),
    enabled(true)
{

}

Health::~Health()
{

}

void Health::init()
{
    analogReadResolution(12);
    //analogReference(0);
    pinMode(THER, INPUT);
    pinMode(RDIV, INPUT);
    pinMode(HALL, INPUT);
}


void Health::read(state* st)
{
    if (!enabled)
    {
        return;
    }

    st->vbat = battery_voltage;
    st->ibat = battery_current;
    st->tbat = battery_temperature;

    /*data.healthData.main_battery_temperature = battery_temperature;
    data.healthData.main_battery_voltage = battery_voltage;
    data.healthData.reg_5V_battery_voltage = reg5_voltage;
    data.healthData.reg_3V3_battery_voltage = reg3_voltage;*/
}

void Health::poll(state* st)
{
    int battery_t_raw = analogRead(THER);
    int battery_v_raw = analogRead(RDIV);
    int cur_raw = analogRead(HALL);

    //Serial.printf("%d,%d,%d\n", battery_t_raw, battery_v_raw, cur_raw);

    (void)battery_t_raw; // squash pesky warnings

    st->vbat = calculate_voltage(battery_v_raw, 20e3, 10e3);
    st->tbat = calculate_temperature(battery_t_raw, 3539);
    st->ibat = calculate_current(cur_raw);

    Serial.printf("%f,%f,%f\n", st->vbat, st->tbat, st->ibat);
    //battery_temperature = calculate_temperature(battery_t_raw, THERMISTOR_BETA);
    //battery_voltage = calculate_voltage(battery_v_raw, MOHM, .470 * MOHM);
    //battery_current = calculate_current(cur_raw);
    //reg5_voltage = calculate_voltage(reg5_v_raw, MOHM, MOHM);
    //reg3_voltage = calculate_voltage(reg3_v_raw, MOHM, MOHM);


    read(st);
}

void Health::enable()
{
    enabled = true;
}

void Health::disable()
{
    enabled = false;
}