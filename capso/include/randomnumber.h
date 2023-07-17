#pragma once

#include <memory>
#include <random>

#include "pcg_random.hpp"
#include "capso_export.h"


class CAPSO_EXPORT RandomNumber
{
public:
    RandomNumber();

    float GetRandomFloat();
    int GetRandomInt(int min, int max);

private:
    std::unique_ptr<pcg32> mRNG;
    std::uniform_real_distribution<float> mRealDistribution;
};
