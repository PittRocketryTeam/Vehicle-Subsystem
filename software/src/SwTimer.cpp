#include "SwTimer.hpp"

#include <Arduino.h>

SwTimer::SwTimer() :
    callback(nullptr),
    interval(0),
    now(0),
    then(0),
    delta(0),
    acc(0)
{
    
}

SwTimer::~SwTimer()
{

}

void SwTimer::begin(void (*cb)(void), int val)
{
    callback = cb;
    interval = val;
    now = millis();
    then = now;
}

void SwTimer::update(int nv)
{
    interval = nv;
}

void SwTimer::tick()
{
    now = millis();
    delta = now - then;
    then = now;

    acc += delta;
    if (acc > interval)
    {
        callback();
        acc = 0;
    }    
}