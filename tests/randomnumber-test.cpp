#include "gtest/gtest.h"
#include "randomnumber.h"

TEST(RandomNumber, test_float_limits)
{
    RandomNumber rand;

    bool error = false;

    for(int i = 0; i < 1000; i++)
    {
        auto r = rand.GetRandomFloat();

        if(r < 0 || r > 1)
        {
            error = true;
            break;
        }
    }

    EXPECT_EQ(error, false);
}

TEST(RandomNumber, test_integer_interval)
{
    RandomNumber rand;

    bool error = false;

    for(int i = 0; i < 1000; i++)
    {
        auto a = -10;
        auto b = 5;
        auto r = rand.GetRandomInt(a, b);

        if(r < a || r > b)
        {
            error = true;
            break;
        }
    }

    EXPECT_EQ(error, false);
}
