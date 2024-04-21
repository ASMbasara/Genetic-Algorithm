#include "ImportFileData.h"


void ImportFileData::importTwoVariableFile(std::string path, std::vector<float> &x, std::vector<float> &y){
    std::vector<float> timeData;
    std::vector<float> voltageData;

    // Open the text file for reading
    std::ifstream inputFile(path);

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return;
    }

    // Read and process each line in the file
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        float time, voltage;

        // Attempt to read time and voltage from the line
        if (iss >> time >> voltage) {
            // Store the values in the respective vectors
            x.push_back(time);
            y.push_back(voltage);
        }
        else {
            std::cerr << "Warning: Skipping line with invalid data: " << line << std::endl;
        }
    }

    // Close the file
    inputFile.close();
}

std::vector<float> ImportFileData::crossCorrelation(const std::vector<float>& x, const std::vector<float>& y) {
    std::vector<float> result(x.size() + y.size() - 1, 0.0);

    for (size_t i = 0; i < x.size(); ++i) {
        for (size_t j = 0; j < y.size(); ++j) {
            result[i + j] += x[i] * y[j];
        }
    }

    return result;
}

void ImportFileData::compensatePhaseShift(std::vector<float>& signal, int phaseShift) {
    size_t signalSize = signal.size();

    if (phaseShift > 0) {
        // Circular shift to the left
        std::rotate(signal.begin(), signal.begin() + phaseShift, signal.end());
    }
    else if (phaseShift < 0) {
        // Circular shift to the right
        phaseShift = -phaseShift;
        std::rotate(signal.rbegin(), signal.rbegin() + phaseShift, signal.rend());
    }
}

void ImportFileData::processAndCompensate(std::vector<float>& signal1, std::vector<float>& signal2) {
    // Compute cross-correlation
    std::vector<float> correlationResult = crossCorrelation(signal1, signal2);

    // Find the index corresponding to the maximum value
    size_t maxIndex = 0;
    for (size_t i = 1; i < correlationResult.size(); ++i) {
        if (correlationResult[i] > correlationResult[maxIndex]) {
            maxIndex = i;
        }
    }

    // Calculate the phase shift
    int phaseShift = static_cast<int>(signal2.size()) - 1 - static_cast<int>(maxIndex);

    // Compensate for the phase shift in both signals
    //compensatePhaseShift(signal1, phaseShift);
    compensatePhaseShift(signal2, phaseShift);
}

void ImportFileData::saveDataToFile(const std::vector<float>& x, const std::vector<float>& y, const std::string& filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        for (size_t i = 0; i < x.size(); ++i) {
            outfile << x[i] << " " << y[i] << "\n";
        }
        outfile.close();
        std::cout << "Data saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to open " << filename << std::endl;
    }
}