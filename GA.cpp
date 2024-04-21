#include "GA.h"

GA::GA(const metaData *mD, const signal &input, const signal &output, int mapping_function)
{
	if (mD) {
		mData->generations = mD->generations;
		mData->elitists = mD->elitists;
		mData->population_size = mD->population_size;
		mData->chromosome_size = mD->chromosome_size;
		mData->immigration_size = mD->immigration_size;
		mData->cross_rate = mD->cross_rate;
		mData->mut_rate = mD->mut_rate;
		mData->mut_sigma = mD->mut_sigma;

		mData->limits = std::vector<std::vector<float>>(mData->population_size, std::vector<float>(mData->chromosome_size));
		mData->limits=mD->limits;
		mData->amplitude_weights = mD->amplitude_weights;

		mut_num = std::lround(mData->population_size * mData->mut_rate);
		cross_num = std::lround(mData->population_size * mData->cross_rate);
		if (cross_num % 2 != 0)
			cross_num--;

		population = Utilities::fillRandomMatrix(mData->population_size, mData->chromosome_size, mData->limits);
		extended_population = std::vector<std::vector<float>>(mData->population_size + cross_num / 2 + mData->elitists + mData->immigration_size, std::vector<float>(mData->chromosome_size));
		fitness = std::vector<float>(mData->population_size, 0.0f);
		extended_fitness = std::vector<float>(extended_population.size(), 0.0f);

		m_WH = new WienerHammerstein(input, mData->chromosome_size, mapping_function);
	}
	
	this->mInput = input;
	this->mOutput = output;
	
}

std::vector<float> GA::GetBestChromosome() const
{
	return best_chromosome;
}

std::vector<float> GA::GetLastChromosome() const
{
	return signal();
}


//*************************************************** FITNESS *********************************************************

float GA::ChromosomeFitness(const std::vector<float>& chromosome, signal& nlblock_out) {

	nlblock_out = m_WH->NLBlock(chromosome, mInput);

	scorer->setSignals(nlblock_out, mOutput, mData->amplitude_weights);

	float error = scorer->getRMSE_weighted();

	return scorer->getNegativeScore();

}

std::vector<float> GA::Fitness(std::vector<std::vector<float>> &pop, signal& nlblock_out)
{
	std::vector<float> fit(pop.size(), 0.0f);

	for (int i=0; i<pop.size(); i++)
		fit[i]=ChromosomeFitness(pop[i], nlblock_out);

	return fit;

}

//*************************************************** CROSSOVER *********************************************************
std::vector<float> GA::blendCrossover(std::vector<float>& parent1, std::vector<float>& parent2) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0, 1.0); // Random alpha value between 0 and 1

	float alpha = dist(gen); // Randomly select alpha within [0, alpha]
	std::vector<float> offspring(mData->chromosome_size, 0.0f);

	for (int i = 0; i < mData->chromosome_size; ++i) {
		float lower = std::min(parent1[i], parent2[i]);
		float upper = std::max(parent1[i], parent2[i]);

		// Calculate the gene for the offspring
		offspring[i] = lower + alpha * (upper - lower);
		offspring[i] = std::min(std::max(offspring[i], mData->limits[i][0]), mData->limits[i][1]);
	}

	return offspring;
}

std::vector<float> GA::blendCrossover2(std::vector<float>& parent1, std::vector<float>& parent2) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dist(0.0, 1.0); // Random alpha value between 0 and 1

	float alpha = dist(gen); // Randomly select alpha within [0, alpha]
	std::vector<float> offspring(mData->chromosome_size, 0.0f);

	for (int i = 0; i < mData->chromosome_size; ++i) {
		offspring[i] = alpha * parent1[i] + (1 - alpha) * parent2[i];
		offspring[i] = std::min(std::max(offspring[i], mData->limits[i][0]), mData->limits[i][1]);
	}
	

	return offspring;
}
std::vector<std::vector<float>> GA::reproducePopulation(int crossoverType, const std::vector<float>& probabilities) {
	 std::vector<int> indexes = Utilities::generateBestIndexes(probabilities,cross_num);
	std::vector<std::vector<float>> offsprings(cross_num / 2, std::vector<float>(mData->chromosome_size));

	switch (crossoverType) {
	case BLEND_CROSSOVER:
		for (int i = 0, j = 0; i < cross_num - 1, j < offsprings.size(); i += 2, j++)
			offsprings[j] = blendCrossover(population[indexes[i]], population[indexes[i + 1]]);
		break;

	case BLEND_CROSSOVER2:
		for (int i = 0, j = 0; i < cross_num - 1, j < offsprings.size(); i += 2, j++)
			offsprings[j] = blendCrossover2(population[indexes[i]], population[indexes[i + 1]]);
		break;

	default:
		for (int i = 0, j = 0; i < cross_num - 1, j < offsprings.size(); i += 2, j++)
			offsprings[j] = blendCrossover(population[indexes[i]], population[indexes[i + 1]]);
		break;
	}

	return offsprings;
}



//*************************************************** MUTATION *********************************************************

