//
// Created on 2023/11/27.
//

#ifndef FINALPROJECT_UTILS_H
#define FINALPROJECT_UTILS_H

#include <Arduino.h>
#include "defines.h"
#include "constants.h"
#include "semantics.h"

// reset all ports/pins to input mode
void clearPorts()
{
    for (auto port: PORTS) {
        port.clear();
    }
}

// Measure the voltage at the analog pin of the given _port.
// Assume the analog pin is already set to input mode.
voltage_V_t getVoltageAtAnalogPin(const Port &_port)
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

template <typename T>
void printItems(T arg)
{
    Serial.print(forward<T>(arg));
}

template <typename firstArg, typename ...Args>
void printItems(firstArg arg, Args... args)
{
    Serial.print(forward<firstArg>(arg));
    printItems(forward<Args>(args)...);
}

template <typename T>
void printItemsToDisplay(Adafruit_PCD8544 &display, T arg)
{
    display.print(forward<T>(arg));
}

template <typename firstArg, typename ...Args>
void printItemsToDisplay(Adafruit_PCD8544 &display, firstArg arg, Args... args)
{
    display.print(forward<firstArg>(arg));
    printItemsToDisplay(display, forward<Args>(args)...);
}



#endif //FINALPROJECT_UTILS_H
