#pragma once
#include <vector>
#include <random>
#include <cmath>
#include <iostream>
#include "Utilities.h"
#include "WienerHammerstein.h"
#include "Scorer.h"


using signal = std::vector<float>;

using metaData = struct metaData {
	int generations;
	int elitists;

	int population_size;
	int chromosome_size;
	int immigration_size;

	float cross_rate;
	float mut_rate;
	float mut_sigma;

	std::vector<std::vector<float>> limits;
	std::vector<float> amplitude_weights;
	
};

enum mutType {
	UNIFORM_MUTATION,
	BIT_FLIP_MUTATION,
	SWAP_MUTATION
};

enum crossType {
	BLEND_CROSSOVER,
	BLEND_CROSSOVER2,
	UNIFORM_CROSSOVER,
	SINGLE_POINT_CROSSOVER
};

class GA
{
private:
	metaData* mData = new metaData;
	WienerHammerstein *m_WH;
	Scorer* scorer = new Scorer;

	signal mInput;
	signal mOutput;

	std::vector<std::vector<float>> population;
	std::vector<std::vector<float>> extended_population;
	std::vector<float> best_chromosome;
	std::vector<float> fitness;
	std::vector<float> extended_fitness;
	int cross_num;
	int mut_num;

public:
	explicit GA(const metaData *mD, const signal& input, const signal& output, int mapping_function);
	std::vector<float> GetBestChromosome() const;
	std::vector<float> GetLastChromosome() const;

	float ChromosomeFitness(const signal &chromosome, signal &nlblock_out);

	std::vector<float> Fitness(std::vector<std::vector<float>> &pop, signal& nlblock_out);

	std::vector<float> blendCrossover(std::vector<float>& parent1, std::vector<float>& parent2);
	std::vector<float> blendCrossover2(std::vector<float>& parent1, std::vector<float>& parent2);

	std::vector<std::vector<float>> reproducePopulation(int crossoverType, const std::vector<float>& probabilities);

	void uniformMutation(std::vector<float>& chromosome, std::vector<std::vector<float>>& lims);

	void MutatePopulation(int mutationType, std::vector<std::vector<float>> &pop);

	void addImmigrants();

	void sortAndTruncatePopulation();
	void insertElitists(std::vector<std::vector<float>> &pop, std::vector<std::vector<float>> elitists);

	void Optimize(signal& prediction, std::vector<float>& best_params);

	~GA();

};

