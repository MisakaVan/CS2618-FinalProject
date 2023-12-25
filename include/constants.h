//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_CONSTANTS_H
#define FINALPROJECT_CONSTANTS_H

#include <Arduino.h>
#include <math.h> // NOLINT(*-deprecated-headers)
#include "defines.h"

// The high/low resistance of the resistor in the circuit.

constexpr resistance_ohm_t HIGH_RESISTANCE = 510000;
constexpr resistance_kOhm_t HIGH_RESISTANCE_K = HIGH_RESISTANCE / 1000.0;
// Note! 470k is the actual value, but we failed to notice this error in this header
// until the current code review, which seems too late QwQ (2023/12/26)

constexpr resistance_ohm_t LOW_RESISTANCE = 680;
constexpr resistance_kOhm_t LOW_RESISTANCE_K = LOW_RESISTANCE / 1000.0;

const capacitance_nanoF_t IS_C_LOWER_TH = 0.5;
const resistance_ohm_t IS_R_UPPER_TH = 2000000;


// The voltage of the power supply.
constexpr voltage_V_t VCC = 5.0;

constexpr portID_t PORT_COUNT = 3;

// There are 3 ports in the circuit.
constexpr Port PORTS[PORT_COUNT] = {
        Port(
                A0,
                5,
                6
        ),
        Port(
                A4,
                9,
                10
        ),
        Port(
                A5,
                11,
                12
        )
};

// constants for the part 2
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


// constants for display
constexpr pinID_t SCLK = 8,
        DIN = 7,
        DC = 17,
        CS = 16,
        RST = 15;

#endif //FINALPROJECT_CONSTANTS_H
