//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_RESISTOR_MEASURE_H
#define FINALPROJECT_RESISTOR_MEASURE_H

#include "defines.h"
#include "constants.h"
#include "utils.h"


/**
 * Measure the resistance of the resistor connected to the given two ports.
 * The resistance is measured by measuring the voltage across the resistor.
 *
 * @tparam mode low for using the pin connected to the low resistance,
 *  high for using the pin connected to the high resistance.
 * @param port1 this port will be connected to VCC.
 * @param port2 this port will be connected to GND.
 * @return
 */
template <RMeasureMode mode>
resistance_ohm_t measureResistance(const Port &port1, const Port &port2)
{
    // VCC(port1.analogPin=HIGH) -> R to be measured --*-> ConstantR -> GND(port2.digitalPinHighR=LOW)
    //                                                 *-> AnalogPin(port2.analogPin)
    // use port2.analogPin to measure the voltageOnConstantR between R and port2.digitalPinHighR (*)
    clearPorts();
    const auto constantR =
            mode == RMeasureMode::LowR ? LOW_RESISTANCE : HIGH_RESISTANCE;
    const auto port2DigitalPin =
            mode == RMeasureMode::LowR ? port2.digitalPinLowR : port2.digitalPinHighR;

    pinMode(port1.analogPin, OUTPUT);
    digitalWrite(port1.analogPin, HIGH);
    pinMode(port2DigitalPin, OUTPUT);
    digitalWrite(port2DigitalPin, LOW);
    delay(20);

    const auto aR = analogRead(port2.analogPin);

    // VCC / (R+HighR) = voltageOnConstantR / (HighR)
    // R = HighR * (VCC / voltageOnConstantR - 1)
    // auto resistance = static_cast<resistance_ohm_t>(constantR * (VCC - voltageOnConstantR) / voltageOnConstantR );
    // Avoid overflow in double.
    double ratioOfConstantR = 1023.0 / aR - 1;
    auto resistance = static_cast<resistance_ohm_t>(constantR * ratioOfConstantR);

    if (aR <= 3 || aR >= 1015) {
        // aR too small means the R to be measured is too large for the given constant R.
        // aR too large means the R to be measured is too small for the given constant R.
        // abort for accuracy.
        return 0;
    }

    return resistance;
}


#endif //FINALPROJECT_RESISTOR_MEASURE_H
