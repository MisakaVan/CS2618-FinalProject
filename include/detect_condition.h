//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_DETECT_CONDITION_H
#define FINALPROJECT_DETECT_CONDITION_H

#include "constants.h"
#include "defines.h"
#include "utils.h"


/**
 *  Detect whether the circuit is a transistor.
 * @param resultHolder If the circuit is a transistor, the result (1 type and 3 portIDs) will be stored in this variable.
 * @return Whether the circuit is a transistor.
 */
bool isTransistor(ConnectionState::StateData::t_field &resultHolder)
{
    Port resultPort[4];   // Port
    resultPort[1] = PORTS[0]; // PORTS[1]; // [resultHolder.portB];
    resultPort[2] = PORTS[1]; // [resultHolder.portC];
    resultPort[3] = PORTS[2]; // [resultHolder.portE];
    for (int i = 1; i <= 3; i++) {
        clearPorts();
        for (int j = 1; j <= 3; j++) {
            pinMode(resultPort[j].digitalPinLowR, OUTPUT);
            digitalWrite(resultPort[j].digitalPinLowR, LOW);
        }
        pinMode(resultPort[i].digitalPinLowR, OUTPUT);
        digitalWrite(resultPort[i].digitalPinLowR, HIGH);
        delay(1);
        if (getVoltageAtAnalogPin(resultPort[1]) >= 0.5 && getVoltageAtAnalogPin(resultPort[2]) >= 0.5 &&
            getVoltageAtAnalogPin(resultPort[3]) >= 0.5) {
            testTrasistorMeasure(resultPort[1], resultPort[2], resultPort[3], resultHolder);  // further measure
            return true;
        }
    }
    return false;
}

/**
 * Detect whether the circuit is a capacitor.
 * @param resultHolder If the circuit is a capacitor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a capacitor.
 */
bool isCapacitor(ConnectionState::StateData::c_field &resultHolder)
{
    for (int i = 0; i < 3; i++) {
        int port1 = i;
        int port2 = (i + 1) % 3;
        const auto &port_a = PORTS[port1];
        const auto &port_b = PORTS[port2];
        clearPorts();
        capacitance_nanoF_t capacitance = measureCapacitancePrototype<
                CMeasureMode::LowR, CMeasureMode::HighR>(port_a,
                                                         port_b);  // use a high R and a low R for faster measurement


        if (capacitance > IS_C_LOWER_TH) {
            resultHolder.port1 = port1;
            resultHolder.port2 = port2;
            return true;
        }
    }
    return false;
}

/**
 * Detect whether the circuit is a resistor.
 * @param resultHolder If the circuit is a resistor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a resistor.
 */
bool isResistor(ConnectionState::StateData::r_field &resultHolder)
{
    for (int i = 0; i < 3; i++) {
        int port1 = i;
        int port2 = (i + 1) % 3;
        const auto &port_a = PORTS[port1];
        const auto &port_b = PORTS[port2];
        clearPorts();
        dischargeCapacitor(port_a, port_b);
        clearPorts();
        pinMode(port_a.digitalPinLowR, OUTPUT);
        digitalWrite(port_a.digitalPinLowR, HIGH);
        pinMode(port_b.digitalPinLowR, OUTPUT);
        digitalWrite(port_b.digitalPinLowR, LOW);

        // if it is a capacitor, the v on port_a will increase
        // (maybe very slow) eventually to 1023.
        int prevValue, curValue = 0;
        do {
            prevValue = curValue;
            delay(1000);
            curValue = analogRead(port_a.analogPin);
            printItems("<!> ", curValue, '\n');
        } while (curValue > prevValue && curValue < 1023);

        // Serial.println(getVoltageAtAnalogPin(port_b));
        // Serial.println(getVoltageAtAnalogPin(port_a));
        if (analogRead(port_b.analogPin) > 1 || curValue < 1022) {
            // if the r is too large, consider it as a capacitor
            const auto r = measureResistance<RMeasureMode::HighR>(port_a, port_b);
            if (r > IS_R_UPPER_TH) {
                return false;
            }

            resultHolder.port1 = port1;
            resultHolder.port2 = port2;
            return true;
        }
    }
    return false;
}


/**
 * Detect the connection mode of the circuit.
 * @param resultHolder Changes in-place to store the result.
 * @return void
 */
void detectConnection(ConnectionState &resultHolder)
{
    printItems("    Trying T...\n");
    if (isTransistor(resultHolder.data.t)) {
        resultHolder.mode = ConnectionMode::T;
        return;
    }
    printItems("    Trying R...\n");
    if (isResistor(resultHolder.data.r)) {
        resultHolder.mode = ConnectionMode::R;
        return;
    }
    printItems("    Trying C...\n");
    if (isCapacitor(resultHolder.data.c)) {

        resultHolder.mode = ConnectionMode::C;
        return;
    }
    resultHolder.mode = ConnectionMode::Null;
}

#endif //FINALPROJECT_DETECT_CONDITION_H
