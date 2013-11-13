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

    LocalCaPso(int width, int height);

    void initialize();

    void nextGen();

    void setInitialAlivePreys(float value);
    void setCompetitionFactor(float value);
    void setPreyReproductiveCapacity(int value);
    void setPreyReproductionRadius(int value);
    void setPredatorReproductiveCapacity(int value);
    void setPredatorReproductionRadius(int value);
    void setSocialRadius(int value);
    void setFitnessRadius(int value);
    void setInitialSwarmSize(int value);
    void setCognitiveFactor(float value);
    void setSocialFactor(float value);
    void setMaximumSpeed(int value);
    void setMitrationTime(int value);
    void setInitialInertialWeight(float value);
    void setFinalInertiaWeight(float value);

    int numberOfPreys() const;
    int numberOfPredators() const;

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

    // Use the c++0x implementation of the Mersenne twister RNG
    std::mt19937 mRandom;
    std::uniform_real_distribution<float> mDistReal_0_1;

    // A function pointer that handles transitions
    void (LocalCaPso::*mNextStage)();

    // Model parameters
    double mPreyInitialDensity;
    double mCompetitionFactor;
    int mPreyReproductiveCapacity;
    int mPreyReproductionRadius;
    int mPredatorReproductiveCapacity;
    int mPredatorReproductionRadius;
    int mSocialRadius;
    int mFitnessRadius;
    int NEIGHBORHOOD_SIZE;

    // PSO parameters
    int mInitialSwarmSize;
    int mMigrationTime;
    int mMigrationCount;
    float mInitialInertiaWeight;
    float mCurrentInertiaWeight;
    float mFinalInertiaWeight;
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
