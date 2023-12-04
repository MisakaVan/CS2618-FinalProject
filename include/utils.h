//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_UTILS_H
#define FINALPROJECT_UTILS_H

#include <Arduino.h>
#include "defines.h"
#include "constants.h"

// reset all ports/pins to input mode
void clearPorts()
{
    for (auto port: PORTS) {
        port.clear();
    }
}

// Measure the voltage at the analog pin of the given _port.
// Assume the analog pin is already set to input mode.
voltage_t getVoltageAtAnalogPin(const Port &_port)
{
    return static_cast<double>(analogRead(_port.analogPin)) / 1023.0 * VCC;
}

// Output buffer
char outputBuff[256]{};

void printMsg(const char *msg)
{
    Serial.println(msg);
}

// Print formatted message to Serial.
template <typename... Args>
void printMsg(const char *format, Args... args)
{
    // maybe we can use forward here?
    sprintf(outputBuff, format, forward<Args>(args)...);
    Serial.println(outputBuff);
}

void printMsg(long long int x)
{
    printMsg("%lld", x);
}

// if R * R > R_HIGH * R_LOW we consider it above threshold.
constexpr bool isResistorAboveThreshold(resistance_t resistance)
{
    // notice overflow, so we use division instead of multiplication

    return resistance > HIGH_RESISTANCE || (HIGH_RESISTANCE / resistance < resistance / LOW_RESISTANCE);
}

// Arduino Nano
void setPinAsClkOutput(){


}

#endif //FINALPROJECT_UTILS_H
