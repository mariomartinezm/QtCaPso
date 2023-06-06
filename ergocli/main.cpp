#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <cxxopts.hpp>

#include "Models/localcapso.h"
#include "Models/capsosettings.h"

const int WIDTH         = 512;
const int HEIGHT        = 512;
const int NUM_SEASONS   = 300;

template <typename Iterator>
struct ForEachBlock
{
    void operator() (Iterator first, Iterator last, std::function<void(const float&)> f)
    {
        std::for_each(first, last, f);
    }
};

template <typename Iterator>
void ParalellForEach(Iterator first, Iterator last,
                     unsigned long userThreads,
                     std::function<void(const float&)> f)
{
    unsigned long const length = std::distance(first, last);

    if(!length)
    {
        return;
    }

    unsigned long const hwThreads    = std::thread::hardware_concurrency();
    unsigned long const numThreads   = std::min(hwThreads != 0
                                                ? hwThreads :
                                                2, userThreads);

    std::cout << "Using " << numThreads << " threads.\n";

    unsigned long const blockSize = length / numThreads;
    std::vector<std::thread> threads(numThreads - 1);

    Iterator blockStart = first;
    for(unsigned int i = 0; i < (numThreads - 1); ++i)
    {
        Iterator blockEnd = blockStart;

        std::advance(blockEnd, blockSize);
        threads[i] = std::thread(ForEachBlock<Iterator>(), blockStart, blockEnd, f);
        blockStart = blockEnd;
    }

    ForEachBlock<Iterator>() (blockStart, last, f);

    for(auto& t: threads)
    {
        t.join();
    }
}

void f(const float& d) {
    std::cout << "Working on " << d << "\n";

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
    settings.initialPreyDensity = d;

    LocalCaPso ca(WIDTH, HEIGHT);
    ca.setSettings(settings);
    ca.initialize();

    std::ofstream outputStream("density_" + std::to_string(d) + ".csv");
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

    std::cout << "Simulation for density = " << d << " finished!.\n";
}

int main(int argc, char** argv)
{
    cxxopts::Options options("ergocli",
                             "Ergodicity test of the CaPso model");
    options.add_options()
        ("t,threads", "Number of threads", cxxopts::value<int>())
        ("b,bins",    "Number of bins",    cxxopts::value<int>())
        ("s,seasons", "Number of seasons", cxxopts::value<int>());

    auto result = options.parse(argc, argv);

    std::cout << "Number of threads = " << result["threads"].as<int>() << std::endl;
    std::cout << "Number of bins    = " << result["bins"].as<int>()    << std::endl;
    std::cout << "Number of seasons = " << result["seasons"].as<int>() << std::endl;

    int numBins = result["bins"].as<int>();
    std::vector<float> densities(numBins + 1, 0);

    float delta = (0.7F - 0.3F) / numBins;
    int i       = 0;
    std::generate(densities.begin(), densities.end(), [i, delta] () mutable {
                  return 0.3F + (delta * i++);
                  });

    ParalellForEach(densities.begin(), densities.end(),
                    result["threads"].as<int>(), f);

    return 0;
}
