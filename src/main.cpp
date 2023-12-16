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
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "resistor_measure.h"
#include "capacitor_measure.h"
//#include "transistor_measure_old.h"
#include "transistor_measure_new.h"
#include "detect_condition.h"
#include "part2.h"



// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);



Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 17, 16, 15);

void setup()
{
//    Serial.begin(9600);
//    while (!Serial) {
//        delay(10);
//    }
//    Serial.println("Serial ready");
//    display.clearDisplay();
//    display.begin();
//    display.setContrast(23);
//    display.println("No connection!");
//    display.display();
//    delay(5000);
//    display.clearDisplay();
    setup2();

}

void loop()
{
//    testPrintMsg();
//    testMainMeasurementToSerial(5000);
//    capacitorMeasurePrototype();
//    testMainMeasurementToDisplay(5000);
    loop2();
}






