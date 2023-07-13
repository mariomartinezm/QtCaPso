#include <chrono>
#include "randomnumber.h"

RandomNumber::RandomNumber()
    : mRealDistribution(0.0, 1.0)
{
    // Initialize PRNG
    std::random_device rd;

    // Check if the implementation provides a usable random_device
    if (0 != rd.entropy())
    {
        pcg_extras::seed_seq_from<std::random_device> seed_source;
        mRNG = std::make_unique<pcg32>(seed_source);
    }
    else
    {
        // No random device available, seed using the system clock. Note however
        // that in MinGW std::chrono::high_resolution_clock is not accurate
        // enough. Thus threads that start close to one another might have
        // the same seed. A workaround seems to use std::chrono::steady_clock
        // instead.
        uint64_t seed = static_cast<unsigned> (std::chrono::steady_clock::now().time_since_epoch().count());
        mRNG = std::make_unique<pcg32>(seed);
    }
}

float RandomNumber::GetRandomFloat()
{
    return mRealDistribution(*mRNG);
}

int RandomNumber::GetRandomInt(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(*mRNG);
}
