#include "WienerHammerstein.h"


WienerHammerstein::WienerHammerstein(const signal &input, const int param_size, int mapp)
{
	
	this->input_size = input.size();
	this->function = mapp;
	if (mapp != POLY)
		this->poly_size = 4;
	else
		this->poly_size = param_size - g_params_size;
	
	signal abs_input = input;
	for (float& value : abs_input)
		value = std::abs(value);

	LPF = new LowPassFilter(abs_input);
	x_bias = signal(input_size);
	x_bias = LPF->filter();

}

signal WienerHammerstein::mappingFunctionPoly(signal& input, const signal& params) {
	signal out(input.size(), 0.0f);
	signal temp(input.size(), 0.0f);
	int poly_pos = g_params_size;

	for (int i = 0; i < poly_size; i++) {
		temp = Utilities::powerVector(input, i);
		temp=Utilities::multiplyVectorByConstant(temp, params[poly_pos++]);
		out = Utilities::sumFloatVectors(out, temp);
	}
	
	return out;
}


signal WienerHammerstein::mappingFunctionTanh(signal& input, const signal& params) {
	signal out(input.size(), 0.0f);

	int poly_pos = g_params_size;
	float kp = params[poly_pos++];
	float kn = params[poly_pos++];
	float gp = params[poly_pos++];
	float gn = params[poly_pos++];

	float ap = (1 - std::pow(std::tanh(kp), 2));
	float bp = std::tanh(kp);
	float an = (1 - std::pow(std::tanh(kn), 2));
	float bn = -std::tanh(kn);

	float sample = 0.0f;

	for (int i = 0; i < input.size(); i++) {
		sample = input[i];
		if (sample > kp)
			out[i] = ap * std::tanh(gp * (sample - kp)) + bp;
		else if (sample < -kn)
			out[i] = an * std::tanh(gn * (sample + kn)) + bn;
		else
			out[i] = std::tanh(sample);
	}

	return out;
}


signal WienerHammerstein::mappingFunctionAtan(signal& input, const signal& params) {
	signal out(input.size(), 0.0f);
	
	int poly_pos = g_params_size;
	float kp = params[poly_pos++];
	float kn = params[poly_pos++];
	float gp = params[poly_pos++];
	float gn = params[poly_pos++];

	float ap = (1 - std::pow(std::atan(kp),2));
	float bp = std::atan(kp);
	float an = (1 - std::pow(std::atan(kn), 2));
	float bn = -std::atan(kn);

	float sample = 0.0f;

	for (int i = 0; i < input.size(); i++) {
		sample = input[i];
		if (sample > kp)
			out[i] = ap * std::atan(gp * (sample - kp)) + bp;
		else if (sample < -kn)
			out[i] = an * std::atan(gn * (sample + kn)) + bn;
		else
			out[i] = std::atan(sample);
	}

	return out;
}

void WienerHammerstein::UnpackParams(const signal &params) {
	g_pre = params[0];
	g_bias = params[1];
	g_wet = params[2];
	g_post = params[3];
}

signal WienerHammerstein::NLBlock(const signal &params, signal &input) {
	UnpackParams(params);
	float g_dry = 1 - g_wet;

	signal x_pre = Utilities::multiplyVectorByConstant(input, g_pre); //x_pre
	signal x_bias_weighted = Utilities::multiplyVectorByConstant(x_bias, g_bias);
	signal x_pre_mapping = Utilities::sumFloatVectors(x_pre,x_bias_weighted); //x_pre_mapping
	signal x_mapped;

	switch (function) {
		case POLY:
			x_mapped = mappingFunctionPoly(x_pre_mapping, params);
			break;
		case TANH:
			x_mapped = mappingFunctionTanh(x_pre_mapping, params);
			break;
		case ATAN:
			x_mapped = mappingFunctionAtan(x_pre_mapping, params);
			break;
		default:
			x_mapped = mappingFunctionPoly(x_pre_mapping, params);
			break;
	}
	
	signal wet = Utilities::multiplyVectorByConstant(x_mapped, g_wet);
	signal dry = Utilities::multiplyVectorByConstant(x_pre, g_dry);
	signal mix = Utilities::sumFloatVectors(wet, dry);
	signal out = Utilities::multiplyVectorByConstant(mix, g_post);

	return out;
}



WienerHammerstein::~WienerHammerstein()
{
	delete LPF;
}
