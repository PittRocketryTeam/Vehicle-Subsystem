#ifndef __XBEE_HPP__
#define __XBEE_HPP__

#include <Arduino.h>

class XBee
{
    private:
        String formattedData;
        int mode;

    public:
        XBee();
        ~XBee();
        void transmit();
        void setCachedData();
        int getModeFromGC();
        void receive();
};

#endif
