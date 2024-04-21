#pragma once
#include "Utilities.h"
#include <vector>
#include "LowPassFilter.h"

using signal = std::vector<float>;

enum mapping_function {
	POLY,
	TANH,
	ATAN
};

class WienerHammerstein
{
private:
	int input_size;

	float g_pre=1;
	float g_bias=0;
	float g_wet=1;
	float g_post=1;
	
	int g_params_size=4;
	int poly_size;

	int function;

	LowPassFilter *LPF;
	signal x_lp;
	signal x_bias;


public:

	explicit WienerHammerstein(const signal& input, const int poly_size, int mapp);

	signal mappingFunctionPoly(signal& input, const signal& params);

	signal mappingFunctionTanh(signal& input, const signal& params);

	signal mappingFunctionAtan(signal& input, const signal& params);

	void UnpackParams(const signal &params);

	signal NLBlock(const signal &params, signal &input);

	~WienerHammerstein();

};

