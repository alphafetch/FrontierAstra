#include "math.hpp"

#include <cmath>
#include <random>
#include <climits>

using std::sqrt;

// Warp a variable to a sphere based on the other two
float warpToSphere(float a, float b, float target) {
    return target * sqrt(1 - ((a * a) / 2) - ((b * b) / 2) + (((a * a) * (b * b)) / 3));
}

// Get a child seed from the master seed
int getChildSeed(const int master, int id) {
    std::mt19937 rng(master ^ (id * 0x9E3779B9));
    std::uniform_int_distribution<int> dist(INT_MIN, INT_MAX);
    return dist(rng);
}