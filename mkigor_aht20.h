/************************************************************************************
Library for test and use ASAIR AHT20 i2c sensor temperature & humidity
by Igor Mkprog, mkprogigor@gmail.com

use examples:
https://github.com/peff74/ESP_AHT20_BMP280

V1.0 from 30.08.2025

Glossary, abbreviations used in the module, prefix and suffix:
gv_*    -   Global Variable;
lv_*    -   Local Variable (live inside statement);
cl_*    -   CLass;
cd_*    -   Class Definishion;
cgv_*   -   Class public (Global) member (Variable);
clv_*   -   Class private (Local) member (Variable);
cgf_*   -   Class public (Global) metod (Function), not need, no usefull, becouse we see parenthesis => ();
clf_*   -   Class private (Local) metod (Function);

*_stru  -   [or *_stru_t] suffix, as usual, point the type.

Metods (functions) dont use symbol '_', only small or capital letters (ex.: onlySmallOrCapitalLetters)
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
    cl_AHT20(){
        // default constructor
    }
    bool begin();
    void do1Meas();
    bool isMeas();
    bool isCalibr();
    aht_stru readTH();
};

#endif
    