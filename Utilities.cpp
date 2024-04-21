#include "Utilities.h"
#include <iostream>


namespace Utilities {

    float getMaxValue(std::vector<float>& vec) {
        float maxValue = vec[0];
        for (float value : vec)
            if (value > maxValue)
                maxValue = value;
        return maxValue;
    }

    float getMinValue(std::vector<float>& vec) {
        float minValue = vec[0];
        for (float value : vec)
            if (value < minValue)
                minValue = value;
        return minValue;
    }

    std::vector<float> multiplyVectorByConstant(std::vector<float>& vec, float factor)
    {
        std::vector<float> result(vec.size(), 0.0f);

        for (int i = 0; i < result.size(); i++)
            result[i] = vec[i] * factor;

        return result;
    }


    std::vector<float> divideVectorByConstant(std::vector<float>& vec, int factor) {
        std::vector<float> result;
        result.reserve(vec.size()); // Reserve space for efficiency

        std::transform(vec.begin(), vec.end(), std::back_inserter(result), [factor](const float& value) {
            return static_cast<float>(value) / factor;
            });

        return result;
    }

   
    std::vector<float> powerVector(std::vector<float>& vec, int exponent) {
        std::vector<float> result;
        result.reserve(vec.size());

        for (const float& value : vec) {
            result.push_back(std::pow(value, exponent));
        }

        return result;
    }

    std::vector<float> sqrtVector(std::vector<float>& vec, int exponent) {
        std::vector<float> result;
        result.reserve(vec.size());

        for (const float& value : vec) {
            result.push_back(std::sqrt(value));
        }

        return result;
    }

    std::vector<float> sumFloatVectors(std::vector<float>& vec1, std::vector<float>& vec2) {
        if (vec1.size() != vec2.size()) {
            // Handle vectors of different sizes
            throw std::invalid_argument("Vectors must have the same size");
        }

        std::vector<float> result;
        result.reserve(vec1.size());

        for (size_t i = 0; i < vec1.size(); ++i) {
            result.push_back(vec1[i] + vec2[i]);
        }

        return result;
    }

    std::vector<float> subtractVectors(std::vector<float>& vec1, std::vector<float>& vec2) {
        if (vec1.size() != vec2.size()) {
            // Handle vectors of different sizes
            throw std::invalid_argument("Vectors must have the same size");
        }

        std::vector<float> result;
        result.reserve(vec1.size());

        for (size_t i = 0; i < vec1.size(); ++i) {
            result.push_back(vec1[i] - vec2[i]);
        }

        return result;
    }



    std::vector<std::vector<float>> fillRandomMatrix(int nRows, int nCols, std::vector<std::vector<float>> lims) {
        std::random_device rd;
        std::vector<std::mt19937> generators(nCols);
        std::vector<std::uniform_real_distribution<float>> distributions(nCols);

        // Create a 2D vector with the specified dimensions and fill it with random values
        std::vector<std::vector<float>> randomMatrix(nRows, std::vector<float>(nCols));

        if (lims.size() != nCols)
            return randomMatrix;

        float minVal, maxVal;

        for (int i = 0; i < nCols; ++i) {

            minVal = lims[i][0];
            maxVal = lims[i][1];

            // Initialize random number generator and distribution for each column
            generators[i] = std::mt19937(rd());
            distributions[i] = std::uniform_real_distribution<float>(minVal, maxVal);
        }



        for (int i = 0; i < nRows; ++i) {
            for (int j = 0; j < nCols; ++j) {
                // Generate a random float value within the specified range for each column
                randomMatrix[i][j] = distributions[j](generators[j]);
            }
        }

        return randomMatrix;
    }

