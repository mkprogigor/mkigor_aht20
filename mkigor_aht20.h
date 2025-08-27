/************************************************************************************
Library for test and use ASAIR AHT20 i2c sensor temperature & humidity
by Igor Mkprog, mkprogigor@gmail.com

use examples:
https://github.com/peff74/ESP_AHT20_BMP280

V1.0 from 18.06.2025
************************************************************************************/

#ifndef mkigor_aht20_h
#define mkigor_aht20_h

#include <Arduino.h>
#include <Wire.h>

struct aht_stru {
    float temp1;
    float humi1;
};

class cl_AHT20 {
public:
    cl_AHT20();     // default constructor
    bool begin();
    void do1Meas();
    bool isMeas();
    bool isCalibr();
    aht_stru read_data();
};

#endif
    