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
resistance_t measureResistance(const Port &port1, const Port &port2)
{
    // VCC(port1.analogPin=HIGH) -> R to be measured --*-> ConstantR -> GND(port2.digitalPinHighR=LOW)
    //                                                 *-> AnalogPin(port2.analogPin)
    // use port2.analogPin to measure the voltageOnConstantR between R and port2.digitalPinHighR (*)

    clearPorts();
    pinMode(port1.analogPin, OUTPUT);
    digitalWrite(port1.analogPin, HIGH);
    if (mode == RMeasureMode::LowR) {
        pinMode(port2.digitalPinLowR, OUTPUT);
        digitalWrite(port2.digitalPinLowR, LOW);
    } else {
        pinMode(port2.digitalPinHighR, OUTPUT);
        digitalWrite(port2.digitalPinHighR, LOW);
    }
    delay(1);

    const auto voltageOnConstantR = getVoltageAtAnalogPin(port2);
    const auto constantR = mode == RMeasureMode::LowR ? LOW_RESISTANCE : HIGH_RESISTANCE;

    printMsg("Mode: %d", static_cast<int>(mode));
    printMsg("analogRead: %d", analogRead(port2.analogPin));
    printMsg("voltageOnConstantR: %d / 1000", static_cast<ull_int>(voltageOnConstantR * 1000));

    // VCC / (R+HighR) = voltageOnConstantR / (HighR)
    // R = HighR * (VCC / voltageOnConstantR - 1)
    if (voltageOnConstantR == 0 || voltageOnConstantR == VCC) {
        return 0;
    }
    auto resistance = static_cast<resistance_t>(constantR * (VCC - voltageOnConstantR) / voltageOnConstantR );
    return resistance;
}

// A test around measureResistance.
// Measure the resistance around two given ports.
// Print the resistance to Serial.
void testResistanceMeasure(const Port &port1, const Port &port2)
{
    auto resistance = measureResistance<RMeasureMode::LowR>(port1, port2);
    printMsg("LowR: %d", static_cast<int>(resistance));
    resistance = measureResistance<RMeasureMode::HighR>(port1, port2);
    printMsg("HighR: %d", static_cast<int>(resistance));
    printMsg("====================================");

    delay(5 * 1000);
}

#endif //FINALPROJECT_RESISTOR_MEASURE_H
