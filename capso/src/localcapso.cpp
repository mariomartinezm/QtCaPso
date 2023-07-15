#include <algorithm>
#include <ctime>
#include <cmath>
#include <memory>
#include "localcapso.h"

using std::list;
using std::weak_ptr;
using std::shared_ptr;
using std::make_shared;
using std::for_each;
using std::copy;
using std::transform;

capso::Local::Local(int width, int height)
    : CellularAutomaton(width, height),
    mPreyDensities(width * height),
    mTemp(width * height),
    mPredatorSwarm(1.0f, 2.0f, 0.9f, 10, 3,
                   mLattice, mPreyDensities, mTemp,
                   width, height, PREDATOR, mRandom)
{
    initialize();
}

void capso::Local::initialize()
{
    clear();

    // Create and render predators
    mPredatorSwarm.initialize(mPredatorInitialSwarmSize);

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(),
             [this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            setState(getAddress(p->position.row, p->position.col), PREDATOR);

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

    mNextStage = &capso::Local::competitionOfPreys;
    mCurrentStage = COMPETITION;
}

void capso::Local::clear()
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

void capso::Local::nextGen()
{
    (this->*mNextStage)();
}

void capso::Local::setPredatorMigrationTime(int value)
{
    mPredatorMigrationTime = value;
}

void capso::Local::setSettings(const CaPsoSettings &settings)
{
    mPreyInitialDensity           = settings.initialPreyDensity;
    mPreyCompetitionFactor        = settings.competitionFactor;
    mPreyReproductionRadius       = settings.preyReproductionRadius;
    mPreyReproductiveCapacity     = settings.preyReproductiveCapacity;
    mFitnessRadius                = settings.fitnessRadius;
    mPredatorInitialSwarmSize     = settings.predatorInitialSwarmSize;
    mPredatorSwarm.setCognitiveFactor(settings.predatorCognitiveFactor);
    mPredatorSwarm.setSocialFactor(settings.predatorSocialFactor);
    mPredatorSwarm.setMaxSpeed(settings.predatorMaxSpeed);
    mPredatorReproductiveCapacity = settings.predatorReproductiveCapacity;
    mPredatorReproductionRadius   = settings.predatorReproductionRadius;
    mPredatorSwarm.setSocialRadius(settings.predatorSocialRadius);
    mPredatorInitialInertiaWeight = settings.initialInertiaWeight;
    mPredatorFinalInertiaWeight   = settings.finalInertiaWeight;

    NEIGHBORHOOD_SIZE = (2 * mFitnessRadius + 1) * (2 * mFitnessRadius + 1) - 1;
}

CaPsoSettings capso::Local::settings() const
{
    CaPsoSettings settings;
    settings.initialPreyDensity           = mPreyInitialDensity;
    settings.competitionFactor            = mPreyCompetitionFactor;
    settings.preyReproductionRadius       = mPreyReproductionRadius;
    settings.preyReproductiveCapacity     = mPreyReproductiveCapacity;
    settings.fitnessRadius                = mFitnessRadius;
    settings.predatorInitialSwarmSize     = mPredatorInitialSwarmSize;
    settings.predatorCognitiveFactor      = mPredatorSwarm.cognitiveFactor();
    settings.predatorSocialFactor         = mPredatorSwarm.socialFactor();
    settings.predatorMaxSpeed             = mPredatorSwarm.maxSpeed();
    settings.predatorReproductiveCapacity = mPredatorReproductiveCapacity;
    settings.predatorReproductionRadius   = mPredatorReproductionRadius;
    settings.predatorSocialRadius         = mPredatorSwarm.socialRadius();
    settings.initialInertiaWeight         = mPredatorInitialInertiaWeight;
    settings.finalInertiaWeight           = mPredatorFinalInertiaWeight;

    return settings;
}

int capso::Local::numberOfPreys() const
{
    return mNumberOfPreys;
}

int capso::Local::numberOfPredators() const
{
    return mNumberOfPredators;
}

float capso::Local::preyBirthRate() const
{
    return mPreyBirthRate;
}

float capso::Local::predatorBirthRate() const
{
    return mPredatorBirthRate;
}

float capso::Local::preyDeathProbability() const
{
    return mPreyDeathProbability;
}

float capso::Local::predatorDeathProbability() const
{
    return mPredatorDeathProbability;
}

int capso::Local::currentStage() const
{
    return mCurrentStage;
}

void capso::Local::competitionOfPreys()
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

    mNextStage = &capso::Local::migration;
    mCurrentStage = MIGRATION;
}

void capso::Local::migration()
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
        mNextStage = &capso::Local::reproductionOfPredators;
        mCurrentStage = REPRODUCTION_OF_PREDATORS;
        mPredatorMigrationCount = 0;
        mPredatorSwarm.setInertiaWeight(mPredatorInitialInertiaWeight);
    }
}

void capso::Local::reproductionOfPredators()
{
    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

    list<shared_ptr<Particle>> newParticles;

    int initialNumberOfPredators = mNumberOfPredators;

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

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
                    particle->position.row = finalRow;
                    particle->position.col = finalCol;
                    particle->bestPosition.row = p->bestPosition.row;
                    particle->bestPosition.col = p->bestPosition.col;

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

    mNextStage = &capso::Local::predatorsDeath;
    mCurrentStage = DEATH_OF_PREDATORS;
}

void capso::Local::predatorsDeath()
{
    int currentAddress;

    int initialNumberOfPredators = mNumberOfPredators;

    for(auto it = mPredatorSwarm.begin(); it != mPredatorSwarm.end();)
    {
        currentAddress = getAddress((*it)->position.row, (*it)->position.col);

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

    mNextStage = &capso::Local::predation;
    mCurrentStage = DEATH_OF_PREYS;
}

void capso::Local::predation()
{
    int initialNumberOfPreys = mNumberOfPreys;

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

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

    mNextStage = &capso::Local::reproductionOfPreys;
    mCurrentStage = REPRODUCTION_OF_PREYS;
}

void capso::Local::reproductionOfPreys()
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

    mNextStage = &capso::Local::competitionOfPreys;
    mCurrentStage = COMPETITION;
}

void capso::Local::notifyNeighbors(const int& row, const int& col, const bool& death)
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

bool capso::Local::checkState(int address, State state)
{
    return mLattice[address] & state;
}

void capso::Local::setState(int address, State state)
{
    mLattice[address] |= state;
}

void capso::Local::clearState(int address, State state)
{
    mLattice[address] &= ~state;
}
