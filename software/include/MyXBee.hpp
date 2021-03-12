#ifndef __MYXBEE_HPP__
#define __MYXBEE_HPP__

#include "protocol.hpp"

#include <Arduino.h>

class MyXBee
{
    private:
        String formattedData;
        int mode;
        int cycle;
        unsigned char buffer[100];
        float scratch[100];
        char notify;

    public:
        MyXBee();
        ~MyXBee();
        void init();
        void transmit(state*);
        void setCachedData();
        int getModeFromGC();
        void receive();
        char checkForCommand();
};

#endif
