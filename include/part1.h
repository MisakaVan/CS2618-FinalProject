//
// Created on 2023/12/16.
//

#ifndef FINALPROJECT_PART1_H
#define FINALPROJECT_PART1_H

#include "defines.h"
#include "constants.h"
#include "utils.h"
#include "resistor_measure.h"
#include "capacitor_measure.h"
#include "transistor_measure_new.h"
#include "detect_condition.h"


class Adafruit_PCD8544;

void setupPart1(Adafruit_PCD8544 &display)
{
    clearPorts();
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }
    Serial.println("Serial ready");
    display.begin();
    display.setContrast(50);
    display.clearDisplay();
    display.setTextSize(1);
    display.println("No connection!");
    display.display();
    delay(5000);
    display.clearDisplay();
}


///**
// * Basic function test for measuring the R, C, T. Logs to Serial.
// */
//void testMainMeasurementToSerial(int intervalMs)
//{
//    ConnectionState curState{};
//    detectConnection(curState);
//    switch (curState.mode) {
//        case ConnectionMode::Null: {
//            printMsg("No connection detected");
//        }
//            break;
//        case ConnectionMode::R: {
//            printMsg("Resistance: %d",
//                     static_cast<int>(measureResistance<RMeasureMode::LowR>(
//                             PORTS[curState.data.r.port1],
//                             PORTS[curState.data.r.port2]
//                     )));
//            break;
//        }
//        case ConnectionMode::C: {
//            printMsg("Capacitance: %d",
//                     static_cast<int>(measureCapacitance<CMeasureMode::LowR>(
//                             PORTS[curState.data.c.port1],
//                             PORTS[curState.data.c.port2]
//                     )));
//            break;
//        }
//        case ConnectionMode::T: {
//            auto transistorType = curState.data.t.type;  // TransistorType
//            beta_t beta = curState.data.t.beta;  // beta
//            printMsg("Transistor Type: %s",
//                     transistorType == TransistorType::PNP ? "PNP" : "NPN");
//            printMsg("base: %u", curState.data.t.portB);
//            printMsg("collector: %u", curState.data.t.portC);
//            printMsg("emitter: %u", curState.data.t.portE);
//            printMsg("Transistor Beta: %d", beta);
//            break;
//        }
//    }
//
//    delay(intervalMs);
//
//}

/**
 * Basic function test for measuring the R, C, T. Logs to LCD.
 * @param intervalMs
 */
void testMainMeasurementToDisplay(Adafruit_PCD8544 &display, int intervalMs)
{
    ConnectionState curState{};
    printItems("Detecting connection...\n");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Detecting ...");
    display.display();


    detectConnection(curState);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.print("Analyzing ...");
    display.display();

    switch (curState.mode) {
        case ConnectionMode::Null: {
            printItems("== No connection ==");
            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.println("No connection!");
            display.display();
            delay(200);
//            display.clearDisplay();
            break;
        }

        case ConnectionMode::R: {
            printItems("== Connection: R ==\n");
            printItems("port id: ", curState.data.r.port1, ", ", curState.data.r.port2, "\n");

            const auto r_h = measureResistance<RMeasureMode::HighR>(
                    PORTS[curState.data.r.port1],
                    PORTS[curState.data.r.port2]
            );
            printItems("R from HighR mode: ", r_h, "(ohm)\n");

            const auto r_l = measureResistance<RMeasureMode::LowR>(
                    PORTS[curState.data.r.port1],
                    PORTS[curState.data.r.port2]
            );
            printItems("R from LowR mode: ", r_l, "(ohm)\n");

            const auto finalResult = r_h == 0 ? r_l : r_h;

            printItems("Final result: R=", finalResult, "(ohm)\n");

            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.print("Resistor.");

            display.setCursor(0, 12);
            printItemsToDisplay(display, "Final result: R=", finalResult, "(ohm)\n");
            display.display();
            delay(1000);
            display.clearDisplay();
            break;
        }

            //这部分加上去，LCD不亮了，不知道为什么，我怀疑有bug
        case ConnectionMode::C: {
            printItems("== Connection: C ==\n");

            printItems("port id: ", curState.data.c.port1, " ", curState.data.c.port2, "\n");


            const auto c_hh = measureCapacitancePrototype<>(
                    PORTS[curState.data.c.port1],
                    PORTS[curState.data.c.port2]
            );

            printItems("Final result: C=", c_hh, "(nF)\n");

            display.clearDisplay();
            display.setCursor(0, 12);
            printItemsToDisplay(display, "Final result: C=", c_hh, "(nF)\n");
            display.display();
            delay(1000);

            break;
        }

        case ConnectionMode::T: {
            printItems("== Connection: T ==\n");


            auto transistorType = curState.data.t.type;  // TransistorType
//            beta_t beta = curState.data.t.beta;  // beta
            printMsg("Transistor Type: %s",
                     transistorType == TransistorType::PNP ? "PNP" : "NPN");
            printMsg("b: %u ", curState.data.t.portB);
            printMsg("c: %u ", curState.data.t.portC);
            printMsg("e: %u ", curState.data.t.portE);
            printItems("Beta: ", curState.data.t.beta, '\n');



            display.clearDisplay();
            display.setCursor(0, 0);
            display.setTextSize(1);
            display.print("Transistor:");
            String s = transistorType == TransistorType::PNP ? "PNP" : "NPN";
            printItemsToDisplay(display, s, "\n");

            display.setCursor(0, 12);
            printItemsToDisplay(display, "Base:", curState.data.t.portB, "\n");

            display.setCursor(0, 21);
            printItemsToDisplay(display, "Collector:", curState.data.t.portC, "\n");

            display.setCursor(0, 30);
            printItemsToDisplay(display, "Emitter:", curState.data.t.portE, "\n");

            display.setCursor(0, 39);
            printItemsToDisplay(display, "Beta:", curState.data.t.beta, "\n");

            display.display();

            delay(1000);
//            display.clearDisplay();
            break;
        }
    }

    delay(intervalMs);

}


#endif //FINALPROJECT_PART1_H
