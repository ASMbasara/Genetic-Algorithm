#include "LowPassFilter.h"


LowPassFilter::LowPassFilter(const std::vector<float> &input)
{
    x = input;
    len = input.size();
    y = std::vector<float>(len,0.0f);
    
}

std::vector<float> LowPassFilter::filter()
{
    int m = 10;
    std::vector<float> result;

    for (int n = 0; n < len; n++) {
        //x[n] = sample;
        y[n] = b[0] * x[n] + b[1] * x[(n - 1 + m) % m] + b[2] * x[(n - 2 + m) % m] + b[3] * x[(n - 3 + m) % m] + b[4] * x[(n - 4 + m) % m]
            - a[1] * y[(n - 1 + m) % m] - a[2] * y[(n - 2 + m) % m] - a[3] * y[(n - 3 + m) % m] - a[4] * y[(n - 4 + m) % m];

        result.push_back(y[n]);
    }

    return result;
}
