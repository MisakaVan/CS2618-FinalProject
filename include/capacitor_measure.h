//
// Created on 2023/12/4.
//

#ifndef FINALPROJECT_CAPACITOR_MEASURE_H
#define FINALPROJECT_CAPACITOR_MEASURE_H

#include "defines.h"
#include "constants.h"
#include "utils.h"

void dischargeCapacitor(const Port &port1, const Port &port2)
{
    clearPorts();
    // discharge the capacitor with the small resistor
    pinMode(port1.digitalPinLowR, OUTPUT);
    digitalWrite(port1.digitalPinLowR, LOW);
    pinMode(port2.digitalPinLowR, OUTPUT);
    digitalWrite(port2.digitalPinLowR, LOW);
    while (analogRead(port1.analogPin) > 1 || analogRead(port2.analogPin) > 1) {
        delay(1);
    }
    delay(5);
    clearPorts();
}


template <CMeasureMode mode1 = CMeasureMode::HighR, CMeasureMode mode2 = CMeasureMode::HighR>
capacitance_nanoF_t measureCapacitancePrototype(const Port &port1, const Port &port2)
{
/*
 * VCC(port1.digitalPinHighR=HIGH) -> constantR1 -*-> C -> constantR2 -*-> GND(port2.digitalPinHighR=LOW)
 *
 *
 */
    printItems(">>> measureC()\n");
    printItems("    Mode: ",
               mode1 == CMeasureMode::LowR ? "LowR" : "HighR", " ",
               mode2 == CMeasureMode::LowR ? "LowR" : "HighR", '\n');

    clearPorts();
    // discharge the capacitor with the small resistor
    pinMode(port1.digitalPinLowR, OUTPUT);
    digitalWrite(port1.digitalPinLowR, LOW);
    pinMode(port2.digitalPinLowR, OUTPUT);
    digitalWrite(port2.digitalPinLowR, LOW);
    while (analogRead(port1.analogPin) > 1 || analogRead(port2.analogPin) > 1) {
        delay(1);
    }
    delay(5);
    clearPorts();

    // charge the capacitor with the big resistor
    const pinID_t pinR1 = mode1 == CMeasureMode::LowR ? port1.digitalPinLowR : port1.digitalPinHighR;
    const pinID_t pinR2 = mode2 == CMeasureMode::LowR ? port2.digitalPinLowR : port2.digitalPinHighR;
    const resistance_kOhm_t totalR = (mode1 == CMeasureMode::LowR ? LOW_RESISTANCE_K : HIGH_RESISTANCE_K) +
                                     (mode2 == CMeasureMode::LowR ? LOW_RESISTANCE_K : HIGH_RESISTANCE_K);


    pinMode(pinR1, OUTPUT);
    digitalWrite(pinR1, LOW);
    pinMode(pinR2, OUTPUT);
    digitalWrite(pinR2, LOW);

    auto startTime = micros();
    digitalWrite(pinR1, HIGH);
    for (;;) {
        auto highV = analogRead(port1.analogPin);
        auto lowV = analogRead(port2.analogPin);
        auto deltaV = highV - lowV;
        if (deltaV > 646) {
            // charged for a RC time
            break;
        }
    }
    auto endTime = micros();
    digitalWrite(pinR1, LOW);

    auto timeElapsedMs = endTime - startTime;

    // t = R * C
    // 1e6 MicroSec = 1e-3 kOhm * 1e9 nF
    // 1 nF = 1 MicroSec / 1 kOhm
    auto c = static_cast<capacitance_nanoF_t>( timeElapsedMs / totalR );

    printItems("    Capacitance: ", c, " nF\n");
    printItems("<<< measureC()\n");
    return c;
}


#endif //FINALPROJECT_CAPACITOR_MEASURE_H
