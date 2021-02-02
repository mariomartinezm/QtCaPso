#include "randomnumber.h"

#include<ctime>

RandomNumber::RandomNumber()
    : mRNG(static_cast<unsigned int>(time(NULL))),
      mRealDistribution(0.0, 1.0)
{

}

float RandomNumber::GetRandomFloat()
{
    return mRealDistribution(mRNG);
}

int RandomNumber::GetRandomInt(int min, int max)
{
    return std::uniform_int_distribution<int>{min, max}(mRNG);
}