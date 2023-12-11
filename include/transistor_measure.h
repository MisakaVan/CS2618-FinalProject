//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_TRANSISTOR_MEASURE_H
#define FINALPROJECT_TRANSISTOR_MEASURE_H

#include "constants.h"
#include "utils.h"
#include "defines.h"


voltage_t Result[4];
Port resultPort[4];
TransistorType PNP, NPN;

// transistor feature
bool isFeature()
{
    if (Result[1] >= 1000 && Result[2] <= 10 && Result[3] <= 10)
        return true;
    if (Result[1] <= 10 && Result[2] >= 1000 && Result[3] <= 10)
        return true;
    if (Result[1] <= 10 && Result[2] <= 10 && Result[3] >= 1000)
        return true;
    return false;
}




// calculate PNP transistor factor
void calculateTransistorPNP(const Port &base, const Port &collector, const Port &emitter, ConnectionState::StateData::t_field& resultHolder)
{
    clearPorts();
    // low on portB and portC, high on portE
    pinMode(base.digitalPinLowR, OUTPUT);   // LowR
    digitalWrite(base.digitalPinLowR, LOW);
    pinMode(collector.analogPin, OUTPUT);   // only measure
    digitalWrite(collector.analogPin, LOW);
    pinMode(emitter.digitalPinLowR, OUTPUT);
    digitalWrite(emitter.digitalPinLowR, HIGH);
    delay(1);

    Result[0] = getVoltageAtAnalogPin(base);
    Result[1] = getVoltageAtAnalogPin(emitter);

    if (Result[0] >= 2)
        resultHolder.beta = (Result[1] - Result[0]) / Result[0];
    else
    {
        clearPorts();
        pinMode(base.digitalPinHighR, OUTPUT);   // HighR
        digitalWrite(base.digitalPinHighR, LOW);
        pinMode(collector.analogPin, OUTPUT);   // only measure
        digitalWrite(collector.analogPin, LOW);
        pinMode(emitter.digitalPinLowR, OUTPUT);
        digitalWrite(emitter.digitalPinLowR, HIGH);
        delay(1);

        Result[0] = getVoltageAtAnalogPin(base);
        Result[1] = getVoltageAtAnalogPin(emitter);
        resultHolder.beta = (Result[1] * HIGH_RESISTANCE) / (Result[0] * LOW_RESISTANCE);
    }
}


// analyse PNP transistor
void analyseTransistorPNP(int x, int y, int z, ConnectionState::StateData::t_field& resultHolder)
{
    Port portx = resultPort[x];
    Port porty = resultPort[y];
    Port portz = resultPort[z];
    resultHolder.portB = x;  // Base
    
    // try: high on TPx, low on TPy and TPz
    clearPorts();
    pinMode(portx.digitalPinHighR, OUTPUT);
    digitalWrite(portx.digitalPinHighR, HIGH);
    pinMode(porty.digitalPinHighR, OUTPUT);
    digitalWrite(porty.digitalPinHighR, LOW);
    pinMode(portz.digitalPinHighR, OUTPUT);
    digitalWrite(portz.digitalPinHighR, LOW);
    delay(10);

    Result[1] = getVoltageAtAnalogPin(portx);
    Result[2] = getVoltageAtAnalogPin(porty);
    Result[3] = getVoltageAtAnalogPin(portz);

    if (Result[y] >= 100)
    {
        resultHolder.portC = y;  // Collector
        resultHolder.portE = z;  // Emitter
        calculateTransistorPNP(resultPort[x], resultPort[y], resultPort[z], resultHolder);
    }
    else
    {
        resultHolder.portC = z;  // Collector
        resultHolder.portE = y;  // Emitter
        calculateTransistorPNP(resultPort[x], resultPort[z], resultPort[y], resultHolder);
    }
}


