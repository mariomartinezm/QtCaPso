#ifndef CAPSO_H
#define CAPSO_H

#include <random>
#include <list>
#include "cellularautomata.h"
#include "particle.h"
#include "latticepoint.h"

class CaPso : public CellularAutomata
{
public:
	enum { EMPTY, PREY, PREDATOR, PREY_PREDATOR };

	CaPso(int width, int height);

	void initialize();

	void nextGen();

	void setInitialAlivePreys(double value);
	void setCompetenceFactor(double value);
	void setPreyMeanOffspring(int value);
	void setPreyReproductionRadius(int value);
	void setPredatorMeanOffspring(int value);
	void setPredatorReproductionRadius(int value);
	void setSearchRadius(int value);
	void setInitialSwarmSize(int value);
	void setPFactor(double value);
	void setGFactor(double value);
	void setMitrationTime(int value);
	void setInitialInertialWeight(double value);
	void setFinalInertiaWeight(double value);

	int numberOfPreys() const;
	int numberOfPredators() const;

private:
	// Containers
	std::vector<unsigned char> mTemp;
	std::vector<unsigned char> mPreyHistogram;
	std::list<Particle> mParticles;

	int mNumberOfPreys, mNumberOfPredators;

	// Use the c++0x implementation of the Mersenne twister RNG
	std::mt19937 mRandom;
	std::uniform_real_distribution<double> mDistReal_0_1;

	// A function pointer that handles transitions
	void (CaPso::*mNextStage)();

	// Model parameters
	double mInitialAlivePercentage;
	double mCompetenceFactor;
	int mPreyMeanOffspring;
	int mPreyReproductionRadius;
	int mPredatorMeanOffspring;
	int mPredatorReproductionRadius;
	int mSearchRadius;
	int NEIGHBORHOOD_SIZE;

	// PSO parameters
	int mInitialSwarmSize;
	double mPFactor;
	double mGFactor;
	int mMigrationTime;
	int mMigrationCount;
	double mInitialInertiaWeight;
	double mCurrentInertiaWeight;
	double mFinalInertiaWeight;
	const double INERTIA_STEP;
	const int mMaxSpeed;

	// Model stages
	void competenceOfPreys();
	void migration();
	void reproductionOfPredators();
	void predatorsDeath();
	void predatorsFeeding();
	void reproductionOfPreys();

	// Misc methods
	void notifyNeighbors(const int& row, const int& col,
						 const bool& death);
};

#endif // CAPSO_H
