//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_DETECT_CONDITION_H
#define FINALPROJECT_DETECT_CONDITION_H

#include "constants.h"
#include "defines.h"



/**
 * TODO: Detect whether the circuit is a transistor.
 * @param resultHolder If the circuit is a transistor, the result (1 type and 3 portIDs) will be stored in this variable.
 * @return Whether the circuit is a transistor.
 */
bool isTransistor(ConnectionState::StateData::r_field& resultHolder);

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
