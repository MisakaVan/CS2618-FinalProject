//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_TRANSISTOR_MEASURE_H
#define FINALPROJECT_TRANSISTOR_MEASURE_H

#include "constants.h"
#include "utils.h"
#include "defines.h"


voltage_V_t Result[4]{};
Port resultPort[4];
const TransistorType PNP = TransistorType::PNP, NPN = TransistorType::NPN;

// transistor feature
bool isFeature()
{
    if (Result[1] >= 4.9 && Result[2] <= 0.05 && Result[3] <= 0.05)
        return true;
    if (Result[1] <= 0.05 && Result[2] >= 4.9 && Result[3] <= 0.05)
        return true;
    if (Result[1] <= 0.05 && Result[2] <= 0.05 && Result[3] >= 4.9)
        return true;
    return false;
}


// calculate PNP transistor factor
void calculateTransistorPNP(const Port &base, const Port &collector, const Port &emitter,
                            ConnectionState::StateData::t_field &resultHolder)
{
    clearPorts();
    pinMode(base.digitalPinHighR, OUTPUT);
    digitalWrite(base.digitalPinHighR, LOW);
    pinMode(collector.digitalPinLowR, OUTPUT);
    digitalWrite(collector.digitalPinLowR, LOW);
    pinMode(emitter.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(emitter.digitalPinLowR, HIGH);
    delay(10);
    double b = getVoltageAtAnalogPin(base);
    double e = getVoltageAtAnalogPin(emitter);
    double c = getVoltageAtAnalogPin(collector);

    Serial.println(b);
    Serial.println(e);
    Serial.println(c);
    current_A_t ib = (100000.0 * b) / double(HIGH_RESISTANCE);
    Serial.println(ib);
    current_A_t ic = (100000.0 * c) / double(LOW_RESISTANCE);
    Serial.println(ic);
    resultHolder.beta = ic / ib;
}


// analyse PNP transistor
void analyseTransistorPNP(int x, int y, int z, ConnectionState::StateData::t_field &resultHolder)
{
    resultHolder.portB = x;  // Base

    Port portx = resultPort[x];
    Port porty = resultPort[y];
    Port portz = resultPort[z];

    // high on portx, high on porty, low on portz
    clearPorts();
    pinMode(portx.digitalPinHighR, OUTPUT);
    digitalWrite(portx.digitalPinHighR, LOW);
    pinMode(porty.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(porty.digitalPinLowR, HIGH);
    pinMode(portz.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(portz.digitalPinLowR, LOW);
    delay(1);
    Result[y] = getVoltageAtAnalogPin(porty);


    clearPorts();
    pinMode(portx.digitalPinHighR, OUTPUT);
    digitalWrite(portx.digitalPinHighR, LOW);
    pinMode(porty.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(porty.digitalPinLowR, LOW);
    pinMode(portz.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(portz.digitalPinLowR, HIGH);

    delay(1);
    Result[z] = getVoltageAtAnalogPin(portz);

    resultHolder.portC = Result[y] > Result[z] ? y : z;
    resultHolder.portE = Result[y] > Result[z] ? z : y;
    calculateTransistorPNP(resultPort[x], resultPort[resultHolder.portC], resultPort[resultHolder.portE], resultHolder);

}


// calculate NPN transistor factor
void calculateTransistorNPN(const Port &base, const Port &collector, const Port &emitter,
                            ConnectionState::StateData::t_field &resultHolder)
{
    clearPorts();
    pinMode(base.digitalPinHighR, OUTPUT);
    digitalWrite(base.digitalPinHighR, HIGH);
    pinMode(collector.digitalPinLowR, OUTPUT);
    digitalWrite(collector.digitalPinLowR, HIGH);
    pinMode(emitter.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(emitter.digitalPinLowR, LOW);
    delay(10);
    Serial.println(getVoltageAtAnalogPin(base));
    Serial.println(getVoltageAtAnalogPin(emitter));
    Serial.println(getVoltageAtAnalogPin(collector));
    current_A_t ib = (5000000 - 1000000 * getVoltageAtAnalogPin(base)) / double(HIGH_RESISTANCE);
    Serial.println(ib);
    current_A_t ic = (5000000 - 1000000 * getVoltageAtAnalogPin(collector)) / double(LOW_RESISTANCE);
    Serial.println(ic);
    resultHolder.beta = ic / ib;
}

// analyse NPN transistor
void analyseTransistorNPN(int x, int y, int z, ConnectionState::StateData::t_field &resultHolder)
{
    resultHolder.portB = x;  // Base

    Port portx = resultPort[x];
    Port porty = resultPort[y];
    Port portz = resultPort[z];

    // high on portx, high on porty, low on portz
    clearPorts();
    pinMode(portx.digitalPinHighR, OUTPUT);
    digitalWrite(portx.digitalPinHighR, HIGH);
    pinMode(porty.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(porty.digitalPinLowR, HIGH);
    pinMode(portz.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(portz.digitalPinLowR, LOW);
    delay(1);
    Result[y] = getVoltageAtAnalogPin(porty);


    clearPorts();
    pinMode(portx.digitalPinHighR, OUTPUT);
    digitalWrite(portx.digitalPinHighR, HIGH);
    pinMode(porty.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(porty.digitalPinLowR, LOW);
    pinMode(portz.digitalPinLowR, OUTPUT);   // only measure
    digitalWrite(portz.digitalPinLowR, HIGH);

    delay(1);
    Result[z] = getVoltageAtAnalogPin(portz);

    resultHolder.portE = Result[y] > Result[z] ? y : z;
    resultHolder.portC = Result[y] > Result[z] ? z : y;
    calculateTransistorNPN(resultPort[x], resultPort[resultHolder.portC], resultPort[resultHolder.portE], resultHolder);

}


// main function to test transistor
void testTrasistorMeasure(const Port &port1, const Port &port2, const Port &port3,
                          ConnectionState::StateData::t_field &resultHolder)
{
    int count = 0;
    bool h[4] = {false, false, false, false};

    for (int i = 1; i <= 3; i++) {
        clearPorts();
        resultPort[1] = port1;
        resultPort[2] = port2;
        resultPort[3] = port3;
        for (int j = 1; j <= 3; j++) {
            pinMode(resultPort[j].digitalPinLowR, OUTPUT);
            digitalWrite(resultPort[j].digitalPinLowR, LOW);
        }
        pinMode(resultPort[i].digitalPinLowR, OUTPUT);
        digitalWrite(resultPort[i].digitalPinLowR, HIGH);
        delay(1);

        Result[1] = getVoltageAtAnalogPin(resultPort[1]);
        Result[2] = getVoltageAtAnalogPin(resultPort[2]);
        Result[3] = getVoltageAtAnalogPin(resultPort[3]);

        if (isFeature()) {
            count++;
            h[i] = true;
        }
    }
    Serial.println(count);
    if (count == 1)  // PNP
    {
        int i;
        for (i = 1; i <= 3; ++i) {
            if (h[i]) break;
        }
        resultHolder.type = PNP;
        switch (i) {
            case 1:
                analyseTransistorPNP(1, 2, 3, resultHolder);
                break;
            case 2:
                analyseTransistorPNP(2, 1, 3, resultHolder);
                break;
            case 3:
                analyseTransistorPNP(3, 2, 1, resultHolder);
                break;
        }
    } else if (count == 2)  // NPN
    {
        int i;
        for (i = 1; i <= 3; ++i) {
            if (!h[i]) break;
        }
        resultHolder.type = NPN;
        switch (i) {
            case 1:
                analyseTransistorNPN(1, 2, 3, resultHolder);
                break;
            case 2:
                analyseTransistorNPN(2, 1, 3, resultHolder);
                break;
            case 3:
                analyseTransistorNPN(3, 2, 1, resultHolder);
                break;
        }
    }

}


#endif //FINALPROJECT_TRANSISTOR_MEASURE_H
