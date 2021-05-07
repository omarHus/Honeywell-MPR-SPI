#include "MPR.h"

int pin1 = 10;
int pin2 = 9;

// Create mpr sensor objects
MPR mpr1 = MPR(pin1);
MPR mpr2 = MPR(pin2);

void setup() {
    Serial.begin(9600);
    SPI.begin();

    Serial.println("Testing MPR Pressure Sensors");

    if (!mpr1.begin()){
      Serial.println("Failed to connect to sensor 1, check wiring!");
      while(1);
    }

    if (!mpr2.begin()){
      Serial.println("Failed to connect to sensor 2, check wiring!");
      while(1);
    }

    Serial.println("All sensors connected");
}

void loop() {
    // Get pressure reading
    double pressure1 = mpr1.getPressure();
    double pressure2 = mpr2.getPressure();

    Serial.print("Pressure 1 in PSI = "); Serial.println(pressure1);
    Serial.print("Pressure 2 in PSI = "); Serial.println(pressure2);

    delay(1000);
}
