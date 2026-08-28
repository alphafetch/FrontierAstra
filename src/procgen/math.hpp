#ifndef PROCGEN_MATH_HPP
#define PROCGEN_MATH_HPP

// Warp a variable to a sphere based on the other two
float warpToSphere(float a, float b, float target);

// Get a child seed from the master seed
int getChildSeed(const int master, const int id);

#endif