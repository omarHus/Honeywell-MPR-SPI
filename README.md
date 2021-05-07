# Honeywell Micropressure Sensors (MPR Series) With SPI

This `arduino` library allows you to hook up multiple [Honeywell micropressure sensors](https://sensing.honeywell.com/honeywell-sensing-micropressure-board-mount-pressure-mpr-series-datasheet-32332628-en.pdf) to the same serial bus using SPI.


![Honeywell Micropressure Sensors](images/Honeywell_MicroPressure_Evaluation_Board.png?raw=true "Honeywell MPR Series Sensors")

## Usage
Most libraries give you the I2C protocol for hooking up these sensors, but this only allows you to have one sensor per bus, which is very limiting. Using SPI and this library, you can hook up multiple sensors, which allows you to measure pressure in multiple locations, and even convert your system into a flow meter.

These sensors are rated for 3.3 V so if you are using an arduino uno you will have to be sure to convert the signal from 5V to 3.3V.
I have used this library on a teensy 4.0 which is a 3.3V microcontroller and it works great.

```
// Start SPI
SPI.begin();

// Create pressure sensor object
sensor1 = new mpr(sensorPin1);

// Get pressure reading
double pressure1 = sensor1.getPressure();
```

It's as simple as that folks.

## Hookup guide

To hookup your sensors to your microcontroller you need to know the SPI pins of your controller. The table below shows the SPI pins for a teensy 4.0.

| Sensor Pin      | Teensy 4.0 Pin |
| ------------- |:-------------:|
| SCLK/SCL | 15 |
| MISO     | 12 |
| MOSI     | 11 |
| SS Chip Select       | 10 |
| GND      | G  |
| VCC      | 3V |

To hook up multiple sensors to the same Serial Bus you just have to hook up the additional sensor's SCL, MOSI, MISO, GND, and VCC in parallel to the first sensor. The SS for the additional sensor has to be unique and not the same as the first sensor. At least one of the sensors must be connected to the main chip select pin on your controller (for teensy 4 it is pin 10).