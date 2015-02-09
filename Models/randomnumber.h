#ifndef RANDOMNUMBER_H
#define RANDOMNUMBER_H

#include<random>

class RandomNumber
{
public:
    RandomNumber();
    float GetRandomFloat();
    int GetRandomInt(int min, int max);

private:
    // Use the c++0x implementation of the Mersenne twister RNG
    std::mt19937 mRNG;
    std::uniform_real_distribution<float> mRealDistribution;
};

#endif // RANDOMNUMBER_H