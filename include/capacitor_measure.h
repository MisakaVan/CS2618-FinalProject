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
    pinMode(port1.analogPin, OUTPUT);
    digitalWrite(port1.analogPin, LOW);
    pinMode(port2.digitalPinLowR, OUTPUT);
    digitalWrite(port2.digitalPinLowR, LOW);
    delay(3000);
    clearPorts();
}


/**
 * Measure the capacitance of the capacitor connected to the given two ports.
 * By measuring the time it takes to charge the capacitor to 63.2% of VCC.
 * @param port1
 * @param port2
 * @return
 */
template <CMeasureMode mode>
capacitance_nanoF_t measureCapacitance(const Port &port1, const Port &port2)
{
    // VCC(port1.analogPin=HIGH) -> C to be measured --*-> ConstantR -> GND(port2.digitalPinHighR=LOW)
    //                                                 *-> AnalogPin(port2.analogPin)
    // use port2.analogPin to measure the voltageOnConstantR between R and port2.digitalPinHighR (*)


    clearPorts();
    pinMode(port1.analogPin, OUTPUT);
    digitalWrite(port1.analogPin, HIGH);
    auto digitalPin = mode == CMeasureMode::LowR ? port2.digitalPinLowR : port2.digitalPinHighR;
    pinMode(digitalPin, OUTPUT);

    auto startTime = millis();
    digitalWrite(digitalPin, HIGH);
    while (getVoltageAtAnalogPin(port2) > VCC * 0.368) {
        delay(1);
    }
    auto endTime = millis();
    digitalWrite(digitalPin, LOW);

    auto timeMiliSec = endTime - startTime;
    auto R = mode == CMeasureMode::LowR ? LOW_RESISTANCE : HIGH_RESISTANCE;

    // time = R * C
    // 1000 ms = 1 ohm * 1e9 nF
    capacitance_nanoF_t capacitance = static_cast<capacitance_nanoF_t>(timeMiliSec * 1e6 / R);
    return capacitance;

    // TODO: remains to be tested!
}

#endif //FINALPROJECT_CAPACITOR_MEASURE_H
