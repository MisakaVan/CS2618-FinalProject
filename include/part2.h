//
// Created on 2023/12/16.
//

#ifndef FINALPROJECT_PART2_H
#define FINALPROJECT_PART2_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "constants.h"
#include "utils.h"

// int B_control=6;
// int B_clk=5;

// int C_control=10;
// int C_clk=9;


voltage_V_t resultUce[4][8]{};
current_mA_t resultIc[4][8]{};
current_uA_t resultIb[4][8]{};

voltage_V_t maxRes_V = 0;
current_mA_t maxRes_mA = 0;

//Adafruit_PCD8544 display2 = Adafruit_PCD8544(8, 7, 17, 16, 15);

voltage_V_t curUb()
{
    double x1 = analogRead(pin_B) * 5.0 / 1023.0;
    delay(5);
    double x2 = analogRead(pin_B) * 5.0 / 1023.0;
    delay(5);
    double x3 = analogRead(pin_B) * 5.0 / 1023.0;
    delay(5);
    double x = (x1 + x2 + x3) / 3.0;
    return x;
}

voltage_V_t curUc()
{
    double x1 = analogRead(pin_C) * 5.0 / 1023.0;
    delay(5);
    double x2 = analogRead(pin_C) * 5.0 / 1023.0;
    delay(5);
    double x3 = analogRead(pin_C) * 5.0 / 1023.0;
    delay(5);
    double x = (x1 + x2 + x3) / 3.0;
    return x;
}

current_uA_t curIb()//微安
{
    double x = analogRead(pin_B_out) * 5.0 / 1023.0;
    double y = analogRead(pin_B) * 5.0 / 1023.0;
    return (x - y) / R1;
}

current_mA_t curIc()//毫安
{
    double x = analogRead(pin_C_out) * 5.0 / 1023.0;
    double y = analogRead(pin_C) * 5.0 / 1023.0;
    return (x - y) / R2;
}

void clearPotentiometer()
{
    digitalWrite(pin_B_up, LOW);
    digitalWrite(pin_C_up, LOW);
    digitalWrite(pin_B_clk, HIGH);
    digitalWrite(pin_C_clk, HIGH);
    for (int i = 0; i < 150; i++) {
        digitalWrite(pin_B_clk, LOW);
        digitalWrite(pin_C_clk, LOW);
        delay(10);
        digitalWrite(pin_B_clk, HIGH);
        digitalWrite(pin_C_clk, HIGH);
        delay(10);
    }

    printItems("cleared both potentiometers.\n");

}

// templatize the above 4 functions
enum class Ptt {
    B, C
};

enum class Dir {
    Inc, Dec
};


/**
 * @param count the number of steps to move
 * @param interval the half T of clk pulse. default to 20ms.
 */
template <Ptt potentiometer, Dir direction, int interval = 5>
void movePtt(int count)
{
    const auto pin_up = potentiometer == Ptt::B ? pin_B_up : pin_C_up;
    const auto pin_clk = potentiometer == Ptt::B ? pin_B_clk : pin_C_clk;
    const auto value_up = direction == Dir::Inc ? HIGH : LOW;
    digitalWrite(pin_clk, HIGH);
    for (int t = 0; t < count; ++t) {
        digitalWrite(pin_up, value_up);
        digitalWrite(pin_clk, LOW);
        delay(interval);
        digitalWrite(pin_clk, HIGH);
        delay(interval);
    }
}

void printAllCurState()
{
    printItems("all\n");
    printItems("ub:", curUb(), "\n");
    printItems("uc:", curUc(), "\n");
    printItems("ib:", curIb(), "\n");
    printItems("ic:", curIc(), "\n");
    printItems("\n");
}

void setupPart2(Adafruit_PCD8544 &display2)
{
    Serial.begin(9600);
    display2.begin();
    display2.setContrast(23);
    display2.clearDisplay();
    pinMode(pin_B_up, OUTPUT);
    pinMode(pin_B_clk, OUTPUT);
    pinMode(pin_C_up, OUTPUT);
    pinMode(pin_C_clk, OUTPUT);

    // fill resultUce and resultIc with 0
    for (int i = 0; i < 4; ++i) {
        for (int t = 0; t < 8; ++t) {
            resultUce[i][t] = 0;
            resultIc[i][t] = 0;
        }
    }
    maxRes_mA = 0;
    maxRes_V = 0;

}

void mainPart2_NPN(Adafruit_PCD8544 &display2,
                   current_uA_t IbDelta = 0.01)
{
    display2.display();
    for (int curveIdx = 0; curveIdx < 4; ++curveIdx) {
        clearPotentiometer();

        // Set the base working point
        movePtt<Ptt::B, Dir::Inc>(35 + curveIdx * 17);
        movePtt<Ptt::C, Dir::Inc>(10);
        delay(10);
        const current_uA_t baseIb = curIb();

        printItems("I_B(uA) should be: ", baseIb, "\n");
        //printAllCurState();
        resultUce[curveIdx][0] = curUc();
        resultIc[curveIdx][0] = curIc();
        resultIb[curveIdx][0] = baseIb;
        for (int pointIdx = 1; pointIdx < 8; ++pointIdx) {
            movePtt<Ptt::C, Dir::Inc>(12);
            //printAllCurState();
            auto ib = curIb();
            while (ib - baseIb > IbDelta || ib - baseIb < -IbDelta) {
                if (ib > baseIb) {
                    movePtt<Ptt::B, Dir::Dec>(1);
                } else {
                    movePtt<Ptt::B, Dir::Inc>(1);
                }
                delay(10);
                ib = curIb();
            }
            const auto uc = curUc();
            const auto ic = curIc();
            resultUce[curveIdx][pointIdx] = uc;
            resultIc[curveIdx][pointIdx] = ic;
            resultIb[curveIdx][pointIdx] = ib;
            maxRes_mA = max(maxRes_mA, ic);
            maxRes_V = max(maxRes_V, uc);
            //printAllCurState();
        }

        printItems("Uce(V)  Ic(mA)  Ib(uA)\n");
        for (int i = 0; i < 8; ++i) {
            printItems(resultUce[curveIdx][i], "    ",
                       resultIc[curveIdx][i], "    ",
                       resultIb[curveIdx][i], "\n");
        }
        printItems("Measured curve ", curveIdx, " finished.\n");
        printItems("recorded maxRes_mA: ", maxRes_mA, "\n");
        printItems("recorded maxRes_V: ", maxRes_V, "\n");
        printItems("\n");
    }
    // display the curves
    for (int curveIdx = 0; curveIdx < 4; ++curveIdx) {
        int prevX = 0, prevY = 47;
        for (int t = 0; t < 8; ++t) {
            int x = min(int(resultUce[curveIdx][t] *83 / maxRes_V), 83);
            int y = 47 - int(resultIc[curveIdx][t] * 47 / maxRes_mA);
            display2.drawLine(prevX, prevY, x, y, 23);
            prevX = x;
            prevY = y;
        }
        display2.display();
        delay(200);
    }
}


#endif //FINALPROJECT_PART2_H
