#include <algorithm>
#include <ctime>
#include <cmath>
#include <memory>
#include <QtGlobal>
#include "localcapso.h"

using std::list;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::for_each;
using std::copy;
using std::transform;

LocalCaPso::LocalCaPso(int width, int height)
    : CellularAutomaton(width, height),
    mPreyDensities(width * height),
    mTemp(width * height),
    mPredatorSwarm(1.0f, 2.0f, 0.9f, 10, 3,
                   mLattice, mPreyDensities, mTemp,
                   width, height, PREDATOR, mRandom),
    mNumberOfPreys(0),
    mNumberOfPredators(0),
    mPreyBirthRate(0.0f),
    mPredatorBirthRate(0.0f),
    mPreyDeathProbability(0.0f),
    mPredatorDeathProbability(0.0f),
    mCurrentStage(COMPETITION),
    // Model paremeters
    mPreyInitialDensity(0.3),
    mPreyCompetitionFactor(0.3),
    mPreyReproductiveCapacity(10),
    mPreyReproductionRadius(2),
    mPredatorReproductiveCapacity(10),
    mPredatorReproductionRadius(2),
    mPredatorSocialRadius(3),
    mFitnessRadius(3),
    NEIGHBORHOOD_SIZE((2 * mFitnessRadius + 1)*(2 * mFitnessRadius + 1) - 1),
    // Pso parameters
    mPredatorInitialSwarmSize(3),
    mPredatorMigrationTime(5),
    mPredatorMigrationCount(0),
    mPredatorInitialInertiaWeight(0.9f),
    mPredatorFinalInertiaWeight(0.2f),
    INERTIA_STEP((mPredatorInitialInertiaWeight - mPredatorFinalInertiaWeight) /
                 mPredatorMigrationTime)
{
    initialize();
}

void LocalCaPso::initialize()
{
    clear();

    // Create and render predators
    mPredatorSwarm.initialize(mPredatorInitialSwarmSize);

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(),
             [this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            setState(getAddress(p->position().row(), p->position().col()), PREDATOR);

            mNumberOfPredators++;
        }
    });

    // Randomly create preys
    for(int row = 0; row < mHeight; row++)
    {
        for(int col = 0; col < mWidth; col++)
        {
            if(mRandom.GetRandomFloat() < mPreyInitialDensity)
            {
                setState(getAddress(row, col), PREY);

                notifyNeighbors(row, col, false);

                mNumberOfPreys++;
            }
        }
    }

    // Reset the migration counter
    mPredatorMigrationCount = 0;

    mNextStage = &LocalCaPso::competitionOfPreys;
    mCurrentStage = COMPETITION;
}

void LocalCaPso::clear()
{
    CellularAutomaton::clear();

    // Reset the densities
    std::transform(mPreyDensities.begin(), mPreyDensities.end(),
                   mPreyDensities.begin(), [](unsigned char)
    {
        return 0;
    });

    mNumberOfPreys = 0;
    mNumberOfPredators = 0;
    mPreyBirthRate = 0;
    mPredatorBirthRate = 0;
    mPreyDeathProbability = 0;
    mPredatorDeathProbability = 0;
}

void LocalCaPso::nextGen()
{
    (this->*mNextStage)();
}

void LocalCaPso::setPreyCompetitionFactor(float value)
{
    mPreyCompetitionFactor = value;
}

void LocalCaPso::setPreyInitialDensity(float value)
{
    mPreyInitialDensity = value;
}

void LocalCaPso::setPreyReproductiveCapacity(int value)
{
    mPreyReproductiveCapacity = value;
}

void LocalCaPso::setPreyReproductionRadius(int value)
{
    mPreyReproductionRadius = value;
}

void LocalCaPso::setPredatorReproductiveCapacity(int value)
{
    mPredatorReproductiveCapacity = value;
}

void LocalCaPso::setPredatorReproductionRadius(int value)
{
    mPredatorReproductionRadius = value;
}

void LocalCaPso::setPredatorSocialRadius(int value)
{
    mPredatorSocialRadius = value;
}

void LocalCaPso::setFitnessRadius(int value)
{
    mFitnessRadius = value;

    NEIGHBORHOOD_SIZE = (2 * mFitnessRadius + 1)*(2 * mFitnessRadius + 1) - 1;
}

void LocalCaPso::setPredatorInitialSwarmSize(int value)
{
    mPredatorInitialSwarmSize = value;
}

void LocalCaPso::setPredatorCognitiveFactor(float value)
{
    mPredatorSwarm.setCognitiveFactor(value);
}

void LocalCaPso::setPredatorSocialFactor(float value)
{
    mPredatorSwarm.setSocialFactor(value);
}

void LocalCaPso::setPredatorMaximumSpeed(int value)
{
    mPredatorSwarm.setMaxSpeed(value);
}

void LocalCaPso::setPredatorMigrationTime(int value)
{
    mPredatorMigrationTime = value;
}

void LocalCaPso::setPredatorInitialInertiaWeight(float value)
{
    mPredatorInitialInertiaWeight = value;
}

