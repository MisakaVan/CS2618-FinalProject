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
//    const auto voltageOnConstantR = aR / 1023.0 * VCC;

    // VCC / (R+HighR) = voltageOnConstantR / (HighR)
    // R = HighR * (VCC / voltageOnConstantR - 1)
    // auto resistance = static_cast<resistance_ohm_t>(constantR * (VCC - voltageOnConstantR) / voltageOnConstantR );
    // Avoid overflow in double.
    double ratioOfConstantR = 1023.0 / aR - 1;
    auto resistance = static_cast<resistance_ohm_t>(constantR * ratioOfConstantR);
//    auto resistance_k = static_cast<resistance_kOhm_t>(constantR / 1000.0 * ratioOfConstantR);
    // logging
    printItems(">>> measureR\n");
//    printItems("    Mode: ", mode == RMeasureMode::LowR ? "LowR" : "HighR", '\n');
    printItems("    AnalogRead: ", aR, '\n');
//    printItems("    Voltage on constantR: ", voltageOnConstantR, '\n');
//    printItems("    Ratio of constantR: ", ratioOfConstantR, '\n');
//    printItems("    ConstantR: ", static_cast<unsigned long>(constantR), '\n');
    printItems("    Resistance(ohm): ", static_cast<unsigned long>(resistance), '\n');
//    printItems("    Resistance(kOhm): ", resistance_k, '\n');
    printItems("<<< measureR", '\n');

    if (aR <= 3 || aR >= 1015) {
        // aR too small means the R to be measured is too large for the given constant R.
        // aR too large means the R to be measured is too small for the given constant R.
        // abort for accuracy.
        return 0;
    }

    return resistance;
}

//// A test around measureResistance.
//// Measure the resistance around two given ports.
//// Print the resistance to Serial.
//void testResistanceMeasure(const Port &port1, const Port &port2)
//{
//    auto resistance = measureResistance<RMeasureMode::LowR>(port1, port2);
//    printMsg("LowR: %lld", resistance);
//
//    resistance = measureResistance<RMeasureMode::HighR>(port1, port2);
//    printMsg("HighR: %lld", resistance);
//
//    printMsg("====================================");
//
//    delay(5 * 1000);
//}

#endif //FINALPROJECT_RESISTOR_MEASURE_H
