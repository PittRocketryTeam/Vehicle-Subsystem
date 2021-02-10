#ifndef EVENT_HPP
#define EVENT_HPP

#include "protocol.hpp"

typedef enum {IDLE = 0, LAUNCH = 1, BURNOUT = 2, APOGEE = 3, DROGUE = 4, MAIN = 5, PREDEP = 6, DEPLOY = 7, TOUCHDOWN = 10} event_t;

class Event
{
public:
    Event();
    ~Event();

    void infer(state*);
    event_t getCurrentEvent();

private:
    event_t m_cur;
};

#endif /* EVENT_HPP */