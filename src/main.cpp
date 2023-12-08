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
#include "transistor_measure.h"
#include "detect_condition.h"

/**
 * Basic function test for measuring the R, C, T. Logs to Serial.
 */
void testMainMeasurementToSerial(int intervalMs = 0);


void setup()
{
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }
    Serial.println("Serial ready");
}

void loop()
{
//    // test resistance measure
//    testResistanceMeasure(PORTS[0], PORTS[1]);
//
//    delay(1000);

    testMainMeasurementToSerial(5000);
}


void testMainMeasurementToSerial(int intervalMs)
{
    ConnectionState curState{};
    detectConnection(curState);
    switch (curState.mode) {
        case ConnectionMode::Null: {
            printMsg("No connection detected");
        }
            break;
        case ConnectionMode::R: {
            printMsg("Resistance: %d",
                     static_cast<int>(measureResistance<RMeasureMode::LowR>(
                             PORTS[curState.data.r.port1],
                             PORTS[curState.data.r.port2]
                     )));
            break;
        }
        case ConnectionMode::C: {
            printMsg("Capacitance: %d",
                     static_cast<int>(measureCapacitance<CMeasureMode::LowR>(
                             PORTS[curState.data.c.port1],
                             PORTS[curState.data.c.port2]
                     )));
            break;
        }
        case ConnectionMode::T: {
            auto transistorType = curState.data.t.type;  // TransistorType
            beta_t beta = curState.data.t.beta;  // beta
            printMsg("Transistor Type: %s",
                     transistorType == TransistorType::PNP ? "PNP" : "NPN");
            printMsg("base: %u", curState.data.t.portB);
            printMsg("collector: %u", curState.data.t.portC);
            printMsg("emitter: %u", curState.data.t.portE);
            printMsg("Transistor Beta: %d", beta);
            break;
        }
    }

    delay(intervalMs);

}
