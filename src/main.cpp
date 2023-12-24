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

# define PART 2

#if PART == 1
#include "part1.h"
#elif PART == 2
#include "part2.h"
#endif



// Adafruit_PCD8544(CLK,DIN,D/C,CE,RST);



Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 17, 16, 15);

void setup()
{

#if PART == 1
    setupPart1(display);
#elif PART == 2
    setupPart2(display);
#endif
//    setupPart1(display);
//    setupPart2(display);



}

void loop()
{
    printItems("\n\nLoop starts.\n");
#if PART == 1
    testMainMeasurementToDisplay(display, 1);
#elif PART == 2
    mainPart2_NPN(display);
#endif

//    setupPart2(display);
//    mainPart2_NPN(display);
    delay(10000);
}






