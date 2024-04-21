// Genetic Algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GA.h"
#include "ImportFileData.h"
#include "Utilities.h"
#include "WienerHammerstein.h"
//#include "matplotlibcpp.h"



//namespace plt = matplotlibcpp;

void initializeGAMetaData(metaData& md) {
    md.generations = 1000;
    md.elitists = 1;
    md.population_size = 1000;
    md.chromosome_size = 12;
    md.immigration_size = std::round(md.population_size/10);
    md.cross_rate = 0.7;
    md.mut_rate = 0.5;
    md.mut_sigma = 0.35;

    std::vector<std::vector<float>> limits(md.chromosome_size,std::vector<float>(2));
    for (int i = 0; i < md.chromosome_size; i++) {
        if (i == 0 || i == 3) { //g_pre ou g_post
            limits[i][0] = 0.01;
            limits[i][1] = 10;
        }
        else if (i == 1) {
            limits[i][0] = -1;
            limits[i][1] = 1;
        }
        else if (i == 2) {
            limits[i][0] = 0;
            limits[i][1] = 1;
        }
        else {
            limits[i][0] = -1;
            limits[i][1] = 1;
        }
    }
    md.limits = limits;

    std::vector<float> amplitude_weights = { 2,1,1,2 };
    md.amplitude_weights = amplitude_weights;
}

int main()
{
    metaData md;
    initializeGAMetaData(md);

    /*
    std::vector<float> time_in, time_out, input, output;
    std::string in_path = "C:\\Users\\jhvaz\\Documents\\Faculdade\\5º Ano\\Tese\\SPICE\\Simple SINE\\simple_sine_input_10m.txt";
    std::string out_path = "C:\\Users\\jhvaz\\Documents\\Faculdade\\5º Ano\\Tese\\SPICE\\Simple SINE\\simple_sine_output_10m.txt";

    ImportFileData imp;
    imp.importTwoVariableFile(in_path, time_in, input);
    imp.importTwoVariableFile(out_path, time_out, output);

    float sample_rate = 44.1e3;
    int interpolation = LINEAR;
    size_t desired_points = 10e3;

    
    input = Utilities::upsampleToMatchSampleRate(input, sample_rate);
    output = Utilities::upsampleToMatchSampleRate(output, sample_rate);
    time_in = Utilities::upsampleToMatchSampleRate(time_in, sample_rate);
    time_out = Utilities::upsampleToMatchSampleRate(time_out, sample_rate);
    
    Utilities::normalizeVectors(input, output);

    std::string outFilePath = "C:\\Users\\jhvaz\\Documents\\Faculdade\\5º Ano\\Tese\\Python\\Signals to Plot\\Simple Sine 10m\\";

    imp.saveDataToFile(time_in, input, outFilePath + "simple_sine_input_10m.txt");
    imp.saveDataToFile(time_out, output, outFilePath + "simple_sine_output_10m.txt");

    imp.processAndCompensate(input, output);

    imp.saveDataToFile(time_in, input, outFilePath + "simple_sine_input_10m_compensated.txt");
    imp.saveDataToFile(time_out, output, outFilePath + "simple_sine_output_10m_compensated.txt");
    */
    std::vector<float> time_in, time_out, input, output;

    std::string in_path = "C:\\Users\\jhvaz\\Documents\\Faculdade\\5º Ano\\Tese\\Python\\Signals to Plot\\Simple Sine 10m\\simple_sine_input_10m_compensated.txt";
    std::string out_path = "C:\\Users\\jhvaz\\Documents\\Faculdade\\5º Ano\\Tese\\Python\\Signals to Plot\\Simple Sine 10m\\simple_sine_output_10m_compensated.txt";

    ImportFileData imp;
    imp.importTwoVariableFile(in_path, time_in, input);
    imp.importTwoVariableFile(out_path, time_out, output);

    std::cout << std::endl << "Initializing Genetic Algorithm..." << std::endl << std::endl;
    
    std::vector<std::vector<float>> limits_tan;
    int mapping_function = TANH;
    if (mapping_function == ATAN || mapping_function==TANH) {
        md.chromosome_size = 8;
        limits_tan=std::vector<std::vector<float>>(md.chromosome_size, std::vector<float>(2));
        for (int i = 0; i < md.chromosome_size; i++) {
            if (i == 0 || i == 3 || i==6 || i==7) { //g_pre ou g_post ou  gp ou gn
                limits_tan[i][0] = 0.01;
                limits_tan[i][1] = 10;
            }
            else if (i == 1) {
                limits_tan[i][0] = -1;
                limits_tan[i][1] = 1;
            }
            else if (i == 2) {
                limits_tan[i][0] = 0;
                limits_tan[i][1] = 1;
            }
            else if(i==4 || i==5){ //kp ou kn
                limits_tan[i][0] = 0;
                limits_tan[i][1] = 1;
            }
            
        }
        md.limits = limits_tan;
    }
    
    GA ga(&md, input, output, mapping_function);

    std::vector<float> pred;
    std::vector<float> best_params;
    ga.Optimize(pred, best_params);
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
