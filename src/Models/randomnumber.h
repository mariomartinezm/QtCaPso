#ifndef RANDOMNUMBER_H
#define RANDOMNUMBER_H

#include <random>
#include "pcg_random.hpp"

class RandomNumber
{
public:
    RandomNumber();
    ~RandomNumber();

    float GetRandomFloat();
    int GetRandomInt(int min, int max);

private:
    pcg32* mRNG;
    std::uniform_real_distribution<float> mRealDistribution;
};

#endif // RANDOMNUMBER_H
