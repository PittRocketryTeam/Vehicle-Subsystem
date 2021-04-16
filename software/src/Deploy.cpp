#include "Deploy.hpp"
#include "config.hpp"

#include <Arduino.h>
#include <cstring>

Deploy::Deploy() :
    Sensor(),
    inflight(0),
    deployed(0),
    elapsed(0)
{

}

Deploy::~Deploy()
{

}

void Deploy::init()
{
    for (int i = 0; i < ROLLING_BUFFER_SIZE; ++i)
    {
        altbuf[i] = 0.f;
    }

    for (int i = 0; i < ROLLING_BUFFER_SIZE; ++i)
    {
        accbuf[i] = 0.f;
    }

    accbuf_p = accbuf;
    altbuf_p = altbuf;
}

void Deploy::read(state* st)
{
    st->payload_dep = deployed;
    st->payload_pre = inflight;
}

void Deploy::poll(state* st)
{
    *altbuf_p = st->altitude;
    *accbuf_p = anet(st);

    // move head
    ++altbuf_p;
    if (altbuf_p - altbuf >= ROLLING_BUFFER_SIZE)
    {
        altbuf_p = altbuf;
    }

    ++accbuf_p;
    if (accbuf_p - accbuf >= ROLLING_BUFFER_SIZE)
    {
        accbuf_p = accbuf;
    }

    // compute avg
    float alt_avg = 0;
    for (int i = 0; i < ROLLING_BUFFER_SIZE; ++i)
    {
        alt_avg += altbuf[i];
    }
    alt_avg /= (float)ROLLING_BUFFER_SIZE;

    float acc_avg = 0;
    for (int i = 0; i < ROLLING_BUFFER_SIZE; ++i)
    {
        acc_avg += accbuf[i];
    }
    acc_avg /= (float)ROLLING_BUFFER_SIZE;

    if (inflight)
    {
        if (!deployed)
        {
            if (alt_avg < DEPLOY_ALT)
            {
                deployed = 1;
                last = st->ts;
                digitalWrite(DEPLOY_SIGNAL_WIRE, HIGH);
            }
        }
        else if (elapsed < DEPLOY_SIGNAL_TIME)
        {
            elapsed += st->ts - last;
            last = st->ts;

            if (elapsed > DEPLOY_SIGNAL_TIME)
            {
                digitalWrite(DEPLOY_SIGNAL_WIRE, LOW);
            }
        }
    }
    else
    {
        if (acc_avg > LAUNCH_ACCEL_THR)
        {
            inflight = 1;
        }
    }

    read(st); // update state
}