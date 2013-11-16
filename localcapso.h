#ifndef CAPSO_H
#define CAPSO_H

#include <random>
#include <list>
#include "cellularautomaton.h"
#include "swarm.h"

class LocalCaPso : public CellularAutomaton
{
public:
    enum State { EMPTY, PREY, PREDATOR, PREY_PREDATOR };
    enum Stage { COMPETITION, MIGRATION, REPRODUCTION_OF_PREDATORS,
                 DEATH_OF_PREDATORS, DEATH_OF_PREYS, REPRODUCTION_OF_PREYS };

    LocalCaPso(int width, int height);

    void initialize();

    void nextGen();

    void setPreyInitialDensity(float value);
    void setPreyCompetitionFactor(float value);
    void setPreyReproductiveCapacity(int value);
    void setPreyReproductionRadius(int value);
    void setPredatorReproductiveCapacity(int value);
    void setPredatorReproductionRadius(int value);
    void setPredatorSocialRadius(int value);
    void setFitnessRadius(int value);
    void setPredatorInitialSwarmSize(int value);
    void setPredatorCognitiveFactor(float value);
    void setPredatorSocialFactor(float value);
    void setPredatorMaximumSpeed(int value);
    void setPredatorMigrationTime(int value);
    void setPredatorInitialInertiaWeight(float value);
    void setPredatorFinalInertiaWeight(float value);

    int numberOfPreys() const;
    int numberOfPredators() const;
    float preyBirthRate() const;
    float predatorBirthRate() const;
    int currentStage() const;

    virtual void clear();

private:
    LocalCaPso(const LocalCaPso&);
    LocalCaPso& operator=(const LocalCaPso&);

private:
    Swarm mPredatorSwarm;

    // Containers
    std::vector<unsigned char> mTemp;
    std::vector<unsigned char> mPreyDensities;

    int mNumberOfPreys, mNumberOfPredators;
    float mPreyBirthRate, mPredatorBirthRate;
    int mCurrentStage;

    // Use the c++0x implementation of the Mersenne twister RNG
    std::mt19937 mRandom;
    std::uniform_real_distribution<float> mDistReal_0_1;

    // A function pointer that handles transitions
    void (LocalCaPso::*mNextStage)();

    // Model parameters
    double mPreyInitialDensity;
    double mPreyCompetitionFactor;
    int mPreyReproductiveCapacity;
    int mPreyReproductionRadius;
    int mPredatorReproductiveCapacity;
    int mPredatorReproductionRadius;
    int mPredatorSocialRadius;
    int mFitnessRadius;
    int NEIGHBORHOOD_SIZE;

    // PSO parameters
    int mPredatorInitialSwarmSize;
    int mPredatorMigrationTime;
    int mPredatorMigrationCount;
    float mPredatorInitialInertiaWeight;
    float mPredatorCurrentInertiaWeight;
    float mPredatorFinalInertiaWeight;
    const float INERTIA_STEP;

    // Model stages
    void competitionOfPreys();
    void migration();
    void reproductionOfPredators();
    void predatorsDeath();
    void predation();
    void reproductionOfPreys();

    // Misc methods
    void notifyNeighbors(const int& row, const int& col,
                         const bool& death_birth);
    bool checkState(int address, State state);
    void setState(int address, State state);
    void clearState(int address, State state);
};

#endif // CAPSO_H
