#include "Scorer.h"
#include "Utilities.h"

void Scorer::setSignals(const std::vector<float>& pred, const std::vector<float>& target)
{
    this->prediction = pred;
    this->target = target;
}
void Scorer::setSignals(const std::vector<float>& pred, std::vector<float>& target, const std::vector<float> weights)
{
    this->prediction = pred;
    this->target = target;

    int sections = weights.size();
    float resolution = static_cast<float>(1) / sections;
    float rmse_weighted;
    float min = Utilities::getMinValue(target);
    float max = Utilities::getMaxValue(target);
    float max_amp = max - min;
    this->amplitude_weights = weights;
    this->borders=std::vector<float>(sections, 0.0f);
    std::vector<float> sorted_target(target);
    std::sort(sorted_target.begin(), sorted_target.end());
    int j = 1;
    for (int i = 0; i < target.size(); i++) {
        if (sorted_target[i] >= min + resolution * j * max_amp) {
            borders[j - 1] = sorted_target[i];
            j++;
            if (j > sections)
                break;
        }
    }
}

float Scorer::getAverage(const std::vector<float>& s1)
{
    float sum = std::accumulate(s1.begin(), s1.end(), 0.0f);
    return sum / static_cast<float>(s1.size());

}

std::vector<float> Scorer::getDiffs(std::vector<float>& s1, std::vector<float>& s2)
{   
    std::vector<float> diffs = Utilities::subtractVectors(s1, s2);
    return diffs;
}

float Scorer::getAverageDiff()
{
    std::vector<float> diffs = getDiffs(target,prediction);
    
    return getAverage(diffs);
}

float Scorer::getESR()
{
    std::vector<float> diffs = getDiffs(target, prediction);
    std::vector<float> diffs_squared = Utilities::powerVector(diffs, 2);
    std::vector<float> target_squared = Utilities::powerVector(target, 2);
    error = std::accumulate(diffs_squared.begin(), diffs_squared.end(), 0.0f)/std::accumulate(target_squared.begin(), target_squared.end(), 0.0f);
    return error;
}

float Scorer::getMSE()
{
    std::vector<float> diffs = getDiffs(target, prediction);
    std::vector<float> diffs_squared = Utilities::powerVector(diffs, 2);
    error = std::accumulate(diffs_squared.begin(), diffs_squared.end(), 0.0f) / diffs_squared.size();

    return error;
}

float Scorer::getRMSE()
{
    error = std::sqrt(getMSE());
    return error;
}

float Scorer::getRMSE_weighted()
{
    error = 0.0f;
    int j = 0;
    float diff_squared=0.0f;
    std::vector<float> sorted_target(target);
    std::vector<float> sorted_pred(prediction);
    std::sort(sorted_target.begin(), sorted_target.end());
    std::sort(sorted_pred.begin(), sorted_pred.end());
    for (int i = 0; i < target.size(); i++) {
        if (sorted_target[i] < borders[j]) {
            diff_squared = std::pow(sorted_target[i] - sorted_pred[i], 2);
            error += std::sqrt(diff_squared) * amplitude_weights[j];
        }
        else {
            j++;
            if (j == borders.size())
                break;
        }

    }
    return error/target.size();
}

float Scorer::getScore()
{
    score = 1 / error;
    return score;
}

float Scorer::getNegativeScore() {
    score = -error;
    return score;
}


