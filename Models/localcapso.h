#ifndef CAPSO_H
#define CAPSO_H

#include <random>
#include <list>
#include "cellularautomaton.h"
#include "swarm.h"
#include "util.h"

class LocalCaPso final : public CellularAutomaton
{
public:
    enum State { EMPTY, PREY, PREDATOR, PREY_PREDATOR };
    enum Stage { COMPETITION, MIGRATION, REPRODUCTION_OF_PREDATORS,
                 DEATH_OF_PREDATORS, DEATH_OF_PREYS, REPRODUCTION_OF_PREYS };

    LocalCaPso(int width, int height);

    void initialize() override;

    void nextGen() override;

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
    float preyDeathProbability() const;
    float predatorDeathProbability() const;
    int currentStage() const;

    virtual void clear() override;

private:
    LocalCaPso(const LocalCaPso&);
    LocalCaPso& operator=(const LocalCaPso&);

private:
    // Containers
    std::vector<unsigned char> mPreyDensities;
    std::vector<unsigned char> mTemp;

    Swarm mPredatorSwarm;

    int mNumberOfPreys, mNumberOfPredators;
    float mPreyBirthRate, mPredatorBirthRate;
    float mPreyDeathProbability, mPredatorDeathProbability;
    int mCurrentStage;

    RandomNumber mRandom;

    // A function pointer that handles transitions
    void (LocalCaPso::*mNextStage)();

    // Model parameters
    double mPreyInitialDensity;
    double mPreyCompetitionFactor;
    int mPreyReproductiveCapacity;
    int mPreyReproductionRadius;
    int mPredatorReproductiveCapacity;
    int mPredatorReproductionRadius;
    int mFitnessRadius;
    int NEIGHBORHOOD_SIZE;

    // PSO parameters
    int mPredatorInitialSwarmSize;
    int mPredatorMigrationTime;
    int mPredatorMigrationCount;
    float mPredatorInitialInertiaWeight;
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