void LocalCaPso::setPredatorFinalInertiaWeight(float value)
{
    mPredatorFinalInertiaWeight = value;
}

int LocalCaPso::numberOfPreys() const
{
    return mNumberOfPreys;
}

int LocalCaPso::numberOfPredators() const
{
    return mNumberOfPredators;
}

float LocalCaPso::preyBirthRate() const
{
    return mPreyBirthRate;
}

float LocalCaPso::predatorBirthRate() const
{
    return mPredatorBirthRate;
}

float LocalCaPso::preyDeathProbability() const
{
    return mPreyDeathProbability;
}

float LocalCaPso::predatorDeathProbability() const
{
    return mPredatorDeathProbability;
}

int LocalCaPso::currentStage() const
{
    return mCurrentStage;
}

void LocalCaPso::competitionOfPreys()
{
    std::copy(mPreyDensities.begin(), mPreyDensities.end(), mTemp.begin());

    int currentAddress;
    double deathProbability;

    for(int row = 0; row < mHeight; row++)
    {
        for(int col = 0; col < mWidth; col++)
        {
            currentAddress = getAddress(row, col);

            if(checkState(currentAddress, PREY))
            {
                deathProbability = mTemp[currentAddress] *
                    mPreyCompetitionFactor / NEIGHBORHOOD_SIZE;

                if(mRandom.GetRandomFloat() <= deathProbability)
                {
                    // Only kill the prey
                    clearState(currentAddress, PREY);

                    notifyNeighbors(row, col, true);

                    mNumberOfPreys--;
                }
            }
        }
    }

    mNextStage = &LocalCaPso::migration;
    mCurrentStage = MIGRATION;
}

void LocalCaPso::migration()
{
    // Update the positions of all predators
    mPredatorSwarm.nextGen();

    // Decrease the inertia weight and increase the migration counter
    float weight = mPredatorSwarm.inertiaWeight() - INERTIA_STEP;
    mPredatorSwarm.setInertiaWeight(weight);
    mPredatorMigrationCount++;

    // If migration has ended, point to the next stage and reset the inertia
    // weight and migration count
    if(mPredatorMigrationCount == mPredatorMigrationTime)
    {
        mNextStage = &LocalCaPso::reproductionOfPredators;
        mCurrentStage = REPRODUCTION_OF_PREDATORS;
        mPredatorMigrationCount = 0;
        mPredatorSwarm.setInertiaWeight(mPredatorInitialInertiaWeight);
    }
}

void LocalCaPso::reproductionOfPredators()
{
    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

    list<shared_ptr<Particle>> newParticles;

    int initialNumberOfPredators = mNumberOfPredators;

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position().row();
            int pCol = p->position().col();

            int birthCount = 0;

            while(birthCount < mPredatorReproductiveCapacity)
            {
                // Obtain an offset
                int finalRow = mRandom.GetRandomInt(-mPredatorReproductionRadius, mPredatorReproductionRadius);
                int finalCol = mRandom.GetRandomInt(-mPredatorReproductionRadius, mPredatorReproductionRadius);

                if(finalRow == 0 && finalCol == 0)
                {
                    continue;
                }

                // Get relative coordinates
                finalRow += pRow;
                finalCol += pCol;

                // Get final coordinates
                if (finalRow < 0 && finalCol < 0)
                {
                    finalRow = mHeight + finalRow % mHeight;
                    finalCol = mWidth + finalCol % mWidth;
                }
                else if (finalRow < 0 && finalCol >= 0)
                {
                    finalRow = mHeight + finalRow % mHeight;
                    finalCol = finalCol % mWidth;
                }
                else if (finalRow >= 0 && finalCol < 0)
                {
                    finalRow = finalRow % mHeight;
                    finalCol = mWidth + finalCol % mHeight;
                }
                else
                {
                    finalRow = finalRow % mHeight;
                    finalCol = finalCol % mWidth;
                }

                if(!checkState(getAddress(finalRow, finalCol), PREDATOR))
                {
                    // Create a new particle
                    auto particle = make_shared<Particle>();
                    particle->setPosition(finalRow, finalCol);
                    particle->setBestPosition(p->bestPosition().row(), p->bestPosition().col());

                    setState(getAddress(finalRow, finalCol), PREDATOR);

                    newParticles.push_back(particle);

                    mNumberOfPredators++;
                }

                birthCount++;
            }
        }
    });

    mPredatorSwarm.add(newParticles);

    int numberOfBirths = mNumberOfPredators - initialNumberOfPredators;

    mPredatorBirthRate = static_cast<float>(numberOfBirths) / mLattice.size();

    mNextStage = &LocalCaPso::predatorsDeath;
    mCurrentStage = DEATH_OF_PREDATORS;
}

