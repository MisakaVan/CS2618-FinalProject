//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_CONSTANTS_H
#define FINALPROJECT_CONSTANTS_H

#include <Arduino.h>
#include <math.h> // NOLINT(*-deprecated-headers)
#include "defines.h"

// The high/low resistance of the resistor in the circuit.
constexpr resistance_ohm_t HIGH_RESISTANCE = 100000;
constexpr resistance_ohm_t LOW_RESISTANCE = 500;
// sqrt(HIGH_RESISTANCE * LOW_RESISTANCE)


// The voltage of the power supply.
constexpr voltage_t VCC = 5.0;

constexpr portID_t PORT_COUNT = 3;

// There are 3 ports in the circuit.
constexpr Port PORTS[PORT_COUNT] = {
        Port(A3, 2, 3),
        Port(A2, 4, 5),
        Port(A1, 6, 7)
};

#endif //FINALPROJECT_CONSTANTS_H
