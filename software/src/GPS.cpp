#include "GPS.hpp"
#include "config.hpp"
#include "Error.hpp"

GPS::GPS() :
    Sensor(),
    my_time(0),
    my_lat(0),
    my_lat_direction(0),
    my_lon(0),
    my_long_direction(0),
    my_fix_quality(0),
    my_number_of_satellites(0),
    my_hdop(0),
    my_alt(0),
    my_rssi(0)
{
}

GPS::~GPS()
{

}

void GPS::init()
{
    pinMode(GPS_ENABLE, OUTPUT);

    gps = Adafruit_GPS(&GPS_SERIAL);

    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    int err = 0;
    gps.begin(9600);
    int i;
    for (i = 0; i < CONN_ATTEMPTS; i++)
    {
        Error::on(GPS_INIT);

        if (gps.available())
        {
            break;
        }

        delay(CONN_DELAY);
    }
    
    Error::off();
    if (i == CONN_ATTEMPTS)
    {
        Serial.println("cannot init gps");
        Error::display(GPS_INIT, FATAL);
        Error::display(WERE_SCREWED, FATAL);
    }

    /*while (!gps.available())
    {
        Error::on(GPS_INIT);
        err++;
        delay(CONN_DELAY);

        if (err > 10)
        {
            Serial.println("GPS INIT ERR");
            Error::display(GPS_INIT, FATAL);
            break;
        }
    }*/
    Error::off();
    
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    
    // uncomment this line to turn on only the "minimum recommended" data
    //gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

    // Set the update rate
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

    // Request updates on antenna status, comment out to keep quiet
    gps.sendCommand(PGCMD_ANTENNA);
}

void GPS::internal_read()
{
    gps.read();
}

void GPS::read(state* st)
{
    /*data.gpsData.time = my_time;
    data.gpsData.latitude = my_lat;
    data.gpsData.lat_direction = my_lat_direction;
    data.gpsData.longitude = my_lon;
    data.gpsData.long_direction = my_long_direction;
    data.gpsData.fix_quality = my_fix_quality;
    data.gpsData.number_of_satellites = my_number_of_satellites;
    data.gpsData.hdop = my_hdop;
    data.gpsData.altitude = my_alt;
    data.gpsData.rssi = my_rssi;*/
}

void GPS::poll(state* st)
{
    //if (gps.newNMEAreceived())
    {
        //Error::on(WERE_SCREWED);
        gps.parse(gps.lastNMEA());
    }

    st->lon = gps.longitude;
    st->lat = gps.latitude;
    st->nsats = gps.satellites;

    /*my_fix_quality = gps.fixquality;
    my_time = gps.seconds;
    my_number_of_satellites = gps.satellites;
    my_lat = gps.latitude;
    my_lon = gps.longitude;
    my_alt = gps.altitude;
    //my_rssi = atoi(gps.lastNMEA());
    my_hdop = gps.HDOP;*/

    read(st);
}

void GPS::enable()
{
    //digitalWrite(GPS_ENABLE, HIGH);
}

void GPS::disable()
{
    //digitalWrite(GPS_ENABLE, LOW);
}
