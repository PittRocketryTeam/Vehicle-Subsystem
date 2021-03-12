#include "MyXBee.hpp"
#include "Error.hpp"
#include <cstring>

MyXBee::MyXBee() :
    mode(-1),
    cycle(0)
{

}

MyXBee::~MyXBee() {}

void MyXBee::init()
{
    Serial.println("myxbee\n");
    memset(buffer, 0, 100);
    Serial2.begin(9600); //Serial2 is used for the PCB
    delay(500);
    int i;
    for (i = 0; i < CONN_ATTEMPTS; i++)
    {
        Error::on(TX_INIT);
        if (Serial2)
        {
            break;
        }

        Serial.println("fail");

        delay(CONN_DELAY);
    }
    Error::off();
    if (i == CONN_ATTEMPTS)
    {
        Error::display(TX_INIT, FATAL);
    }

    /*Serial.println("commandmode");
    delay(1000);
    Serial2.print("+++");
    delay(12000);
    Serial.println("done!");*/
}

void MyXBee::transmit(state* st)
{
    //digitalWrite(13, HIGH);
    
    int id = 5;
    int len = 0;
    // send periodic data
    if (cycle % 10 == 0) // send health data
    {
        len = 6 * sizeof(float);
        *(int*)(buffer + 0) = len;
        *(int*)(buffer + 4) = 4;
        *(float*)(buffer + 8) = st->vbat;
        *(float*)(buffer + 12) = st->ibat;
        *(float*)(buffer + 16) = st->tbat;
        *(float*)(buffer + 20) = st->temp;

        memcpy(buffer + 8, scratch, (len - 1) * sizeof(float));

    }
    else if ((cycle + 1) % 10 == 0) // send flight evets
    {

    }
    else // send orientation packet
    {
        len = 14 * sizeof(float);

        *(int*)(buffer) = len;
        *(int*)(buffer + 4) = 5;
        get_orientation(st, scratch);
        memcpy(buffer + 8, scratch, (len - 1) * sizeof(float));
    }

    // reset cycle counter
    ++cycle;
    if (cycle == 10)
    {
        cycle = 0;
    }

    Serial2.write('S');
    Serial2.write('B');
    Serial2.write('E');
    Serial2.write('G');
    Serial2.write(buffer, len + sizeof(float));
    Serial2.write('S');
    Serial2.write('F');
    Serial2.write('I');
    Serial2.write('N');
    //Serial2.write('\r');
    Serial2.flush();

    //digitalWrite(13, LOW);

    if (Serial2.available())
    {
        notify = Serial2.read();
    }
}

/*void MyXBee::setCachedData(Data newData)
{
    formattedData = "";

    formattedData += String(newData.timestamp) + ",";

    //IMU data
    formattedData += String(newData.imuData.euler_abs_orientation_x) + ",";
    formattedData += String(newData.imuData.euler_abs_orientation_y) + ",";
    formattedData += String(newData.imuData.euler_abs_orientation_z) + ",";
    formattedData += String(newData.imuData.acceleration_x) + ",";
    formattedData += String(newData.imuData.acceleration_y) + ",";
    formattedData += String(newData.imuData.acceleration_z) + ",";

    //GPS data
    formattedData += String(newData.gpsData.latitude) + ",";
    formattedData += String(newData.gpsData.longitude) + ",";
    //Altimeter data
    formattedData += String(newData.altimeterData.temperature) + ",";
    formattedData += String(newData.altimeterData.pressure) + ",";
    formattedData += String(newData.altimeterData.altitude) + ",";

    // Health data
    formattedData += String(newData.healthData.main_battery_voltage) + ",";
    formattedData += String(newData.healthData.reg_3V3_battery_voltage) + ",";
    formattedData += String(newData.healthData.reg_5V_battery_voltage) + ",";

    //formattedData += "";
}*/

/*(Data MyXBee::receive()
{
    if (Serial2.available())
    {
        mode = Serial2.read() == 'b';
        Serial.println(mode);
        delay(500);
        //Serial.println("\t\tyep");
    }
    else
    {
        ///Serial.println("nope");
    }
    

    Data d;
    return d;
}*/

int MyXBee::getModeFromGC()
{
    return mode;    
}

char MyXBee::checkForCommand()
{
    char out = notify;
    notify = '\0';
    return out;
}
