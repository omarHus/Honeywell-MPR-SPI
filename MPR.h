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

#ifndef MPR_h
#define MPR_h

#include "Arduino.h"
#include <SPI.h>

class MPR {

public:
    MPR(int pin);
    double getPressure();
    boolean begin();
    
private:
    int _pin;
    void connectToSensor();
    void initiateSPITransfer(int _pin);
    double transferFunction(uint32_t rawValue);


    // Calibration parameters based on datasheet Figure 4
    const double _max_psi = 25.0;       // Maximum Pressure in PSI (From datasheet, depends on sensor type)
    const double _min_psi = 0.0;        // Minimum Pressure in PSI (From datasheet, depends on sensor type)
    const uint32_t _max_cnt = 15099494; // Max count (From datasheet)
    const uint32_t _min_cnt = 1677722;  // Min count (From datasheet)
    const int SPI_CLOCK_SPEED = 800000; // SPI clock speed (From datasheet)
};

#endif // MPR_h
