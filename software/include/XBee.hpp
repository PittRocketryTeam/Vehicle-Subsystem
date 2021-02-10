#ifndef __XBEE_HPP__
#define __XBEE_HPP__

#include "protocol.hpp"

#include <Arduino.h>

class XBee
{
    private:
        String formattedData;
        int mode;
        int cycle;
        unsigned char buffer[100];
        float scratch[100];

    public:
        XBee();
        ~XBee();
        void transmit(state*);
        void setCachedData();
        int getModeFromGC();
        void receive();
};

#endif