void LocalCaPso::predatorsDeath()
{
    int currentAddress;

    int initialNumberOfPredators = mNumberOfPredators;

    for(auto it = mPredatorSwarm.begin(); it != mPredatorSwarm.end();)
    {
        currentAddress = getAddress((*it)->position().row(), (*it)->position().col());

        if(!checkState(currentAddress, PREY))
        {
            clearState(currentAddress, PREDATOR);

            // Since the iterator is invalidated when erase() is called, we must
            // use its return value (an iterator pointing to the element following
            // the element that has just been erased) to keep a valid iterator.
            it = mPredatorSwarm.erase(it);
            mNumberOfPredators--;
        }
        else
        {
            it++;
        }
    }

    int numberOfDeaths = initialNumberOfPredators - mNumberOfPredators;

    mPredatorDeathProbability = static_cast<float>(numberOfDeaths) /
            initialNumberOfPredators;

    mNextStage = &LocalCaPso::predation;
    mCurrentStage = DEATH_OF_PREYS;
}

void LocalCaPso::predation()
{
    int initialNumberOfPreys = mNumberOfPreys;

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position().row();
            int pCol = p->position().col();

            int currentAddress = getAddress(pRow, pCol);

            // Kill the prey in the current cell
            if(checkState(currentAddress, PREY))
            {
                clearState(currentAddress, PREY);

                notifyNeighbors(pRow, pCol, true);

                mNumberOfPreys--;
            }
        }
    });

    int numberOfDeaths = initialNumberOfPreys - mNumberOfPreys;

    mPreyDeathProbability = static_cast<float>(numberOfDeaths) /
            initialNumberOfPreys;

    mNextStage = &LocalCaPso::reproductionOfPreys;
    mCurrentStage = REPRODUCTION_OF_PREYS;
}

void LocalCaPso::reproductionOfPreys()
{
    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

    int finalRow, finalCol, neighbourAddress;
    int birthCount, initialNumberOfPreys = mNumberOfPreys;

    for(int row = 0; row < mHeight; row++)
    {
        for(int col = 0; col < mWidth; col++)
        {
            if(mTemp[getAddress(row, col)] & PREY)
            {
                birthCount = 0;

                while(birthCount < mPreyReproductiveCapacity)
                {
                    // Obtain an offset
                    finalRow = mRandom.GetRandomInt(-mPreyReproductionRadius, mPreyReproductionRadius);
                    finalCol = mRandom.GetRandomInt(-mPreyReproductionRadius, mPreyReproductionRadius);

                    if(finalRow == 0 && finalCol == 0)
                    {
                        continue;
                    }

                    // Get relative coordinates
                    finalRow += row;
                    finalCol += col;

                    // Get final coordinates
                    if (finalRow < 0 && finalCol < 0)
                    {
                        finalRow = mHeight + finalRow % mHeight;
                        finalCol = mWidth + finalCol % mWidth;
                    }
                    else if (finalRow < 0 && finalCol >= 0)
                    {
                        finalRow = mHeight + finalRow % mHeight;
                        finalCol = finalCol % mWidth;
                    }
                    else if (finalRow >= 0 && finalCol < 0)
                    {
                        finalRow = finalRow % mHeight;
                        finalCol = mWidth + finalCol % mHeight;
                    }
                    else
                    {
                        finalRow = finalRow % mHeight;
                        finalCol = finalCol % mWidth;
                    }

                    /*finalRow = (mWidth + finalRow) % mWidth;
                    finalCol = (mHeight + finalCol) % mHeight;*/

                    neighbourAddress = getAddress(finalRow, finalCol);

                    if(!(mLattice[neighbourAddress] & PREY))
                    {
                        mLattice[neighbourAddress] |= PREY;

                        notifyNeighbors(finalRow, finalCol, false);

                        mNumberOfPreys++;
                    }

                    birthCount++;
                }
            }
        }
    }

    int numberOfBirths = mNumberOfPreys - initialNumberOfPreys;

    mPreyBirthRate = static_cast<float>(numberOfBirths) / mLattice.size();

    mNextStage = &LocalCaPso::competitionOfPreys;
    mCurrentStage = COMPETITION;
}

void LocalCaPso::notifyNeighbors(const int& row, const int& col, const bool& death)
{
    int finalRow, finalCol;

    for(int nRow = row - mFitnessRadius; nRow <= row + mFitnessRadius; nRow++)
    {
        for(int nCol = col - mFitnessRadius; nCol <= col + mFitnessRadius; nCol++)
        {
            if(nRow == row && nCol == col)
            {
                continue;
            }

            finalRow = (mHeight + nRow) % mHeight;
            finalCol = (mWidth + nCol) % mWidth;

            if(death)
            {
                mPreyDensities[getAddress(finalRow, finalCol)]--;
            }
            else
            {
                mPreyDensities[getAddress(finalRow, finalCol)]++;
            }
        }
    }
}

bool LocalCaPso::checkState(int address, State state)
{
    return mLattice[address] & state;
}

void LocalCaPso::setState(int address, State state)
{
    mLattice[address] |= state;
}

void LocalCaPso::clearState(int address, State state)
{
    mLattice[address] &= ~state;
}
