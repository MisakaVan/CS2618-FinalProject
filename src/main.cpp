/**
 * @file main.cpp
 * @brief This project is a multi-purpose implementation that can be used to test and measure
 * 3 kinds of components:
 *      1. The resistance of a resistor.
 *      2. The capacitance of a capacitor.
 *      3. The sort and beta of an PNP/NPN transistor.
 *
 */

#include <Arduino.h>
#include "resistor_measure.h"
#include "capacitor_measure.h"

void setup()
{
// write your initialization code here
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }
    Serial.println("Serial ready");
}

void loop()
{
    // test resistance measure
    testResistanceMeasure(PORTS[0], PORTS[1]);

    delay(1000);
}

