#include "ThinGPS.h"

ThinGPS::ThinGPS(HardwareSerial* serial){
    this->GPSSerial = serial;
    this->init();
}

void ThinGPS::init() {
    this->index = 0;
    this->_current = this->buffer1;
    this->_last = this->buffer2;
    this->_ready = false;
}

 
 void ThinGPS::begin(unsigned long baud) {
    this->GPSSerial->begin(baud);
 }
 
 void ThinGPS::send_command(const char* cmd) {
     this->GPSSerial->println(cmd);
 }
 boolean ThinGPS::is_ready() {
     return this->_ready;
}

 
 char ThinGPS::read() {
     char c = 0;
     if(this->GPSSerial->available()) {
         c = this->GPSSerial->read();
         if (c == '\n') {
             this->_current[this->index] = '\0';
             if(this->_current == this->buffer1){
                 this->_current = this->buffer2;
                 this->_last = this->buffer1;
             } else {
                 this->_current = this->buffer1;
                 this->_last = this->buffer2;
             }
             this->_newnmea = true;
             this->index = 0;
         } else {
             this->_current[this->index++] = c; 
             if (this->index > MAX_CHARS_NMEA) this->index = 0;
         }
     }
     return c;
 }
 
 
 
 boolean ThinGPS::have_newnmea() {
     return this->_newnmea;
}

 
 char* ThinGPS::get_newnmea() {
     this->_newnmea = false;
     return (char *) this->_last;
}
 
 //Faz a validacao da String NMEA
 boolean ThinGPS::is_validnmea(char* nmea) {
    unsigned int sum = 0; int i = 0;
    unsigned int checksum;
    char checksum_string[3];
    *nmea++;
    while(*nmea != '*' && i <= 83) {
        sum = sum ^ *nmea;
        *nmea++; i++;
    }
    *nmea++;
    memset(checksum_string,0,3);
    checksum_string[0] = *nmea; *nmea++;
    checksum_string[1] = *nmea;
    checksum = strtol(checksum_string,0,16);
    if(sum == checksum) return true;
    return false;
}
 
 
 void ThinGPS::update() {
     if (this->have_newnmea()) {
         char *nmea = this->get_newnmea();
         if ( this->is_validnmea(nmea)) {
             this->parse(nmea);
         } 
     }
}
 //Extrai os dados GPRMC e GPGGA
 boolean ThinGPS::parse(char* nmea) {

     if ( strstr(nmea, "$GPRMC")) {
         //Serial.println(nmea);
         char* p = nmea;
         _parse_time( (p = (strchr(p, ',')+1)), &this->gps_data.hour, &this->gps_data.minutes, &this->gps_data.seconds);
         p = strchr(p, ',')+1;
         this->_ready = (p[0] == 'A' );
         if (!this->_ready) return false;
         _parse_position((p = (strchr(p, ',')+1)), &this->gps_data.latitude, 2);
         p = strchr(p, ',')+1;
         if (p[0] == 'S') this->gps_data.latitude *= -1;
         _parse_position((p = (strchr(p, ',')+1)), &this->gps_data.longitude, 3);
         p = strchr(p, ',')+1;
         if (p[0] == 'W') this->gps_data.longitude *= -1;
         _parse_speed((p = (strchr(p, ',')+1)), &this->gps_data.speed);
         p = strchr(p, ',')+1; 
         _parse_date((p = (strchr(p, ',')+1)), &this->gps_data.day, &this->gps_data.month, &this->gps_data.year);         
         
     } else if (strstr(nmea, "$GPGGA") && this->_ready) {
         //Serial.println(nmea);
         char* p = nmea;
         for ( uint8_t i = 0; i < 6; i++) p = strchr(p, ',')+1;
         if ( *p != ',') this->gps_data.fix_indicator = atoi(p);
         p = strchr(p, ',')+1;
         if ( *p != ',') this->gps_data.satelites = atoi(p);
         p = strchr(p, ',')+1;
         if ( *p != ',') this->gps_data.hdop = atof(p);
         p = strchr(p, ',')+1;
         if ( *p != ',') this->gps_data.altitude = atof(p);

     }
     return true;
}
 //Extrai a HORA
 void ThinGPS::_parse_time(char* str, uint8_t* h, uint8_t* m, uint8_t* s) {
     *h = ((str[0]-48)*10)+(str[1]-48);
     *m = ((str[2]-48)*10)+(str[3]-48);
     *s = ((str[4]-48)*10)+(str[5]-48);
}
 //Extrai a DATA
 void ThinGPS::_parse_date(char* str, uint8_t* d, uint8_t* m, uint16_t* y) {
     *d = ((str[0]-48)*10)+(str[1]-48);
     *m = ((str[2]-48)*10)+(str[3]-48);
     *y = ((str[4]-48)*10)+(str[5]-48)+2000;
}
 //Extrai Latitude e Longitude
 void ThinGPS::_parse_position(char* str,float* pos, uint8_t d) {
     char sfix[d+1];
     strncpy(sfix, str, d);
     sfix[d] = '\0';
     float pos_fix = atof(sfix);
     char *p = str+d;
     float min = atof(p);
     min = min/60;
     *pos = pos_fix+min;     
}
 //Extrai a Velocidade
 void ThinGPS::_parse_speed(char* str, float* speed) {
     *speed = atof(str) * 1.852;
}









