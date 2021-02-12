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
    memset(buffer, 0, 100);
    Serial4.begin(9600); //Serial4 is used for the PCB
    int i;
    for (i = 0; i < CONN_ATTEMPTS; i++)
    {
        //Error::on(TX_INIT);
        if (Serial4.available())
        {
            break;
        }

        delay(CONN_DELAY);
    }
    if (i == CONN_ATTEMPTS)
    {
        //Error::display(TX_INIT, FATAL);
    }
}

void MyXBee::transmit(state* st)
{
    int id;
    int len;
    // send periodic data
    if (cycle % 10 == 0) // send health data
    {

    }
    else if ((cycle + 1) % 10 == 0) // send flight evets
    {

    }
    else // send orientation packet
    {
        len = 14 * sizeof(float);

        *(int*)(buffer) = len;
        *(int*)(buffer + 4) = id;
        get_orientation(st, scratch);
        memcpy(buffer + 8, scratch, 12 * sizeof(float));
    }

    // reset cycle counter
    ++cycle;
    if (cycle == 10)
    {
        cycle = 0;
    }

    Serial4.write(buffer, len + 4);
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
    if (Serial4.available())
    {
        mode = Serial4.read() == 'b';
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