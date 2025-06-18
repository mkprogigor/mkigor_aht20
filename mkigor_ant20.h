/************************************************************************************
Library for test and use ASAIR ANT20 i2c sensor temperature & humidity
by Igor Mkprog, mkprogigor@gmail.com

use examples:
https://github.com/peff74/ESP_AHT20_BMP280

V1.0 from 18.06.2025
************************************************************************************/

#ifndef mkigor_ant20_h
#define mkigor_ant20_h

#include <Arduino.h>
#include <Wire.h>

class ant20 {
public:
    ant20();      // create an object of class 
    bool begin();
    void start_meas();
    bool busy_meas();
    void get_data();
    float ant20_humi, ant20_temp;
};

#endif
    