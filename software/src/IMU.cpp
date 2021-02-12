#include "IMU.hpp"
#include "config.hpp"
#include "Error.hpp"


IMU::IMU()
{
    //last_data = new Data;
}

IMU::~IMU()
{
    //delete last_data;
}

void IMU::init()
{
    sensor = Adafruit_BNO055(55, IMU_ADDR, &Wire1);
    verbose = false;

    int i;
    for (i = 0; i < 5; ++i)
    {
        Error::on(IMU_INIT);
        if (sensor.begin())
        {
            break;
        }

        if (VERBOSE)
        {
            Serial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        }
        //delay(CONN_DELAY);
    }
    Error::off();
    if (i >= CONN_ATTEMPTS)
    {
        Serial.println("cannot connect to imu");
        Error::display(IMU_INIT, FATAL);
    }

    sensor.setExtCrystalUse(true);

    enable();

}

void IMU::read(state* st)
{
    st->ax = a.x();
    st->ay = a.y();
    st->az = a.z();

    st->wx = w.x();
    st->wy = w.y();
    st->wz = w.z();    

    st->qx = q.x();
    st->qy = q.y();
    st->qz = q.z();
    st->qw = q.w();
}

void IMU::poll(state* st)
{
    sensor.getEvent(&event);

    //ox = event.orientation.x;
    //oy = event.orientation.y;
    //oz = event.orientation.z;
    //ox = oy = oz = 69.0f;
    /*o = sensor.getVector(Adafruit_BNO055::VECTOR_EULER);
    ox = (float)o.x();
    oy = (float)o.y();
    oz = (float)o.z();*/

    a = sensor.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    w = sensor.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    q = sensor.getQuat();

    // Serial.print(ox);
    // Serial.print(", ");
    // Serial.print(oy);
    // Serial.print(", ");
    // Serial.print(oz);
    // Serial.println("");

    //ax = event.acceleration.x;
    //ay = event.acceleration.y;
    //az = event.acceleration.z;

    //last_data = data;

    read(st);
}

void IMU::enable()
{
    if (VERBOSE)
    {
        Serial.println("IMU Enabled");
    }

    sensor.enterNormalMode();
}

void IMU::disable()
{
    if (VERBOSE)
    {
        Serial.println("IMU Disabled");
    }

    sensor.enterSuspendMode();
}
