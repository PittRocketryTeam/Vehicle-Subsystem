#include "config.hpp"
#include "Logger.hpp"
#include "Error.hpp"

const int chipSelect = BUILTIN_SDCARD;

Logger::Logger()
{

}

Logger::~Logger()
{
    
}

bool Logger::init()
{
    // Initialize RTC
    /*setSyncProvider(getTeensy3Time);
    delay(100);
    RTC_set_successfully = (timeStatus() == timeSet);*/

    //generateFilename();  // Generate unique log filename

    int i;
    for (i = 0; i < CONN_ATTEMPTS; ++i)
    {
        Error::on(LOG_INIT);
        Serial.println("try to connect SD card");
        
        // pin mapping magic
        //SPI.setMOSI(7);
        //SPI.setMISO(8);
        //int status = SD.begin(62);
        int status = SD.begin(chipSelect);

        if (status)
        {
            break;
        }

        // fail

        delay(CONN_DELAY * 3);
    }

    Error::off();
    if (i == CONN_ATTEMPTS)
    {
        Serial.println("fail");
        Error::display(LOG_INIT, FATAL);
        Error::display(WERE_SCREWED, FATAL);
        return false;
    }
    
    genUniqueFn();
    handle = SD.open(filename, FILE_WRITE);

    return true;
}

void Logger::genUniqueFn()
{
    int log_num = 1;

    if (SD.exists("JRNL"))
    {
        handle = SD.open("JRNL", FILE_READ);
        delay(100);
        char bf[100];
        size_t r = handle.readBytes(bf, 100);
        delay(100);
        bf[r] = '\0';
        log_num = atoi(bf);
        log_num += 1;
        
        handle.close();
        delay(100);
    }

    handle = SD.open("JRNL", FILE_WRITE);
    if (handle)
    {
        handle.seek(0);
        handle.print(log_num);
        handle.close();
    }
    else
    {
        Serial.println("fatal error");
    }
    
    memset(filename, 0, sizeof(filename));
    sprintf(filename, "LG%d.csv", log_num);

    delay(100);
}

/*bool Logger::writeToMemory(Data data)
{
    if (!handle)
    {
        return false;
    }

    handle.printf("%ld, ,%f,%f,%f, ,%f,%f,%f,%f, ,%f,%f,%f, ,%f,%f,%f, ,%f,\n",
            data.timestamp,

            data.altimeterData.temperature, 
            data.altimeterData.pressure, 
            data.altimeterData.altitude,
            
            // data.gpsData.time, 
            data.gpsData.latitude, 
            // data.gpsData.lat_direction, 
            data.gpsData.longitude, 
            // data.gpsData.long_direction, 
            // data.gpsData.fix_quality, 
            data.gpsData.number_of_satellites, 
            // data.gpsData.hdop, 
            data.gpsData.altitude, 
            // data.gpsData.rssi,
            
            // data.healthData.main_battery_temperature, 
            // data.healthData.main_battery_voltage, 
            // data.healthData.reg_5V_battery_temperature, 
            // data.healthData.reg_5V_battery_voltage, 
            // data.healthData.reg_3V3_battery_temperature, 
            // data.healthData.reg_3V3_battery_voltage,
            
            data.imuData.euler_abs_orientation_x, 
            data.imuData.euler_abs_orientation_y, 
            data.imuData.euler_abs_orientation_z,

            data.imuData.acceleration_x,
            data.imuData.acceleration_y,
            data.imuData.acceleration_z,

            data.healthData.main_battery_voltage
            
           // data.photocellData.brightness
    );

    return true; 
}*/

void Logger::write(state* st)
{
    if (handle)
    {
        handle.printf(
            "%ld,%d,,%f,%f,%f,,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
            st->ts,
            st->fcmode,

            st->altitude,
            st->pressure,
            st->temp,
            
            st->ax,
            st->ay,
            st->az,
            st->wx,
            st->wy,
            st->wz,
            st->qx,
            st->qy,
            st->qz,
            st->qw);
    }
}

void Logger::flush()
{
    if (!handle)
    {
        return;
    }
    handle.flush();
}

void Logger::reopen()
{
    if (!handle)
    {
        handle = SD.open(filename, FILE_WRITE);
    }
}

void Logger::close()
{
    if (handle)
    {
        handle.flush();
        handle.close();
    }
}

time_t Logger::getTeensy3Time()
{
    return Teensy3Clock.get();
}