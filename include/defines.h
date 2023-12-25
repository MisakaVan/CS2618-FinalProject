//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_DEFINES_H
#define FINALPROJECT_DEFINES_H

#include <stdint.h> // NOLINT(*-deprecated-headers)

using ull_int = unsigned long long int;

using resistance_ohm_t = unsigned long;
using resistance_kOhm_t = double;
using resistance_MOhm_t = double;
using capacitance_nanoF_t = double;
using voltage_V_t = double;
using voltage_mV_t = double;
using current_A_t = double;
using current_mA_t = double;
using current_uA_t = double;
using beta_t = double;
using pinID_t = uint8_t;
using portID_t = uint8_t;


enum class RMeasureMode : uint8_t {
    LowR = 0,
    HighR = 1
};

enum class CMeasureMode : uint8_t {
    LowR = 0,
    HighR = 1
};

enum class TransistorType : uint8_t {
    PNP = 0,
    NPN = 1
};

enum class ConnectionMode : uint8_t {
    Null = 0,
    R,
    C,
    T
};

// A port contains 3 physical pins:
// 1. An analog pin, directly connected (no resistor) to the outside.
// 2. A digital pin, connected to the outside via a high-resistance resistor.
// 3. A digital pin, connected to the outside via a low-resistance resistor.
struct Port {
    pinID_t analogPin;
    pinID_t digitalPinHighR;
    pinID_t digitalPinLowR;

    Port() {}  //

    constexpr Port(pinID_t analogPin, pinID_t digitalPinHighR, pinID_t digitalPinLowR) :
            analogPin(analogPin), digitalPinHighR(digitalPinHighR), digitalPinLowR(digitalPinLowR) {}

    // Set all pins to input mode.
    void clear() const
    {
        pinMode(analogPin, INPUT);
        pinMode(digitalPinHighR, INPUT);
        pinMode(digitalPinLowR, INPUT);
    }

};


struct ConnectionState {
    ConnectionMode mode;
    union StateData {
        struct r_field {
            portID_t port1;
            portID_t port2;
        } r;
        struct c_field {
            portID_t port1;
            portID_t port2;
        } c;
        struct t_field {
            TransistorType type;
            beta_t beta;  //
            portID_t portB;
            portID_t portC;
            portID_t portE;
        } t;
    } data;
};


#endif //FINALPROJECT_DEFINES_H
