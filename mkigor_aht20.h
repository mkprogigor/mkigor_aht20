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

struct struct_aht {
    float temp1;
    float humi1;
};

class aht20 {
public:
    aht20();
    bool begin();
    void start_meas();
    bool busy_meas();
    bool is_calibr();
    struct_aht read_data();
};

#endif
    