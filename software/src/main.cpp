#include <Arduino.h>
#include "protocol.hpp"
#include "GPS.hpp"
#include "XBee.h"
#include "IMU.hpp"
#include "Logger.hpp"
#include "Health.hpp"
#include "Altimeter.hpp"

#define GPS_PR_SLOW 1'000'000
#define GPS_PR_FAST 1'000'000
#define TRX_PR_SLOW 2'000'000
#define TRX_PR_FAST 250'000
#define SNR_PR_SLOW 250'000
#define SNR_PR_FAST 1'000
#define LOG_PR_SLOW 4'000'000
#define LOG_PR_FAST 1'000'000

enum fcmode_t {IDLE = 0, STARTUP = 1, FAIL = 2};

static Logger lgr;
static state st;
static XBee trx;
static GPS gps;
static IMU ag;
static Altimeter alt;
static Health hlt;
static fcmode_t mode = IDLE;

static IntervalTimer gps_int;
static IntervalTimer lgr_int;
static IntervalTimer trx_int;
static IntervalTimer snr_int;

static bool transition = false;

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
    noInterrupts();
    gps.internal_read();
    interrupts();
}

void lgr_flush_callback()
{
    noInterrupts();
    lgr.flush();
    interrupts();
}

void trx_send_callback()
{
    noInterrupts();
    //todo
    interrupts();
}

void snr_poll_callback()
{
    noInterrupts();
    st.ts = millis();
    //ag.poll(&st);
    alt.poll(&st);
    //hlt.poll(&st);
    gps.poll(&st);
    interrupts();
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
    Serial.println("hello world");

    //lgr.init();
    //ag.init();
    alt.init();
    Serial.println("here");
    gps.init();
    hlt.init();
    Serial.println("ALL SENSORS READY");

    Error::summary();

    // set timers
    gps_int.priority(0);
    trx_int.priority(1);
    //lgr_int.priority(2);
    snr_int.priority(3);

    gps_int.begin(gps_read_callback, GPS_PR_SLOW);
    lgr_int.begin(lgr_flush_callback, LOG_PR_SLOW);
    //trx_int.begin(trx_send_callback, TRX_PR_SLOW);
    snr_int.begin(snr_poll_callback, SNR_PR_SLOW);

    printf("init done");
}

void idle_transition()
{
    gps_int.update(GPS_PR_SLOW);
    //lgr_int.begin(lgr_flush_callback, LOG_PR_SLOW);
    //trx_int.begin(trx_send_callback, TRX_PR_SLOW);
    //snr_int.begin(snr_poll_callback, SNR_PR_SLOW);
}

void idle()
{
    noInterrupts();
    lgr.write(&st);
    interrupts();
}

void startup_transition()
{
    gps_int.update(GPS_PR_FAST);
    //lgr_int.begin(lgr_flush_callback, LOG_PR_FAST);
    //trx_int.begin(trx_send_callback, TRX_PR_FAST);
    //snr_int.begin(snr_poll_callback, SNR_PR_FAST);
}

void startup()
{
    
}

void loop()
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
