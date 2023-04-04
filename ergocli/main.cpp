#include <iostream>
#include <fstream>

#include "Models/localcapso.h"
#include "Models/capsosettings.h"

int main()
{
    const int WIDTH         = 512;
    const int HEIGHT        = 512;
    const int NUM_SEASONS   = 300;
    const int NUM_INTERVALS = 1000;

    const std::string filePreffix = "ergodicity";

    // Initialize settings for the ergodicity experiment
    CaPsoSettings settings;
    settings.competitionFactor = 0.1F;
    settings.finalInertiaWeight = 0;
    settings.fitnessRadius = 10;
    settings.initialInertiaWeight = 0;
    settings.predatorInitialSwarmSize = 100000;
    settings.predatorCognitiveFactor = 0;
    settings.predatorMaxSpeed = 0;
    settings.predatorReproductionRadius = 20;
    settings.predatorReproductiveCapacity = 2;
    settings.predatorSocialFactor = 0;
    settings.predatorSocialRadius = 1;
    settings.preyReproductionRadius = 20;
    settings.preyReproductiveCapacity = 1;

    LocalCaPso ca(WIDTH, HEIGHT);

    float density = 0.3F;
    for(int n = 0; n < NUM_INTERVALS; ++n)
    {
        density += (0.7F - 0.3F) / NUM_INTERVALS;
        settings.initialPreyDensity = density;

        ca.setSettings(settings);
        ca.initialize();

        std::ofstream outputStream(filePreffix + "_" + std::to_string(n) + ".csv");

        outputStream << "Preys,Predators\n";

        for(int i = 0; i < NUM_SEASONS * 10; ++i)
        {
            if(!(i % 10))
            {
                outputStream << ca.numberOfPreys() << ',' << ca.numberOfPredators() << "\n";
            }

            ca.nextGen();
        }

        outputStream.close();

        std::cout << "Simulation " << n + 1 << " finished!.\n";
    }

    return 0;
}
