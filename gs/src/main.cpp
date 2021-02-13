#include <Arduino.h>

static char buffer[8192];
static char* bp;
static bool pkt;
static bool send;

void setup()
{
    pkt = false;
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

    Serial.println("commandmode");
    delay(1000);
    Serial1.print("+++");
    delay(12000);
    Serial.println("done!");
}

void loop()
{
    //while (Serial1.available())
    if (Serial1.available())
    {
        digitalWrite(13, HIGH);
        Serial.println(Serial1.readString());
        //Serial1.println("hello world\n");
        delay(100);
        digitalWrite(13, LOW);
        delay(100);
        /**bp = Serial.read();
        ++bp;
        *bp = 0;

        if (!pkt)
        {
            if ((bp - buffer) == 4)
            {
                if (strcmp(buffer, "SOAR") == 0)
                {
                    pkt = true;
                    bp = buffer;
                    *bp = 0;
                }
            }
        }
        else
        {
            if ((bp - buffer) == 4)
            {
                if (strcmp(buffer, "SOAR") == 0)
                {
                    Serial.write(buffer, (bp - buffer));
                    pkt = false;
                    bp = buffer;
                    *bp = 0;
                    
                }
            }
        }*/
    }
    //else
    {
        digitalWrite(13, LOW);
    }
}