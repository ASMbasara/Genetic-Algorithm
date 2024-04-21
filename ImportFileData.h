#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

class ImportFileData
{
public:
	void importTwoVariableFile(std::string path, std::vector<float>& x, std::vector<float>& y);
	std::vector<float> crossCorrelation(const std::vector<float>& x, const std::vector<float>& y);
	void compensatePhaseShift(std::vector<float>& signal, int phaseShift);
	void processAndCompensate(std::vector<float>& signal1, std::vector<float>& signal2);
	void saveDataToFile(const std::vector<float>& x, const std::vector<float>& y, const std::string& filename);
};

