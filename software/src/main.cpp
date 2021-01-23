#include <Arduino.h>
#include "protocol.hpp"
#include "GPS.hpp"
#include "XBee.h"
#include "IMU.hpp"
#include "Logger.hpp"
#include "Altimeter.hpp"

enum fcmode_t {IDLE = 0, STARTUP = 1, FAIL = 2};

static Logger lgr;
static state st;
static XBee tx;
static GPS gps;
static IMU ag;
static Altimeter alt;
static fcmode_t mode;
static IntervalTimer gps_int;
static bool transition;

void serial_init()
{
    Serial.begin(9600);
    int i;
    for (i = 0; i < CONN_ATTEMPTS; ++i)
    {
        if (Serial)
        {
            break;
        }

        digitalWrite(13, HIGH);
        Error::on(SERIAL_INIT);
        delay(CONN_DELAY);
        digitalWrite(13, LOW);
        delay(CONN_DELAY);
    }
    Error::off();
    if (i >= CONN_ATTEMPTS)
    {
        Error::display(SERIAL_INIT, FATAL);
    }
}

void gps_read_callback()
{
    gps.internal_read();
}

void setup()
{
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);

    memset(&st, 0, sizeof(state));      // zero system state
    mode = IDLE;
    transition = false;

    Error::init();
    serial_init();

    lgr.init();
    //lgr.flush();

    ag.init();
    alt.init();

    gps.init();
    gps_int.begin(gps_read_callback, 1000);

    // init logger

    Error::summary();

    // set timers
}

void idle_transition()
{

}

void idle()
{

}

void startup_transition()
{

}

void startup()
{

}

void loop()
{   
    digitalWrite(13, HIGH);
    if (transition)
    {
        transition = false;
        switch (mode)
        {
            case IDLE:
            idle_transition();
            break;

            case STARTUP:
            startup_transition();
            break;

            case FAIL:
            break;
        }
        return;
    }

    switch (mode)
    {
        case IDLE:
        idle();
        break;

        case STARTUP:
        startup();
        break;

        case FAIL:
        break;
    }

    digitalWrite(13, LOW);
}
