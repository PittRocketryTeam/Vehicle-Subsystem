#include <Arduino.h>

static char buffer[8192];
static char* bp;
static bool pkt;
static bool send;
static char peek[5];
static char* peek_p;

void setup()
{
    pkt = false;
    peek_p = peek;
    send = false;
    bp = buffer;
    *bp = 0;

    pinMode(13, OUTPUT);

    Serial.begin(9600);
    while (!Serial)
    {
        digitalWrite(13, HIGH);
        delay(100);
        digitalWrite(13, LOW);
        delay(100);
    }
    Serial.println("hello world\n");
    delay(1000);
    Serial1.begin(9600);
    while (!Serial1)
    {
        Serial.println("wait for xbee");
        digitalWrite(13, HIGH);
        delay(5000);
        digitalWrite(13, LOW);
        delay(100);
    }
    delay(1000);
}

void loop()
{
    char e = '\n';
    while (Serial1.available())
    {
        int ch = Serial1.read();
        *bp = ch;
        ++bp;
        *bp = 0;
        
        if (!pkt)
        {
            if ((bp - buffer) >= 4 && !strncmp(bp - 4, "SBEG", 4))
            {
                //Serial.println("begin");
                pkt = true;
                bp = buffer;
            }
        }
        else
        {
            if ((bp - buffer) >= 4 && !strncmp(bp - 4, "SFIN", 4))
            {
                //Serial.println("end");
                int len = ((bp - 4) - buffer);
                //Serial.println(len);
                Serial.write(buffer, len);
                Serial.write("\r\n");
                pkt = false;
                bp = buffer;
            }
        }
    }
    //else
    {
        digitalWrite(13, LOW);
    }

    while (Serial.available())
    {
        Serial.write(Serial.read());
    }
}
