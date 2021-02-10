#include <Arduino.h>
#include "protocol.hpp"
#include "GPS.hpp"
#include "XBee.h"
#include "IMU.hpp"
#include "Logger.hpp"
#include "Health.hpp"
#include "Altimeter.hpp"
#include "Event.hpp"

#define GPS_PR_SLOW 1'000'000
#define GPS_PR_FAST 1'000'000
#define TRX_PR_SLOW 2'000'000
#define TRX_PR_FAST 250'000
#define SNR_PR_SLOW 250'000
#define SNR_PR_FAST 1'000
#define LOG_PR_SLOW 4'000'000
#define LOG_PR_FAST 1'000'000

enum fcmode_t {FCIDLE = 0, FCSTARTUP = 1, FCFAIL = 2};

static Logger lgr;
static state st;
static XBee trx;
static GPS gps;
static IMU ag;
static Altimeter alt;
static Health hlt;
static fcmode_t mode = FCIDLE;
static Event evt;

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
    
    interrupts();
}

void snr_poll_callback()
{
    noInterrupts();

    // rolling averages
    st.alt_buf[st.alt_buf_pos++ % 5] = st.altitude;

    st.anet_buf[st.anet_buf_pos++ % 5] = anet(&st);

    // get new data
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
    mode = FCIDLE;
    transition = false;

    Error::init();
    serial_init();
    Serial.println("hello world");

    lgr.init();
    ag.init();
    alt.init();
    Serial.println("here");
    gps.init();
    hlt.init();
    Serial.println("ALL SENSORS READY");

    Error::summary();

    // set timers
    gps_int.priority(0);
    trx_int.priority(1);
    lgr_int.priority(2);
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
    //lgr_int.update(LOG_PR_SLOW);
    //trx_int.update(TRX_PR_SLOW);
    //snr_int.update(SNR_PR_SLOW);
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
    //lgr_int.update(LOG_PR_FAST);
    //trx_int.update(TRX_PR_FAST);
    //snr_int.update(SNR_PR_FAST);
}

void startup()
{
    noInterrupts();
    lgr.write(&st);
    interrupts();

    evt.infer(&st);
}

void loop()
{   
    if (transition)
    {
        transition = false;
        switch (mode)
        {
            case FCIDLE:
            idle_transition();
            break;

            case FCSTARTUP:
            startup_transition();
            break;

            case FCFAIL:
            break;
        }
        return;
    }

    switch (mode)
    {
        case IDLE:
        idle();
        break;

        case FCSTARTUP:
        startup();
        break;

        case FCFAIL:
        break;
    }
}
