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


#include <Adafruit_PCD8544.h>

#include "part1.h"
//#include "part2.h"



// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);



Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 17, 16, 15);

void setup()
{

//    setupPart1(display);
//    setupPart2(display);



    clearPorts();
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }
    Serial.println("Serial ready");
    display.begin();
//    display.setContrast(23);
    display.setContrast(40);
    display.clearDisplay();
    display.setTextSize(1);
    display.println("No connection!");
    display.display();
    delay(5000);
    display.clearDisplay();


}

void loop()
{
    printItems("\n\nLoop starts.\n");
    testMainMeasurementToDisplay(display, 1);


//    setupPart2(display);
//    mainPart2_NPN(display);
    delay(1000);
}






