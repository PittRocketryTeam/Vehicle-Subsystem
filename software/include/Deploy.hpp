#ifndef DEPLOY_HPP
#define DEPLOY_HPP

#include "protocol.hpp"
#include "Sensor.hpp"

#include <cstdint>

#define ROLLING_BUFFER_SIZE 30
#define DEPLOY_ALT 200.0f
#define DEPLOY_SIGNAL_TIME 2000
#define LAUNCH_ACCEL_THR 30.f

class Deploy : public Sensor
{
public:

    Deploy();
    virtual ~Deploy();

    virtual void init();
    virtual void poll(state*);
    virtual void read(state*);

private:

    uint8_t inflight;
    uint8_t deployed;
    float altbuf[ROLLING_BUFFER_SIZE];
    float accbuf[ROLLING_BUFFER_SIZE];
    float* altbuf_p;
    float* accbuf_p;
    uint32_t elapsed;
    uint32_t last;
};

#endif /* DEPLOY_HPP */