#pragma once
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

enum intType {
	LINEAR,
	CUBIC
};

namespace Utilities
{
	float getMaxValue(std::vector<float>& vec);
	float getMinValue(std::vector<float>& vec);

	std::vector<float> multiplyVectorByConstant(std::vector<float>& vec, float factor);
	std::vector<float> divideVectorByConstant(std::vector<float>& vec, int factor);
	std::vector<float> powerVector(std::vector<float>& vec, int exponent);
	std::vector<float> sqrtVector(std::vector<float>& vec, int exponent);
	std::vector<float> sumFloatVectors(std::vector<float>& vec1,  std::vector<float>& vec2);
	std::vector<float> subtractVectors(std::vector<float>& vec1, std::vector<float>& vec2);


	std::vector<std::vector<float>> fillRandomMatrix(int nRows, int nCols, std::vector<std::vector<float>> lims);

	std::vector<int> generateRandomIndexes(int numIndexes, int vectorSize);

	int randomWheelSelection(const std::vector<float>& probabilities);

	std::vector<int> generateBestIndexes(const std::vector<float>& probabilities, int nIndexes);

	void normalizeVectors(std::vector<float>& vec1, std::vector<float>& vec2);

	float linearInterpolation(float x0, float y0, float x1, float y1, float x);
	float linearInterpolation(float t, float y0, float y1);
	float cubicInterpolation(float t, float y0, float y1, float y2, float y3);
	std::vector<float> resampleToDesiredPoints(const std::vector<float>& inputVector, size_t desiredPoints);
	std::vector<float> upsampleToMatchSampleRate(const std::vector<float>& signalToUpsample, double referenceSampleRate);
	std::vector<float> downsampleVector(const std::vector<float>& inputVector, size_t desiredPoints);
};

