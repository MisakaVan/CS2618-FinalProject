//
// Created on 2023/12/16.
//

#ifndef FINALPROJECT_PART2_H
#define FINALPROJECT_PART2_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "constants.h"

// int B_control=6;
// int B_clk=5;

// int C_control=10;
// int C_clk=9;


double a[4][8]{};

double b[4][8]{};
double max_x = 0;
double max_x2 = 0;

Adafruit_PCD8544 display2 = Adafruit_PCD8544(8, 7, 17, 16, 15);

double calU_B()
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

double calU_C()
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

double calI_B()//微安
{
    double x = analogRead(pin_B_out) * 5.0 / 1023.0;
    double y = analogRead(pin_B) * 5.0 / 1023.0;
    return (x - y) / R1;
}

double calI_C()//毫安
{
    double x = analogRead(pin_C_out) * 5.0 / 1023.0;
    double y = analogRead(pin_C) * 5.0 / 1023.0;
    return (x - y) / R2;
}

void dispvol()
{
    double x = calU_B();
    double y = calU_C();
    Serial.print("UB:");
    Serial.print(x);
    Serial.print("\n");
    Serial.print("UC:");
    Serial.print(y);
    Serial.println("\n\n");
}

void dispcur()
{

    double ib = calI_B();//U_B'
    double ic = calI_C();//U_C'
    Serial.print("IB:");
    Serial.print(ib);
    Serial.print("\n");
    Serial.print("IC:");
    Serial.print(ic);
    Serial.print("\n\n");
}

void clear()
{
    digitalWrite(pin_B_up, LOW);
    digitalWrite(pin_C_up, LOW);
    digitalWrite(pin_B_clk, HIGH);
    digitalWrite(pin_C_clk, HIGH);
    for (double i = 0; i < 150; i += 1) {
        digitalWrite(pin_B_clk, LOW);
        digitalWrite(pin_C_clk, LOW);
        delay(10);
        digitalWrite(pin_B_clk, HIGH);
        digitalWrite(pin_C_clk, HIGH);
        delay(10);

    }

    Serial.println("cleared!");

}

void add_B(int i)//加的次数，100次加满
{
    digitalWrite(pin_B_up, HIGH);
    digitalWrite(pin_B_clk, HIGH);
    for (int t = 0; t < i; ++t) {
        digitalWrite(pin_B_clk, LOW);
        delay(20);
        digitalWrite(pin_B_clk, HIGH);
        delay(20);
        //Serial.println(calU_B());
    }
}

void minus_B(int i)//加的次数，100次加满
{
    digitalWrite(pin_B_up, LOW);
    digitalWrite(pin_B_clk, HIGH);
    for (int t = 0; t < i; ++t) {
        digitalWrite(pin_B_clk, LOW);
        delay(20);
        digitalWrite(pin_B_clk, HIGH);
        delay(20);
    }
}

void add_C(int i)//加的次数，100次加满
{
    digitalWrite(pin_C_up, HIGH);
    digitalWrite(pin_C_clk, HIGH);
    for (int t = 0; t < i; ++t) {
        digitalWrite(pin_C_clk, LOW);
        delay(20);
        digitalWrite(pin_C_clk, HIGH);
        delay(20);
    }
}

void minus_C(int i)//加的次数，100次加满
{
    digitalWrite(pin_C_up, LOW);
    digitalWrite(pin_C_clk, HIGH);
    for (int t = 0; t < i; ++t) {
        digitalWrite(pin_C_clk, LOW);
        delay(20);
        digitalWrite(pin_C_clk, HIGH);
        delay(20);
    }
}

void printall()
{
    Serial.println("all");
    Serial.print("ub:");
    Serial.print(calU_B());
    Serial.print("\nuc:");
    Serial.print(calU_C());
    Serial.print("\nib:");
    Serial.print(calI_B());
    Serial.print("\nic:");
    Serial.print(calI_C());
    Serial.print("\n");
    delay(3000);
}

void setup2()
{
    Serial.begin(9600);
    display2.begin();
    display2.setContrast(23);
    display2.clearDisplay();
    pinMode(pin_B_up, OUTPUT);
    pinMode(pin_B_clk, OUTPUT);
    pinMode(pin_C_up, OUTPUT);
    pinMode(pin_C_clk, OUTPUT);

}

void loop2()
{
    display2.display();
    for (int times = 3; times >= 0; --times) {
        clear();
        add_B(35 + times * 17);
        add_C(10);
        double Ib = calI_B();
        Serial.println("I_B should be:");

        Serial.println(Ib);
        //printall();
        a[times][0] = calU_C();
        b[times][0] = calI_C();
        for (int i = 0; i < 7; ++i) {
            add_C(12);
            //printall();
            while (calI_B() - Ib > 0.05 || calI_B() - Ib < -0.05) {
                if (calI_B() > Ib) minus_B(1);
                else add_B(1);
                delay(10);
            }

            a[times][i + 1] = calU_C();
            b[times][i + 1] = calI_C();
            //printall();
        }


        for (int t = 0; t < 8; ++t) {
            Serial.print(a[times][t]);
            Serial.print("  ");
            Serial.print(b[times][t]);
            Serial.print("\n");
            if (max_x2 < b[times][t]) max_x2 = b[times][t];
            if (max_x < a[times][t]) max_x = a[times][t];
        }

        for (int t = 0; t < 8; ++t) {
            if (t == 0) {
                display2.drawLine(0, 47, int(a[times][t] * 83 / max_x), 47 - int(b[times][t] * 47 / max_x2), 23);
            } else {
                if (int(a[times][t] * 83 / max_x) > 80) {
                    display2.drawLine(int(a[times][t - 1] * 83 / max_x), 47 - int(b[times][t - 1] * 47 / max_x2), 83,
                                     47 - int(b[times][t] * 47 / max_x2), 23);
                } else {
                    display2.drawLine(int(a[times][t - 1] * 83 / max_x), 47 - int(b[times][t - 1] * 47 / max_x2),
                                      int(a[times][t] * 83 / max_x), 47 - int(b[times][t] * 47 / max_x2), 23);
                }
            }
            display2.display();
            delay(200);
        }
    }

    delay(10000000);
}


/**
 * draw the I_c - U_be curve of a transistor.
 * this has been implemented in part2.h.
 */
void doPart2(){
    setup2();
    loop2();
}


#endif //FINALPROJECT_PART2_H
