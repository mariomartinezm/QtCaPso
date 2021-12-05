#include "randomnumber.h"

#include <chrono>

RandomNumber::RandomNumber()
    : mRealDistribution(0.0, 1.0)
{
    // Initialize PRNG
    std::random_device rd;
    unsigned seed;

    // Check if the implementation provides a usable random_device
    if (0 != rd.entropy())
    {
        seed = rd();
    }
    else
    {
        // No random device available, seed using the system clock
        seed = static_cast<unsigned> (std::chrono::system_clock::now().time_since_epoch().count());
    }

    mRNG = new pcg32(seed);
}

RandomNumber::~RandomNumber()
{
    delete mRNG;
}

float RandomNumber::GetRandomFloat()
{
    return mRealDistribution(*mRNG);
}

int RandomNumber::GetRandomInt(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(*mRNG);
}
