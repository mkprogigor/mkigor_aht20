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
#include <mkigor_aht20.h>

bool cl_AHT20::begin() {
    Wire.beginTransmission(0x38);
    Wire.write(0xBE);  // 0xBE --> init register for AHT2x
    if (Wire.endTransmission() == 0) {
        Serial.println("ANT20 is present.");                return true;
    }
    else {
        Serial.println("ANT20 not connect, check cables."); return false;
    }
}

void cl_AHT20::do1Meas() {
    Wire.beginTransmission(0x38);
    Wire.write(0xAC);  // 0xAC --> start measurement
    Wire.write(0x33);  // 0x33 --> not really documented what it does, but it's called MEASUREMENT_CTRL
    Wire.write(0x00);  // 0x00 --> not really documented what it does, but it's called MEASUREMENT_CTRL_NOP
    Wire.endTransmission();
}

bool cl_AHT20::isMeas() {
    Wire.requestFrom(0x38, 1);
    if (Wire.available()) {
        uint8_t _status = Wire.read();
        if (_status & 0x80) return true;
        return false;
    }
    else return false;
}

bool cl_AHT20::isCalibr() {
    Wire.requestFrom(0x38, 1);
    if (Wire.available()) {
        uint8_t _status = Wire.read();
        // Serial.print("Status = ");   Serial.println(_status, HEX);
        if (_status & 0x08) return true;
        return false;
    }
    else return false;
}

aht_stru cl_AHT20::readTH() {
    aht_stru lv_aht1 = {0, 0};
    uint8_t lv_regs[7];       // Request 7 bytes of data
    if (Wire.requestFrom(0x38, 7) == 7)  for (uint8_t i = 0; i < 7; i++) lv_regs[i] = Wire.read();

    // Serial.print("Status = ");   Serial.println(lv_regs[0], HEX);
    uint8_t crc = 0xFF;   // Check CRC
    for (uint8_t byteIndex = 0; byteIndex < 6; byteIndex++) {
        crc ^= lv_regs[byteIndex];
        for (uint8_t bitIndex = 8; bitIndex > 0; --bitIndex) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else            crc = (crc << 1);
        }
    }
    if (crc != lv_regs[6]) {
        Serial.println("CRC check failed");
        return lv_aht1;
    }
    // else Serial.println("CRC Ok");

    // Extract the raw data humidity from the bytes 1,2,3
    // Byte 1: is 11-12 bits, Byte 2: is 11-4 bits, Byte 3 MSB 4 bits is 3-0 bits of raw data for humidity
    uint32_t lv_humi = (lv_regs[1] << 12) | (lv_regs[2] << 4) | (lv_regs[3] >> 4);
    // Serial.print("Humidity (raw): ");  Serial.println(__humi);
    lv_aht1.humi1 = ((float)lv_humi) / 1048576.0 * 100;

    // Extract the raw data temperature from the bytes 3,4,5
    // Byte 3 LSB 4 bits is 19-16 bits, Byte 4 is 15-8 bits, Byte 5 is 7-0 bits of the raw data temperature
    uint32_t lv_temp = ((lv_regs[3] & 0x0f) << 16) | (lv_regs[4] << 8) | (lv_regs[5]);
    // Serial.print("Temperature (raw): ");   Serial.println(__temp);
    lv_aht1.temp1 = ((float)lv_temp) / 1048576.0 * 200.0 - 50.0;
    return lv_aht1;
}