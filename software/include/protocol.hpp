#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <cstdint>

typedef struct _state
{
    unsigned long ts;
    int fcmode;
    
    float temp;
    float vbat;
    float pressure;
    float altitude;
    int nsats;
    float lat;
    float lon;

    float ax;
    float ay;
    float az;
    float wx;
    float wy;
    float wz;
    float qx;
    float qy;
    float qz;
    float qw;

    int main_deploy;
    int drogue_deploy;
    int launch_detect;
    int payload_pre;
    int payload_dep;
} state;

typedef struct _HEALTH_PKT // voltages, current draw
{
    float temp; //battery temperature
    float volt; //battery voltage

} HEALTH_PKT;

typedef struct _IMU_PKT // acceleration, rotation, euler coordinates, etc
{

} IMU_PKT;

typedef struct _BARO_PKT // pressure, temperature, altitude
{
    float pressure;
    float temperature;
    float altitude;
    
} BARO_PKT;

typedef struct _GPS_PKT // logitude, latitude, etc
{
    float lat; //latitude
    float lon; //longitude
    
} GPS_PKT;

typedef struct _EVENT_PKT
{
    char main_deploy;
    char drogue_deploy;
    char launch;
    char payload_deploy;

} EVENT_PKT;

typedef struct _CMD_PKT
{
    int command_type;

} CMD_PKT;

#endif /* PROTOCOL_HPP */