// calculate NPN transistor factor
void calculateTransistorNPN(const Port &base, const Port &collector, const Port &emitter, ConnectionState::StateData::t_field& resultHolder)
{
    clearPorts();
    pinMode(base.digitalPinHighR, OUTPUT);
    digitalWrite(base.digitalPinHighR, HIGH);
    pinMode(collector.digitalPinLowR, OUTPUT);
    digitalWrite(collector.digitalPinLowR, HIGH);
    pinMode(emitter.analogPin, OUTPUT);   // only measure
    digitalWrite(emitter.analogPin, LOW);
    delay(10);
    current_t ib = (1023 - getVoltageAtAnalogPin(base)) * 5 / (1023 * HIGH_RESISTANCE);
    current_t ic = (1023 - getVoltageAtAnalogPin(collector)) * 5 / (1023 * LOW_RESISTANCE);
    resultHolder.beta = ic / ib;
}

// analyse NPN transistor
void analyseTransistorNPN(int x, int y, int z, ConnectionState::StateData::t_field& resultHolder)
{
    resultHolder.portB = x;  // Base

    Port portx = resultPort[x];
    Port porty = resultPort[y];
    Port portz = resultPort[z];

    // high on portx, high on porty, low on portz
    clearPorts();
    pinMode(portx.digitalPinLowR, OUTPUT);
    digitalWrite(portx.digitalPinLowR, HIGH);
    pinMode(porty.analogPin, OUTPUT);   // only measure
    digitalWrite(porty.analogPin, HIGH);
    pinMode(portz.analogPin, OUTPUT);   // only measure
    digitalWrite(portz.analogPin, LOW);
    delay(1);
    Result[y] = getVoltageAtAnalogPin(portx);

    // high on portx, low on porty, high on portz
    clearPorts();
    pinMode(portx.digitalPinLowR, OUTPUT);
    digitalWrite(portx.digitalPinLowR, HIGH);
    pinMode(porty.analogPin, OUTPUT);   // only measure
    digitalWrite(porty.analogPin, LOW);
    pinMode(portz.analogPin, OUTPUT);   // only measure
    digitalWrite(portz.analogPin, HIGH);
    delay(1);
    Result[z] = getVoltageAtAnalogPin(portx);

    if (Result[y] > Result[z])
    {
        resultHolder.portC = y;  // Collector
        resultHolder.portE = z;  // Emitter
        calculateTransistorNPN(resultPort[x], resultPort[y], resultPort[z], resultHolder);
    }
    else
    {
        resultHolder.portC = z;  // Collector
        resultHolder.portE = y;  // Emitter
        calculateTransistorNPN(resultPort[x], resultPort[z], resultPort[y], resultHolder);
    }
}




// main function to test trasistor
void testTrasistorMeasure(const Port &port1, const Port &port2, const Port &port3, ConnectionState::StateData::t_field& resultHolder)
{
    bool feature[4];
    int count = 0;
    for (int i = 1; i <= 3; i++)
    {
        clearPorts();
        resultPort[1] = port1;
        resultPort[2] = port2;
        resultPort[3] = port3;
        for (int j = 1; j <= 3; j++){
            pinMode(resultPort[j].digitalPinLowR, OUTPUT);
            digitalWrite(resultPort[j].digitalPinLowR, LOW);
        }
        pinMode(resultPort[i].digitalPinLowR, OUTPUT);
        digitalWrite(resultPort[i].digitalPinLowR, HIGH);
        delay(1);

        Result[1] = getVoltageAtAnalogPin(resultPort[1]);
        Result[2] = getVoltageAtAnalogPin(resultPort[2]);
        Result[3] = getVoltageAtAnalogPin(resultPort[3]);

        if (isFeature())
            count++;
    }
    if (count == 1)  // PNP
    {
        resultHolder.type = PNP;
        if (feature[1])
            analyseTransistorPNP(1, 2, 3, resultHolder);
        else if (feature[2])
            analyseTransistorPNP(2, 1, 3, resultHolder);
        else
            analyseTransistorPNP(3, 1, 2, resultHolder);
    }
    else if (count == 2)  // NPN
    {
        resultHolder.type = NPN;
        if (!feature[1])
            analyseTransistorNPN(1, 2, 3, resultHolder);
        else if (!feature[2])
            analyseTransistorNPN(2, 1, 3, resultHolder);
        else if (!feature[3])
            analyseTransistorNPN(3, 1, 2, resultHolder);
    }
}


#endif //FINALPROJECT_TRANSISTOR_MEASURE_H
