//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_CONSTANTS_H
#define FINALPROJECT_CONSTANTS_H

#include <Arduino.h>
#include <math.h> // NOLINT(*-deprecated-headers)
#include "defines.h"

// The high/low resistance of the resistor in the circuit.
constexpr resistance_ohm_t HIGH_RESISTANCE = 200000;
constexpr resistance_ohm_t LOW_RESISTANCE = 680;
// sqrt(HIGH_RESISTANCE * LOW_RESISTANCE)


// The voltage of the power supply.
constexpr voltage_V_t VCC = 5.0;

constexpr portID_t PORT_COUNT = 3;

// There are 3 ports in the circuit.
constexpr Port PORTS[PORT_COUNT] = {
        Port(
                A5,
                5,
                6
        ),
        Port(
                A6,
                9,
                10
        ),
        Port(
                A7,
                11,
                12
        )
};

// pins for the part 2
constexpr pinID_t
        pin_B = A7,
        pin_B_out = A5,
        pin_C = A6,
        pin_C_out = A4,
        pin_B_up = 10,
        pin_B_clk = 9,
        pin_C_up = 6,
        pin_C_clk = 5;

const resistance_MOhm_t R1 = 0.2;
const resistance_kOhm_t R2 = 0.68;




#endif //FINALPROJECT_CONSTANTS_H
