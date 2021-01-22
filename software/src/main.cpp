#include <Arduino.h>
#include "protocol.hpp"

typedef enum {IDLE = 0, STARTUP = 1, FAIL = 2} mode_t;

static state st;
static XBee tx;
static GPS gps;
static IMU imu;
static Altimeter alt;
static mode_t mode;
static IntervalTimer gps_int;
static bool transition;

void serial_init()
{
    Serial.begin(9600);
    for (int i = 0; i < CONN_ATTEMPTS; ++i)
    {
        if (Serial)
        {
            break;
        }

        Error::on(SERIAL_INIT);
        delay(CONN_DELAY * 3);
    }
    Error::off();
    if (i >= CONN_ATTEMPTS)
    {
        Error::display(SERIAL_INIT, FATAL);
    }
}

int setup()
{
    memset(&st, 0, sizeof(state));      // zero system state
    mode = IDLE;
    transition = false;

    Error::init();
    serial_init();

    imu.init();
    alt.init();

    gps.init();
    gps_int.begin(gps_read_callback, 1000);

    // init logger

    Error::summary();

    // set timers
}

void gps_read_callback()
{
    gps.internal_read();
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

int loop()
{   
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
}
