#pragma once
#include <vector>
#include <numeric>
#include "Utilities.h"

class Scorer
{
private:
	float error;
	float score;
	std::vector<float> prediction;
	std::vector<float> target;
	std::vector<float> borders;
	std::vector<float> amplitude_weights;

public:
	void setSignals(const std::vector<float> &pred, const std::vector<float> &target);
	void setSignals(const std::vector<float>& pred, std::vector<float>& target, const std::vector<float> weights);
	float getAverage(const std::vector<float>& s1);
	std::vector<float> getDiffs(std::vector<float>& s1, std::vector<float>& s2);
	float getAverageDiff();
	float getESR();
	float getMSE();
	float getRMSE();
	float getRMSE_weighted();

	float getScore();
	float getNegativeScore();
};

