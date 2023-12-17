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


///**
// * Measure the capacitance of the capacitor connected to the given two ports.
// * By measuring the time it takes to charge the capacitor to 63.2% of VCC.
// * @param port1
// * @param port2
// * @return
// */
//template <CMeasureMode mode>
//capacitance_nanoF_t measureCapacitance(const Port &port1, const Port &port2)
//{
//    // VCC(port1.analogPin=HIGH) -> C to be measured --*-> ConstantR -> GND(port2.digitalPinHighR=LOW)
//    //                                                 *-> AnalogPin(port2.analogPin)
//    // use port2.analogPin to measure the voltageOnConstantR between R and port2.digitalPinHighR (*)
//
//
//    clearPorts();
//    pinMode(port1.analogPin, OUTPUT);
//    digitalWrite(port1.analogPin, HIGH);
//    auto digitalPin = mode == CMeasureMode::LowR ? port2.digitalPinLowR : port2.digitalPinHighR;
//    pinMode(digitalPin, OUTPUT);
//
//    auto startTime = millis();
//    digitalWrite(digitalPin, HIGH);
//    while (getVoltageAtAnalogPin(port2) > VCC * 0.368) {
//        delay(1);
//    }
//    auto endTime = millis();
//    digitalWrite(digitalPin, LOW);
//
//    auto timeMiliSec = endTime - startTime;
//    auto R = mode == CMeasureMode::LowR ? LOW_RESISTANCE : HIGH_RESISTANCE;
//
//    // time = R * C
//    // 1000 ms = 1 ohm * 1e9 nF
//    auto capacitance = static_cast<capacitance_nanoF_t>((timeMiliSec / R) * 1000000ll );
//    return capacitance;
//
//    // remains to be tested!
//}

//void capacitorMeasurePrototype()
//{
//    clearPorts();
//    auto port1 = PORTS[0];
//    auto port2 = PORTS[1];
//    auto pin1 = port1.digitalPinHighR;
//    auto pin2 = port2.digitalPinHighR;
//    pinMode(pin1, OUTPUT);
//    pinMode(pin2, OUTPUT);
//    digitalWrite(pin2, LOW);
//
//    while (true) {
//        printMsg("=====Charging=====");
//        auto curTime = micros();
//
//        digitalWrite(pin1, HIGH);
//        for (;;) {
////            printMsg("analogRead: %d", analogRead(port1.analogPin));
//            auto highV = analogRead(port1.analogPin);
//            auto lowV = analogRead(port2.analogPin);
//            auto deltaV = highV - lowV;
//            if (deltaV > 646) {
//                // charged for a RC time
//                break;
//            }
////            printMsg("delta V: %d", highV - lowV);
//
//        }
//        auto timeElapsedMs = micros() - curTime;
//        printMsg("timeElapsedMs: %d", timeElapsedMs);
//
//        // t = R * C = (HighR + LowR) * C
//        capacitance_nanoF_t c = static_cast<capacitance_nanoF_t>(timeElapsedMs * 1e3 /
//                                                                 (HIGH_RESISTANCE + HIGH_RESISTANCE));
//        printMsg("capacitance: %d", static_cast<int>(c));
//
//        printMsg("=====Discharging=====");
//        digitalWrite(pin1, LOW);
//
//        delay(8000);
//    }
//}

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

//    printItems("    Time elapsed: ", timeElapsedMs, " MicroSec\n");
//    printItems("    Total resistance: ", totalR, " kOhm\n");
    printItems("    Capacitance: ", c, " nF\n");
    printItems("<<< measureC()\n");
    return c;
}


#endif //FINALPROJECT_CAPACITOR_MEASURE_H
