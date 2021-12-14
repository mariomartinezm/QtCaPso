#ifndef RANDOMNUMBER_H
#define RANDOMNUMBER_H

#include <memory>
#include <random>
#include "pcg_random.hpp"

class RandomNumber
{
public:
    RandomNumber();

    float GetRandomFloat();
    int GetRandomInt(int min, int max);

private:
    std::unique_ptr<pcg32> mRNG;
    std::uniform_real_distribution<float> mRealDistribution;
};

#endif // RANDOMNUMBER_H
