#include "i2c_scanner.h"

#include "esp_log.h"
#include <Arduino.h>
#include <Wire.h>

i2c_scanner::i2c_scanner(/* args */)
{
    byte error, address;
    int nDevices;

    log_d("Scanning...");

    nDevices = 0;
    byte max_address = 1 << 7;
    for (address = 1; address < max_address; address++)
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            log_d("I2C device found at address '0x%02x' !", address);

            nDevices++;
        }
        else if (error == 4)
        {
            log_d("Unknown error at address '0x%02x' !", address);
        }
    }
    if (nDevices == 0)
        log_d("No I2C devices found\n");
    else
        log_d("done\n");
}

i2c_scanner::~i2c_scanner()
{
}
