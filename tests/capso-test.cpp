#include <gtest/gtest.h>
#include "localcapso.h"

TEST(LocalCaPso, test_setSettings)
{
    CaPsoSettings settings;
    settings.initialPreyDensity           = 0.1F;
    settings.competitionFactor            = 0.2F;
    settings.preyReproductionRadius       = 3;
    settings.preyReproductiveCapacity     = 4;
    settings.fitnessRadius                = 5;
    settings.predatorInitialSwarmSize     = 6;
    settings.predatorCognitiveFactor      = 0.7F;
    settings.predatorSocialFactor         = 0.8F;
    settings.predatorMaxSpeed             = 9;
    settings.predatorReproductiveCapacity = 10;
    settings.predatorReproductionRadius   = 11;
    settings.predatorSocialRadius         = 12;
    settings.initialInertiaWeight         = 0.13F;
    settings.finalInertiaWeight           = 0.14F;

    Local ca(512, 512);
    ca.setSettings(settings);
    ca.initialize();

    EXPECT_EQ(ca.numberOfPredators(), 6);
    EXPECT_NEAR(ca.numberOfPreys(), 512 * 512 * 0.1F, 512 * 512 * 0.001F);

    CaPsoSettings caSettings = ca.settings();
    EXPECT_NEAR(caSettings.initialPreyDensity        ,0.1F, 0.0000001F);
    EXPECT_NEAR(caSettings.competitionFactor         ,0.2F, 0.0000001F);
    EXPECT_EQ(caSettings.preyReproductionRadius      ,3);
    EXPECT_EQ(caSettings.preyReproductiveCapacity    ,4);
    EXPECT_EQ(caSettings.fitnessRadius               ,5);
    EXPECT_EQ(caSettings.predatorInitialSwarmSize    ,6);
    EXPECT_NEAR(caSettings.predatorCognitiveFactor   ,0.7F, 0.0000001F);
    EXPECT_NEAR(caSettings.predatorSocialFactor      ,0.8F, 0.0000001F);
    EXPECT_EQ(caSettings.predatorMaxSpeed            ,9);
    EXPECT_EQ(caSettings.predatorReproductiveCapacity,10);
    EXPECT_EQ(caSettings.predatorReproductionRadius  ,11);
    EXPECT_EQ(caSettings.predatorSocialRadius        ,12);
    EXPECT_NEAR(caSettings.initialInertiaWeight      ,0.13, 0.0000001F);
    EXPECT_NEAR(caSettings.finalInertiaWeight        ,0.14, 0.0000001F);
}
