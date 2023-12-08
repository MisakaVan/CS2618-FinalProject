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
bool isTransistor(ConnectionState::StateData::t_field& resultHolder)
{
    int count = 0;
    Port resultPort[4];
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
        if (getVoltageAtAnalogPin(resultPort[1]) >= 100 && getVoltageAtAnalogPin(resultPort[2]) >= 100 && getVoltageAtAnalogPin(resultPort[3]) >= 100){
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
bool isCapacitor(ConnectionState::StateData::c_field& resultHolder);

/**
 * TODO: Detect whether the circuit is a resistor.
 * @param resultHolder If the circuit is a resistor, the result (2 portIDs) will be stored in this variable.
 * @return Whether the circuit is a resistor.
 */
bool isResistor(ConnectionState::StateData::r_field& resultHolder);


/**
 * TODO: Detect the connection mode of the circuit.
 * @param resultHolder Changes in-place to store the result.
 * @return void
 */
void detectConnection(ConnectionState& resultHolder);


#endif //FINALPROJECT_DETECT_CONDITION_H
