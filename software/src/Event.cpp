#include "Event.hpp"
#include "protocol.hpp"
#include <cmath>

static float last_avg_vy;

Event::Event() :
    m_cur(LAUNCH)
{

}

Event::~Event()
{
    
}

void Event::infer(state* st)
{
    if (m_cur == IDLE)
    {
        if (avg_anet(st) > 80.f)
        {
            m_cur = LAUNCH;
            st->launch_detect = 1;
        }
    }
    else if (m_cur == LAUNCH)
    {
        if (avg_vy(st) < 5)
        {
            m_cur = APOGEE;
        }
    }
    else if (m_cur == APOGEE)
    {
        if (avg_vy(st) < 0)
        {
            m_cur = DROGUE;
            last_avg_vy = avg_vy(st);
        }
    }
    else if (m_cur == DROGUE)
    {
        if (avg_vy(st) - last_avg_vy < 5.f)
        {
            m_cur = MAIN;
        }
        last_avg_vy = avg_vy(st);
    }
    else if (m_cur == MAIN)
    {
        if (fabs(avg_vy(st)) < 1)
        {
            m_cur = TOUCHDOWN;
        }
    }
}

event_t Event::getCurrentEvent()
{
    return m_cur;
}