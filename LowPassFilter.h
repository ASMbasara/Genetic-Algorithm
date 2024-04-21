#pragma once
#include <vector>
#include <cmath>

class LowPassFilter {
private:
    std::vector<float> x; // input vector
    std::vector<float> y; // output vector
    // pointer to the current array index
    int len=0;
    const double a[5] = { 1.0, -3.9901906693053046, 5.970615287035284, -3.970658467192523, 0.990233849559477 };
    const double b[5] = { 6.058293979793542e-12, 2.4233175919174167e-11, 3.634976387876125e-11, 2.4233175919174167e-11, 6.058293979793542e-12 };
    int c = 0;

public:
    LowPassFilter(const std::vector<float>& input);
    std::vector<float> filter();
};
