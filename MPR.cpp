/* 
        Arduino Library for Honeywell MPR Pressure Sensors
        
        Supports connecting multiple Honeywell MPR sensors to the same Serial bus using SPI.

        Circuit:
        - Sensor SCL connected to scl0 of arduino (keep physically away from MISO and MOSI wires)
        - Sensor MOSI connected to MOSI0 pin of arduino
        - Sensor MISO connected to MISO0 pin of arduino
        - Sensor SS connected to any digital pin on arduino (for some microcontrollers you need to use a specific pin for at least one pin (e.g. pin 10))
        - VCC Connected to 3.3V logic
        - Ground to Ground
        - Any additional sensors can connect SCL, MOSI, MISO, VCC, and Ground in Parallel to the first sensor. SS pin has to be unique.

        Created May 7, 2021
        By Omar Husain
        MIT License

*/

#include "MPR.h"

/* Constructor for pressure sensor. Takes chip select (SS) pin as argument. */
MPR::MPR(int pin)
{
    pinMode(pin, OUTPUT);    // Set chip select pin as an output
    digitalWrite(pin, HIGH); // Deselect pin
    
    _pin = pin;
    
}

boolean MPR::begin()
{
    // Get status byte from sensor
    initiateSPITransfer(_pin);
    uint8_t stat = SPI.transfer(0xF0);

    // End SPI sequence
    digitalWrite(_pin, HIGH);
    SPI.endTransaction();
    
    return (stat == 0x40);
}

/* Get pressure reading from the sensor */
double MPR::getPressure()
{     
    // Step 1 and 2 of datasheet: Check pressure sensor status
    connectToSensor();

    // Step 3: read pressure count (3x8bit = 24bit pressure count)
    uint32_t raw_value = 0;
    for(uint8_t i = 0; i<3; i++)
    {
        // Get pressure count bits
        raw_value |= SPI.transfer(0x00);
        if (i != 2) raw_value = raw_value << 8;
    }

     // De-select pin and end SPI transaction
    digitalWrite(_pin, HIGH);
    SPI.endTransaction();

    // Convert to PSI using Transfer Function A from Datasheet
    return transferFunction(raw_value);
}

/* Call before any spi transfer sequence to ensure master and slave are communicating at the right end of clock pulses */
void MPR::initiateSPITransfer(int chipSelect)
{
  SPI.beginTransaction(SPISettings(SPI_CLOCK_SPEED, MSBFIRST, SPI_MODE0));
  digitalWrite(chipSelect, LOW);
}

/* Connect to the sensor and get status byte */
void MPR::connectToSensor()
{
    int chipSelect = _pin;
  
    // Start SPI transaction with pin
    initiateSPITransfer(chipSelect);

    // Step 1 - Datasheet SPI commands pp 18 to ask sensor to send master the pressure reading
    SPI.transfer(0xAA);
    SPI.transfer(0x00);
    SPI.transfer(0x00);

    // End SPI sequence
    digitalWrite(_pin, HIGH);
    SPI.endTransaction();

    // Step 2: Delay to clear MISO line
    delay(5);

    // Step 3: Get status Byte
    initiateSPITransfer(chipSelect);
    SPI.transfer(0xF0);
    
}

/* Convert from raw 24-bit value from sensor to the PSI reading (datasheet section 8) */
double MPR::transferFunction(uint32_t raw_value)
{
    return (((raw_value - _min_cnt) * (_max_psi - _min_psi)) / (_max_cnt - _min_cnt)) + _min_psi;
}
