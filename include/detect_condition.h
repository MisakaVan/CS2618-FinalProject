//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_DETECT_CONDITION_H
#define FINALPROJECT_DETECT_CONDITION_H

#include "constants.h"
#include "defines.h"
#include "utils.h"


/**
 * TODO: Detect whether the circuit is a transistor.
 * @param resultHolder If the circuit is a transistor, the result (1 type and 3 portIDs) will be stored in this variable.
 * @return Whether the circuit is a transistor.
 */
bool isTransistorOld(ConnectionState::StateData::t_field &resultHolder)
{
    int count = 0;
    Port resultPort[4];
    resultPort[1] = PORTS[resultHolder.portB];
    resultPort[2] = PORTS[resultHolder.portC];
    resultPort[3] = PORTS[resultHolder.portE];
    for (int i = 1; i <= 3; i++) {
        clearPorts();
        for (int j = 1; j <= 3; j++) {
            pinMode(resultPort[j].digitalPinLowR, OUTPUT);
            digitalWrite(resultPort[j].digitalPinLowR, LOW);
        }
        pinMode(resultPort[i].digitalPinLowR, OUTPUT);
        digitalWrite(resultPort[i].digitalPinLowR, HIGH);
        delay(1);
        if (getVoltageAtAnalogPin(resultPort[1]) >= 100 && getVoltageAtAnalogPin(resultPort[2]) >= 100 &&
            getVoltageAtAnalogPin(resultPort[3]) >= 100) {
            testTrasistorMeasure(resultPort[1], resultPort[2], resultPort[3], resultHolder);  // further measure
            return true;
        }
    }
    return false;
}

/**
 * TODO: Detect whether the circuit is a capacitor.
 * @param resultHolder If the circuit is a capacitor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a capacitor.
 */
bool isCapacitorOld(ConnectionState::StateData::c_field &resultHolder)
{
    Port port1 = PORTS[resultHolder.port1];
    Port port2 = PORTS[resultHolder.port2];
    capacitance_nanoF_t capacitance = measureCapacitance<CMeasureMode::LowR>(port1, port2);
    return capacitance > 1e-5;
}

/**
 * TODO: Detect whether the circuit is a resistor.
 * @param resultHolder If the circuit is a resistor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a resistor.
 */
bool isResistorOld(ConnectionState::StateData::r_field &resultHolder)
{
    int last = 0;
    clearPorts();
    Port port1 = PORTS[resultHolder.port1];
    Port port2 = PORTS[resultHolder.port2];
    dischargeCapacitor(port1, port2);
    clearPorts();
    pinMode(port1.digitalPinLowR, OUTPUT);
    digitalWrite(port1.digitalPinLowR, HIGH);
    pinMode(port2.digitalPinLowR, OUTPUT);
    digitalWrite(port2.digitalPinLowR, LOW);
    delay(200);
    return getVoltageAtAnalogPin(port2) > 2 || getVoltageAtAnalogPin(port1) < 1015;
}


/**
 * TODO: Detect the connection mode of the circuit.
 * @param resultHolder Changes in-place to store the result.
 * @return void
 */
void detectConnectionOld(ConnectionState &resultHolder)
{
    if (isTransistorOld(resultHolder.data.t)) {
        resultHolder.mode = ConnectionMode::T;
        return;
    }
    if (isResistorOld(resultHolder.data.r)) {
        resultHolder.mode = ConnectionMode::R;
        return;
    }
    if (isCapacitorOld(resultHolder.data.c)) {
        resultHolder.mode = ConnectionMode::C;
        return;
    }
    resultHolder.mode = ConnectionMode::Null;
}


/**
 * TODO: Detect whether the circuit is a transistor.
 * @param resultHolder If the circuit is a transistor, the result (1 type and 3 portIDs) will be stored in this variable.
 * @return Whether the circuit is a transistor.
 */
