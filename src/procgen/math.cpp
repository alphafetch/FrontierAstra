#include "math.hpp"

#include <cmath>

using std::sqrt;

float warpToSphere(float a, float b, float target) {
    return target * sqrt(1 - ((a * a) / 2) - ((b * b) / 2) + (((a * a) * (b * b)) / 3));
}