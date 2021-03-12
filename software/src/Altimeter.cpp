#include "Altimeter.hpp"

Altimeter::Altimeter() :    
    Sensor(),
    baselinePressure(SEALEVELPRESSURE_HPA)
{

}

Altimeter::~Altimeter()
{

}

void Altimeter::init()
{
    int i;
    for (i=0; i < CONN_ATTEMPTS; i++)
    {
        Error::on(ALT_INIT);
        Serial.println("try");
        if (bmp.begin(0x77, &Wire))
        {
            break;
        }

        delay(CONN_DELAY);
    }
    Error::off();
    if (i > CONN_ATTEMPTS)
    {
        Serial.println("alt bad");
        Error::display(ALT_INIT, FATAL);
    }
}

void Altimeter::read(state* st)
{
    /*
    data.altimeterData.temperature = bmp.temperature;
    data.altimeterData.pressure = bmp.pressure;

    // doesn't performReading again, altitude is just calculated 
    // from the pressure and the ground level pressure set earlier
    data.altimeterData.altitude = bmp.readAltitude(baselinePressure);
    */
}

void Altimeter::poll(state* st)
{
    if(!bmp.performReading()) //assigns values to pressure and temperature
    {
        //Serial.println("Failed to perform reading :(");
    }

    st->altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    st->pressure = bmp.readPressure();
    st->temp = bmp.readTemperature();

    /*    
    // Serial.printf("temp: %f\npressure: %f\nalt: %f\n", data.altimeterData.temperature, data.altimeterData.pressure, data.altimeterData.altitude);
    */
}

void Altimeter::enable()
{
    bmp_dev->settings.op_mode = BMP3_NORMAL_MODE;
    if(bmp3_set_op_mode(bmp_dev) != 0)
    {
        //Serial.println("Altimeter failed to enable");
    }
}

void Altimeter::disable()
{
    bmp_dev->settings.op_mode = BMP3_SLEEP_MODE;
    if(bmp3_set_op_mode(bmp_dev) != 0);
        //Serial.println("Altimeter failed to disable");
}