bool isTransistor(ConnectionState::StateData::t_field& resultHolder)
{
    int count = 0;
    Port resultPort[4];   // Port
    // auto port_a = PORTS[1];
    // auto port_b = PORTS[port2];
    resultPort[1] = PORTS[0]; // PORTS[1]; // [resultHolder.portB];
    resultPort[2] = PORTS[1]; // [resultHolder.portC];
    resultPort[3] = PORTS[2]; // [resultHolder.portE];
    for (int i = 1; i <= 3; i++)
    {
        clearPorts();
        for (int j = 1; j <= 3; j++){
            pinMode(resultPort[j].digitalPinLowR, OUTPUT);
            digitalWrite(resultPort[j].digitalPinLowR, LOW);
        }
        pinMode(resultPort[i].digitalPinLowR, OUTPUT);
        digitalWrite(resultPort[i].digitalPinLowR, HIGH);
        delay(1);
        if (getVoltageAtAnalogPin(resultPort[1]) >= 0.5 && getVoltageAtAnalogPin(resultPort[2]) >= 0.5 && getVoltageAtAnalogPin(resultPort[3]) >= 0.5){
            testTrasistorMeasure(resultPort[1], resultPort[2], resultPort[3], resultHolder);  // further measure
            return true;
        }
    }
    return false;
}

/**
 * TODO: Detect whether the circuit is a capacitor.
 * @param resultHolder If the circuit is a capacitor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a capacitor.
 */
bool isCapacitor(ConnectionState::StateData::c_field& resultHolder)
{
    for (int i = 0; i<3; i++){
        int port1 = i;
        int port2 = (i+1)%3;
        auto port_a = PORTS[port1];
        auto port_b = PORTS[port2];
        clearPorts();
        // Port port1 = PORTS[resultHolder.port1];
        // Port port2 = PORTS[resultHolder.port2];
        Serial.println("bbb");
        capacitance_nanoF_t capacitance = measureCapacitance<CMeasureMode::LowR>(port_a, port_b);  // to test

        if (capacitance > 1e-5){
            resultHolder.port1 = port1;
            resultHolder.port2 = port2;
            return true;
        }
    }
    return false;
}

/**
 * TODO: Detect whether the circuit is a resistor.
 * @param resultHolder If the circuit is a resistor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a resistor.
 */
bool isResistor(ConnectionState::StateData::r_field& resultHolder)
{
    int last = 0;
    // Port port1 = PORTS[resultHolder.port1];
    // Port port2 = PORTS[resultHolder.port2];
    for (int i = 0; i<3; i++){
        int port1 = i;
        int port2 = (i+1)%3;
        auto port_a = PORTS[port1];
        auto port_b = PORTS[port2];
        clearPorts();
        dischargeCapacitor(port_a, port_b);
        clearPorts();
        pinMode(port_a.digitalPinLowR, OUTPUT);
        digitalWrite(port_a.digitalPinLowR, HIGH);
        pinMode(port_b.digitalPinLowR, OUTPUT);
        digitalWrite(port_b.digitalPinLowR, LOW);
        delay(200);
        // Serial.println(getVoltageAtAnalogPin(port_b));
        // Serial.println(getVoltageAtAnalogPin(port_a));
        if (getVoltageAtAnalogPin(port_b) > 0 || getVoltageAtAnalogPin(port_a) < 5){
            resultHolder.port1 = port1;
            resultHolder.port2 = port2;
            return true;
        }
    }
    return false;
}


/**
 * TODO: Detect the connection mode of the circuit.
 * @param resultHolder Changes in-place to store the result.
 * @return void
 */
void detectConnection(ConnectionState& resultHolder)
{
    // Serial.println("here");
    if (isTransistor(resultHolder.data.t)){
        resultHolder.mode = ConnectionMode::T;
        return;
    }
    if (isResistor(resultHolder.data.r)) {
        resultHolder.mode = ConnectionMode::R;
        return;
    }
        // if (isCapacitor(resultHolder.data.c)) {
        //       Serial.println("ddd");
        //   resultHolder.mode = ConnectionMode::C;
        //   return;
        // }
    else resultHolder.mode = ConnectionMode::Null;

}

#endif //FINALPROJECT_DETECT_CONDITION_H
