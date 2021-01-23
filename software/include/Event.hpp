#ifndef EVENT_HPP
#define EVENT_HPP

#include "protocol.hpp"

typedef enum {LAUNCH = 0, BURNOUT = 1, DROUGE = 2, MAIN = 3, PREDEP = 4, DEPLOY = 5, TOUCHDOWN = 6} event_t;

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