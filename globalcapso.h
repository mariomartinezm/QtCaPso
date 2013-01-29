#ifndef GLOBALCAPSO_H
#define GLOBALCAPSO_H

#include <random>
#include <list>
#include "cellularautomaton.h"
#include "swarm.h"

class GlobalCaPso : public CellularAutomaton
{
public:
	enum State {EMPTY, PREY, PREDATOR, PREY_PREDATOR, BEST = 4};

	GlobalCaPso(int width, int height);

	void setInitialPreyPercentage(float value);
    void setCompetitionFactor(float value);
    void setCompetitionRadius(int value);
	void setPreyMeanOffspring(int value);
	void setPreyReproductionRadius(int value);
	void setPredatorMeanOffspring(int value);
	void setPredatorReproductionRadius(int value);
	void setInitialSwarmSize(int value);
	void setCognitiveFactor(float value);
	void setSocialFactor(float value);
	void setMaximumSpeed(int value);
	void setMitrationTime(int value);
	void setInitialInertialWeight(float value);
	void setFinalInertiaWeight(float value);

	int numberOfPreys() const;
	int numberOfPredators() const;

	void initialize();
    virtual void clear();
	void nextGen();

private:
	GlobalCaPso(const GlobalCaPso&);
	GlobalCaPso& operator=(const GlobalCaPso&);

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
	void (GlobalCaPso::*mNextStage)();

	// Model parameters
	double mInitialPreyPercentage;
    double mCompetitionFactor;
    int mCompetitionRadius;
	int mPreyMeanOffspring;
	int mPreyReproductionRadius;
	int mPredatorMeanOffspring;
	int mPredatorReproductionRadius;
	int NEIGHBORHOOD_SIZE;

	// PSO parameters
	int mInitialSwarmSize;
	int mMigrationTime;
	int mMigrationCount;
	float mInitialInertiaWeight;
	float mCurrentInertiaWeight;
	float mFinalInertiaWeight;
	const float INERTIA_STEP;
	LatticePoint mBestPosition;

	// Model Stages
    void competitionOfPreys();
	void migration();
	void reproductionOfPredators();
	void predatorsDeath();
	void predation();
	void predation2();
	void reproductionOfPreys();

	// Misc methods
	void notifyNeighbors(const int& row, const int& col,
						 const bool& death_birth);

	bool checkState(int address, State state);
	void setState(int address, State state);
	void clearState(int address, State state);
};

#endif // GLOBALCAPSO_H
