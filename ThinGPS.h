/*
* ThinGPS.h
*
* Biblioteca para tratamento de dados NMEA
*
* Criado em 21 Agosto de 2016
* Por Luan Felipe (github: @luanfssilva | Email: luanfelipebtu@hotmail.com), Ronitti Junner (email: ronittijuner@gmail.com)
*
* Modificado em 11 Fevereiro de 2019
* Por Luan Felipe @luanfssilva
*
* MIT License
*/


#ifndef THINGPS_H
#define THINGPS_H
#include <Arduino.h>
#include <HardwareSerial.h>

#define MAX_CHARS_NMEA 84

struct GpsData {
    uint8_t hour, minutes, seconds, day, month;
    uint16_t year;
    uint8_t satelites, fix_indicator;
    float speed, altitude;
    float latitude, longitude, hdop;
};

class ThinGPS {
public:
    HardwareSerial *GPSSerial;
    GpsData gps_data;
    ThinGPS(HardwareSerial* serial);
    void init();
    void begin(unsigned long baud);
    void send_command(const char *cmd);
    char* get_newnmea();
    boolean have_newnmea();
    boolean is_validnmea(char* nmea);
    boolean is_ready();
    void update();
    char read();
    boolean parse(char* nmea);


private:

    void _parse_time(char* str, uint8_t* h, uint8_t* m, uint8_t* s);
    void _parse_date(char* str, uint8_t* d, uint8_t* m, uint16_t* y);
    void _parse_position(char* str, float* pos, uint8_t d);
    void _parse_speed(char* str, float* speed);

    boolean _ready;
    volatile boolean _newnmea = false;

    volatile uint8_t index;
    volatile char buffer1[MAX_CHARS_NMEA];
    volatile char buffer2[MAX_CHARS_NMEA];
    volatile char* _current;
    volatile char* _last ;
};


#endif /* THINGPS_H */