    std::vector<int> generateRandomIndexes(int numIndexes, int vectorSize) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, vectorSize - 1); // Range from 0 to vectorSize - 1

        std::vector<int> uniqueIndexes;

        while (uniqueIndexes.size() < numIndexes) {
            int randomIndex = dist(gen);

            // Ensure the generated index is unique
            if (std::find(uniqueIndexes.begin(), uniqueIndexes.end(), randomIndex) == uniqueIndexes.end()) {
                uniqueIndexes.push_back(randomIndex);
            }
        }

        return uniqueIndexes;
    }

    int randomWheelSelection(const std::vector<float>& probabilities) {
        std::random_device rd;
        std::mt19937 gen(rd());

        // Calculate the cumulative probabilities
        std::vector<float> cumulativeProbabilities(probabilities.size());
        std::partial_sum(probabilities.begin(), probabilities.end(), cumulativeProbabilities.begin());

        // Generate a random number between 0 and 1
        std::uniform_real_distribution<float> dist(0.0, 1.0);
        float randomValue = dist(gen);

        // Find the selected index based on the random value
        for (size_t i = 0; i < cumulativeProbabilities.size(); ++i) {
            if (randomValue <= cumulativeProbabilities[i]) {
                return static_cast<int>(i);
            }
        }

        // If no index is selected, return the last index
        return static_cast<int>(cumulativeProbabilities.size() - 1);
    }

    std::vector<int> generateBestIndexes(const std::vector<float>& probabilities, int nIndexes) {
        std::vector<int> indexes(nIndexes, 0);

        for (int i = 0; i < nIndexes; i++)
            indexes[i] = randomWheelSelection(probabilities);

        return indexes;
    }

    void normalizeVectors(std::vector<float>& vec1, std::vector<float>& vec2) {
        // Calculate the magnitude of each vector
        float magnitude1 = 0.0;
        float magnitude2 = 0.0;

        for (int i = 0; i < vec1.size(); i++) {
            magnitude1 += vec1[i] * vec1[i];
            magnitude2 += vec2[i] * vec2[i];
        }
        magnitude1 = std::sqrt(magnitude1);
        magnitude2 = std::sqrt(magnitude2);
        
        /*for (int i = 0; i < vec1.size(); i++) {
            magnitude2 += value * value;
        }*/
        magnitude2 = std::sqrt(magnitude2);

        // Check if magnitudes are non-zero
        if (magnitude1 != 0.0 && magnitude2 != 0.0) {
            // Normalize vec1
            for (float& value : vec1) {
                value /= magnitude1;
            }

            // Normalize vec2
            for (float& value : vec2) {
                value /= magnitude2;
            }
        }
        else {
            // Handle division by zero or zero vectors here if necessary
            std::cerr << "Error: Cannot normalize zero vectors." << std::endl;
        }
    }

    // Linear interpolation function
    float linearInterpolation(float x0, float y0, float x1, float y1, float x) {
        return y0 + (y1 - y0) * ((x - x0) / (x1 - x0));
    }

    // Linear interpolation function
    float linearInterpolation(float t, float y0, float y1) {
        return y0 + t * (y1 - y0);
    }

    float cubicInterpolation(float t, float y0, float y1, float y2, float y3) {
        float a = y3 - y2 - y0 + y1;
        float b = y0 - y1 - a;
        float c = y2 - y0;
        float d = y1;

        return a * t * t * t + b * t * t + c * t + d;
    }

    // Resample a 1D vector to a desired number of points
    std::vector<float> resampleToDesiredPoints(const std::vector<float>& inputVector, size_t desiredPoints) {
        std::vector<float> resampledVector;

        if (desiredPoints == 0) {
            // If desiredPoints is 0, return an empty vector.
            return resampledVector;
        }

        // Calculate the step size for resampling
        double stepSize = static_cast<double>(inputVector.size() - 1) / (desiredPoints - 1);

        for (size_t i = 0; i < desiredPoints-1; ++i) {
            // Calculate the corresponding index in the original vector
            double originalIndex = i * stepSize;

            // Determine the indices of the surrounding points for interpolation
            size_t lowerIndex = static_cast<size_t>(originalIndex);
            size_t upperIndex = std::min(lowerIndex + 1, inputVector.size() - 1);

            // Calculate the interpolation parameter (t) within the segment
            double t = originalIndex - lowerIndex;

            // Perform linear interpolation between adjacent data points
            float interpolatedValue = linearInterpolation(
                static_cast<float>(lowerIndex), inputVector[lowerIndex],
                static_cast<float>(upperIndex), inputVector[upperIndex],
                static_cast<float>(originalIndex)
            );

            // Add the interpolated value to the resampled vector
            resampledVector.push_back(interpolatedValue);
        }

        return resampledVector;
    }

    // Upsample a signal to match a reference sample rate
    std::vector<float> upsampleToMatchSampleRate(const std::vector<float>& signalToUpsample, double referenceSampleRate) {
        double ratio = referenceSampleRate / static_cast<double>(signalToUpsample.size());

        std::vector<float> upsampledSignal;
        upsampledSignal.reserve(static_cast<size_t>(signalToUpsample.size() * ratio));

        for (size_t i = 0; i < signalToUpsample.size() - 1; ++i) {
            for (double j = 0; j < ratio; ++j) {
                double t = j / ratio;
                float interpolatedValue = linearInterpolation(t, signalToUpsample[i], signalToUpsample[i + 1]);
                upsampledSignal.push_back(interpolatedValue);
            }
        }

        return upsampledSignal;
    }


    std::vector<float> downsampleVector(const std::vector<float>& inputVector, size_t desiredPoints) {
        std::vector<float> downsampledVector;

        if (desiredPoints >= inputVector.size()) {
            // If the desired number of points is greater or equal to the original size,
            // simply return a copy of the original vector.
            return inputVector;
        }

        // Calculate the step size to evenly select points
        size_t stepSize = inputVector.size() / desiredPoints;

        for (size_t i = 0; i < inputVector.size(); i += stepSize) {
            // Add the selected data point to the downsampled vector
            downsampledVector.push_back(inputVector[i]);
        }

        return downsampledVector;
    }

 
}