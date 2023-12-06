//
// Created on 2023/12/7.
//

#ifndef FINALPROJECT_TRANSISTOR_MEASURE_H
#define FINALPROJECT_TRANSISTOR_MEASURE_H

#include "constants.h"

/**
 * TODO: Measure the beta of the transistor connected to the given three ports.
 */
template <TransistorType mode>
beta_t measureTransistorBeta(const Port &portB, const Port &portC, const Port &portE);



#endif //FINALPROJECT_TRANSISTOR_MEASURE_H
