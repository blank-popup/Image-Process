#include "pch.h"
#include "ipl_common.h"
#include <cmath>


int limit_intensity(double intensity) {
    int value_round = static_cast<int>(round(intensity));
    if (value_round < 0) {
        return 0;
    }
    if (value_round > 255) {
        return 255;
    }
    return value_round;
}