void GA::uniformMutation(std::vector<float>& chromosome,  std::vector<std::vector<float>> &lims) {
	std::random_device rd;
	std::vector<std::mt19937> gen(mData->chromosome_size,std::mt19937(rd()));
	std::vector<std::uniform_real_distribution<float>> dist(mData->chromosome_size);

	for (int i = 0; i < mData->chromosome_size; ++i) {

		float minVal = lims[i][0];
		float maxVal = lims[i][1];
		// Initialize random number generator and distribution for each column
		gen[i] = std::mt19937(rd());
		dist[i] = std::uniform_real_distribution<float>(minVal, maxVal);

		chromosome[i]+= dist[i](gen[i]) * mData->mut_sigma;
		chromosome[i] = std::min(std::max(chromosome[i], minVal), maxVal);
	}

}


void GA::MutatePopulation(int mutationType, std::vector<std::vector<float>> &pop) {
	std::vector<int> indexes = Utilities::generateRandomIndexes(mut_num, pop.size());

	switch (mutationType) {
		
	case UNIFORM_MUTATION:
		for (int i : indexes)
			uniformMutation(pop[i], mData->limits);
		break;
	case BIT_FLIP_MUTATION:
		break;
	case SWAP_MUTATION:
		break;

	default:
		for (int i : indexes)
			uniformMutation(pop[i], mData->limits);
		break;
	}
}

//*************************************************** IMMIGRATION *********************************************************
void GA::addImmigrants() {
	std::vector<std::vector<float>> immigrants = Utilities::fillRandomMatrix(mData->immigration_size, mData->chromosome_size, mData->limits);

	extended_population.insert(extended_population.end(), immigrants.begin(), immigrants.end());

}
//*************************************************** MAIN LOOP *********************************************************
void GA::sortAndTruncatePopulation() {
	// Ensure that population and fitness have the same size
	if (extended_population.size() != extended_fitness.size()) {
		std::cerr << "Error: Population and fitness vectors must have the same size." << std::endl;
		return;
	}

	// Create an index array to maintain the original order
	std::vector<int> indexes(extended_population.size());
	std::iota(indexes.begin(), indexes.end(), 0);

	// Sort indexes based on fitness (in descending order)
	std::sort(indexes.begin(), indexes.end(), [&](int a, int b) {
		return extended_fitness[a] > extended_fitness[b];
		});

	// Create a temporary vector to hold the sorted extended_population
	std::vector<std::vector<float>> sortedPopulation(mData->population_size);
	std::vector<float> sortedFitness(mData->population_size);

	// Copy the top 'newSize' rows from the sorted population
	for (int i = 0; i < mData->population_size; ++i) {
		sortedPopulation[i] = extended_population[indexes[i]];
		sortedFitness[i] = extended_fitness[indexes[i]];
	}

	// Replace the original population with the truncated population
	population = std::move(sortedPopulation);

	// Replace the original population with the truncated population
	fitness = std::move(sortedFitness);
}

void GA::insertElitists(std::vector<std::vector<float>> &pop, std::vector<std::vector<float>> elitists) {
	std::vector<std::vector<float>> elite(mData->elitists, std::vector<float>(mData->chromosome_size));

	for (std::vector<float> elitist : elitists) {
		pop.push_back(elitist);
	}
	
}

void GA::Optimize(signal &prediction, std::vector<float> &best_params) {
	std::vector<float> probs(mData->population_size, 0.0f);
	std::vector<std::vector<float>> offspring;
	std::vector<std::vector<float>> elitists(mData->elitists, std::vector<float>(mData->chromosome_size, 0.0f));
	signal nlblock_output;
	fitness = Fitness(population, nlblock_output);
	for (int i = 0; i < mData->generations; i++) {

		for (int j = 0; j < mData->elitists; j++)
			elitists[j] = population[j];
	
		float fit_sum = std::accumulate(fitness.begin(), fitness.end(), 0.0);
		probs = Utilities::divideVectorByConstant(fitness, fit_sum);

		//CROSSOVER
		offspring = reproducePopulation(BLEND_CROSSOVER2, probs);
		extended_population = std::vector<std::vector<float>>(population.begin(), population.end());
		extended_population.insert(extended_population.end(), offspring.begin(), offspring.end());

		//MUTATION
		MutatePopulation(UNIFORM_MUTATION, extended_population);

		//IMMIGRATION
		addImmigrants();

		//ELITISM
		extended_population.insert(extended_population.end(), elitists.begin(), elitists.end());

		extended_fitness = Fitness(extended_population, nlblock_output);
		sortAndTruncatePopulation();
		best_chromosome = population[0];
		std::cout << "Best Fitness value in generation " << i+1 << " is: " << fitness[0] << std::endl;
		std::cout << "Best chromosome in generation " << i + 1 << " is: ";
		for (float value : best_chromosome) {
			std::cout << value << ", ";
		}
		std::cout << std::endl << std::endl;
	}

	best_params = best_chromosome;
	prediction = nlblock_output;

}


GA::~GA()
{
	delete m_WH;
	free(mData);
}


