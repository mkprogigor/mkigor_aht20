/************************************************************************************
Library for test and use ASAIR ANT20 i2c sensor temperature & humidity
by Igor Mkprog, mkprogigor@gmail.com

use examples:
https://github.com/peff74/ESP_AHT20_BMP280

V1.0 from 18.06.2025
************************************************************************************/

#include "mkigor_ant20.h"
float ant20_humi, ant20_temp;

bool begin() {
    Wire.beginTransmission(0x38);
    Wire.write(0xBE);  // 0xBE --> init register for AHT2x
    if (Wire.endTransmission() == 0) {
        Serial.println("ANT20 is present.");                return true;
    }
    else {
        Serial.println("ANT20 not connect, check cables."); return false;
    }
}

void start_meas() {
    Wire.beginTransmission(0x38);
    Wire.write(0xAC);  // 0xAC --> start measurement
    Wire.write(0x33);  // 0x33 --> not really documented what it does, but it's called MEASUREMENT_CTRL
    Wire.write(0x00);  // 0x00 --> not really documented what it does, but it's called MEASUREMENT_CTRL_NOP
    Wire.endTransmission();
}

bool busy_meas() {
    Wire.requestFrom(0x38, 1);
    if (Wire.available()) {
        uint8_t _status = Wire.read();
        if (_status & 0x80) return true;
        return false;
    }
    else return false;
}

void get_data() {
    uint8_t __str[7];
    if (Wire.requestFrom(0x38, 7) == 7) {   // Request 7 bytes of data
        for (uint8_t i = 0; i < 7; i++)   __str[i] = Wire.read();
    }

    uint8_t crc = 0xFF;   // Check CRC
    for (uint8_t byteIndex = 0; byteIndex < 6; byteIndex++) {
        crc ^= __str[byteIndex];
        for (uint8_t bitIndex = 8; bitIndex > 0; --bitIndex) {
            if (crc & 0x80) crc = (crc << 1) ^ 0x31;
            else            crc = (crc << 1);
        }
    }
    if (crc != __str[6]) {
        Serial.println("CRC check failed");   return;
    }
    else Serial.println("CRC Ok");

    // Extract the raw data humidity from the bytes 1,2,3
    // Byte 1: is 11-12 bits, Byte 2: is 11-4 bits, Byte 3 MSB 4 bits is 3-0 bits of raw data for humidity
    uint32_t __humi = (__str[1] << 12) | (__str[2] << 4) | (__str[3] >> 4);
    // Serial.print("Humidity (raw): ");  Serial.println(__humi);
    ant20_humi = (float)__humi / 1048576.0 * 100;

    // Extract the raw data temperature from the bytes 3,4,5
    // Byte 3 LSB 4 bits is 19-16 bits, Byte 4 is 15-8 bits, Byte 5 is 7-0 bits of the raw data temperature
    uint32_t __temp = ((__str[3] & 0x0f) << 16) | (__str[4] << 8) | (__str[5]);
    // Serial.print("Temperature (raw): ");   Serial.println(__temp);
    ant20_temp = (float)__temp / 1048576.0 * 200.0 - 50.0;
}
