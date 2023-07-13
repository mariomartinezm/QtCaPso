#include <algorithm>
#include <ctime>
#include <cmath>
#include <memory>
#include <QtGlobal>
#include "globalcapso.h"

using std::list;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;
using std::for_each;
using std::copy;
using std::transform;
using std::uniform_int_distribution;

GlobalCaPso::GlobalCaPso(int width, int height)
    : CellularAutomaton(width, height),
    mPredatorSwarm(1.0f, 2.0f, 0.9f, 10, 3,
                   mLattice, mPreyDensities, mTemp,
                   width, height, PREDATOR, mRandom),
    // Containers
    mTemp(width * height),
    mPreyDensities(width * height),
    mNumberOfPreys(0),
    mNumberOfPredators(0),
    // Model Parameters
    mInitialPreyPercentage(0.5),
    mCompetitionFactor(0.3),
    mCompetitionRadius(5),
    mPreyMeanOffspring(2),
    mPreyReproductionRadius(1),
    mPredatorMeanOffspring(5),
    mPredatorReproductionRadius(3),
    NEIGHBORHOOD_SIZE((2 * mCompetitionRadius + 1)*(2 * mCompetitionRadius + 1) - 1),
    // Pso Parameters
    mInitialSwarmSize(3),
    mMigrationTime(5),
    mMigrationCount(0),
    mInitialInertiaWeight(0.9f),
    mCurrentInertiaWeight(mInitialInertiaWeight),
    mFinalInertiaWeight(0.2f),
    INERTIA_STEP((mInitialInertiaWeight - mFinalInertiaWeight) / mMigrationTime)
{
    initialize();
}

void GlobalCaPso::setInitialPreyPercentage(float value)
{
    mInitialPreyPercentage = value;
}

void GlobalCaPso::setCompetitionFactor(float value)
{
    mCompetitionFactor = value;
}

void GlobalCaPso::setCompetitionRadius(int value)
{
    mCompetitionRadius = value;

    NEIGHBORHOOD_SIZE = (2 * value + 1)*(2 * value + 1) - 1;
}

void GlobalCaPso::setPreyMeanOffspring(int value)
{
    mPreyMeanOffspring = value;
}

void GlobalCaPso::setPreyReproductionRadius(int value)
{
    mPreyReproductionRadius = value;
}

void GlobalCaPso::setPredatorMeanOffspring(int value)
{
    mPredatorMeanOffspring = value;
}

void GlobalCaPso::setPredatorReproductionRadius(int value)
{
    mPredatorReproductionRadius = value;
}

void GlobalCaPso::setInitialSwarmSize(int value)
{
    mInitialSwarmSize = value;
}

void GlobalCaPso::setCognitiveFactor(float value)
{
    mPredatorSwarm.setCognitiveFactor(value);
}

void GlobalCaPso::setSocialFactor(float value)
{
    mPredatorSwarm.setSocialFactor(value);
}

void GlobalCaPso::setMaximumSpeed(int value)
{
    mPredatorSwarm.setMaxSpeed(value);
}

void GlobalCaPso::setMitrationTime(int value)
{
    mMigrationTime = value;
}

void GlobalCaPso::setInitialInertialWeight(float value)
{
    mInitialInertiaWeight = value;
}

void GlobalCaPso::setFinalInertiaWeight(float value)
{
    mFinalInertiaWeight = value;
}

int GlobalCaPso::numberOfPreys() const
{
    return mNumberOfPreys;
}

int GlobalCaPso::numberOfPredators() const
{
    return mNumberOfPredators;
}

void GlobalCaPso::initialize()
{
    clear();

    // Create and render predators
    mPredatorSwarm.initialize(mInitialSwarmSize);

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
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
            if(mRandom.GetRandomFloat() < mInitialPreyPercentage)
            {
                setState(getAddress(row, col), PREY);

                notifyNeighbors(row, col, false);

                mNumberOfPreys++;
            }
        }
    }

    // Obtain the starting best position known by the swarm
    auto it = mPredatorSwarm.begin();
    mBestPosition.row = (*it)->bestPosition.row;
    mBestPosition.col = (*it)->bestPosition.col;

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

            if(mPreyDensities[getAddress(pRow, pCol)] > mPreyDensities[getAddress(mBestPosition.row, mBestPosition.col)])
            {
                mBestPosition.row = pRow;
                mBestPosition.col = pCol;
            }
        }
    });

    // Render the best position
    setState(getAddress(mBestPosition.row, mBestPosition.col), BEST);

    // Reset the migration counter
    mMigrationCount = 0;

    mNextStage = &GlobalCaPso::competitionOfPreys;
}

void GlobalCaPso::clear()
{
    CellularAutomaton::clear();

    // Reset the densities
    transform(mPreyDensities.begin(), mPreyDensities.end(), mPreyDensities.begin(), [](unsigned char)
    {
        return 0;
    });

    mNumberOfPreys = 0;
    mNumberOfPredators = 0;
}

void GlobalCaPso::nextGen()
{
    (this->*mNextStage)();
}

void GlobalCaPso::competitionOfPreys()
{
    copy(mPreyDensities.begin(), mPreyDensities.end(), mTemp.begin());

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
                        mCompetitionFactor / NEIGHBORHOOD_SIZE;

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

    mNextStage = &GlobalCaPso::migration;
}

void GlobalCaPso::migration()
{
    auto validateVector = [this] (int& row, int& col)
    {
        if(abs(row) > mHeight / 2)
        {
            if(row < 0)
            {
                row = row + mHeight;
            }
            else
            {
                row = row - mHeight;
            }
        }

        if(abs(col) > mWidth / 2)
        {
            if(col < 0)
            {
                col = col + mWidth;
            }
            else
            {
                col = col - mWidth;
            }
        }
    };

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

            // Clear the previous cell
            clearState(getAddress(pRow, pCol), PREDATOR);

            double r1 = mRandom.GetRandomFloat();
            double r2 = mRandom.GetRandomFloat();

            int currentVelRow = p->velocity.row;
            int currentVelCol = p->velocity.col;

            validateVector(currentVelRow, currentVelCol);

            int cognitiveVelRow = p->bestPosition.row - pRow;
            int cognitiveVelCol = p->bestPosition.col - pCol;

            validateVector(cognitiveVelRow, cognitiveVelCol);

            int globalVelRow = mBestPosition.row - pRow;
            int globalVelCol = mBestPosition.col - pCol;

            validateVector(globalVelRow, globalVelCol);

            int velRow = (int)(mCurrentInertiaWeight * currentVelRow +
                mPredatorSwarm.cognitiveFactor() * r1 * cognitiveVelRow +
                mPredatorSwarm.socialFactor() * r2 * globalVelRow);
            int velCol = (int)(mCurrentInertiaWeight * currentVelCol +
                mPredatorSwarm.cognitiveFactor() * r1 * cognitiveVelCol +
                mPredatorSwarm.socialFactor() * r2 * globalVelCol);

            // Adjust speed
            double speed = sqrt((double)(velRow * velRow + velCol * velCol));

            while(speed > mPredatorSwarm.maxSpeed())
            {
                velRow *= (int)(0.9);
                velCol *= (int)(0.9);

                speed = sqrt((double)(velRow * velRow + velCol * velCol));
            }

            // Move the particle
            int posRow = pRow + velRow;
            int posCol = pCol + velCol;

            // Adjust the position
            posRow = (mHeight + posRow) % mHeight;
            posCol = (mWidth + posCol) % mWidth;

            p->position.row = posRow;
            p->position.col = posCol;
            p->velocity.row = velRow;
            p->velocity.col = velCol;

            // Render the particle at its new position
            setState(getAddress(posRow, posCol), PREDATOR);

            // If necessary, update the particle's best know position
            if(mPreyDensities[getAddress(pRow, pCol)] < mPreyDensities[getAddress(posRow, posCol)])
            {
                p->bestPosition.row = posRow;
                p->bestPosition.col = posCol;
            }
        }
    });

    // Decrease the inertia weight and increase the migration counter
    mCurrentInertiaWeight -= INERTIA_STEP;
    mMigrationCount++;

    // If migration has ended, point to the next stage and reset the inertia
    // weight and migration count
    if(mMigrationCount == mMigrationTime)
    {
        mNextStage = &GlobalCaPso::reproductionOfPredators;
        mMigrationCount = 0;
        mCurrentInertiaWeight = mInitialInertiaWeight;
    }
}

void GlobalCaPso::reproductionOfPredators()
{
    copy(mLattice.begin(), mLattice.end(), mTemp.begin());

    list<shared_ptr<Particle>> newParticles;

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

            int birthCount = 0;

            while(birthCount < mPredatorMeanOffspring)
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
                    finalCol = mWidth + finalCol % mWidth;
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

    mNextStage = &GlobalCaPso::predatorsDeath;
}

void GlobalCaPso::predatorsDeath()
{
    int currentAddress;

    for(auto it = mPredatorSwarm.begin(); it != mPredatorSwarm.end();)
    {
        currentAddress = getAddress((*it)->position.row, (*it)->position.col);

        if(!checkState(currentAddress, PREY))
        {
            clearState(currentAddress, PREDATOR);
            it = mPredatorSwarm.erase(it);
            mNumberOfPredators--;
        }
        else
        {
            ++it;
        }
    }

    mNextStage = &GlobalCaPso::predation;
}

void GlobalCaPso::predation()
{
    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
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

    if(!mPredatorSwarm.empty())
    {
        // After feeding the current best position becomes invalidated, obtain a new one,
        // but first delete the original.
        clearState(getAddress(mBestPosition.row, mBestPosition.col), BEST);

        auto it = mPredatorSwarm.begin();
        mBestPosition.row = (*it)->bestPosition.row;
        mBestPosition.col = (*it)->bestPosition.col;

        for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
        {
            if(auto p = wp.lock())
            {
                int pBestRow = p->bestPosition.row;
                int pBestCol = p->bestPosition.col;

                if(mPreyDensities[getAddress(pBestRow, pBestCol)] > mPreyDensities[getAddress(mBestPosition.row, mBestPosition.col)])
                {
                    mBestPosition.row = pBestRow;
                    mBestPosition.col = pBestCol;
                }
            }
        });

        // Render the best position
        setState(getAddress(mBestPosition.row, mBestPosition.col), BEST);
    }

    mNextStage = &GlobalCaPso::reproductionOfPreys;
}

void GlobalCaPso::predation2()
{
    copy(mLattice.begin(), mLattice.end(), mTemp.begin());

    for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

            for(int nRow = pRow - 1; nRow <= pRow + 1; nRow++)
            {
                for(int nCol = pCol - 1; nCol <= pCol + 1; nCol++)
                {
                    int finalRow = (mHeight + nRow) % mHeight;
                    int finalCol = (mWidth + nCol) % mWidth;

                    int neighbourAddress = getAddress(finalRow, finalCol);

                    if(checkState(neighbourAddress, PREY))
                    {
                        // Kill the prey
                        clearState(neighbourAddress, PREY);

                        notifyNeighbors(finalRow, finalCol, true);

                        mNumberOfPreys--;
                    }
                }
            }
        }
    });

    if(!mPredatorSwarm.empty())
    {
        // After feeding the current best position becomes invalidated, obtain a new one,
        // but first delete the original.
        clearState(getAddress(mBestPosition.row, mBestPosition.col), BEST);

        auto it = mPredatorSwarm.begin();
        mBestPosition.row = (*it)->bestPosition.row;
        mBestPosition.col = (*it)->bestPosition.col;

        for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
        {
            if(auto p = wp.lock())
            {
                int pBestRow = p->bestPosition.row;
                int pBestCol = p->bestPosition.col;

                if(mPreyDensities[getAddress(pBestRow, pBestCol)] > mPreyDensities[getAddress(mBestPosition.row, mBestPosition.col)])
                {
                    mBestPosition.row = pBestRow;
                    mBestPosition.col = pBestCol;
                }
            }
        });

        // Render the best position
        setState(getAddress(mBestPosition.row, mBestPosition.col), BEST);
    }

    mNextStage = &GlobalCaPso::reproductionOfPreys;
}

void GlobalCaPso::reproductionOfPreys()
{
    copy(mLattice.begin(), mLattice.end(), mTemp.begin());

    int finalRow, finalCol, neighbourAddress;
    int birthCount;

    for(int row = 0; row < mHeight; row++)
    {
        for(int col = 0; col < mWidth; col++)
        {
            if(mTemp[getAddress(row, col)] & PREY)
            {
                birthCount = 0;

                while(birthCount < mPreyMeanOffspring)
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
                        finalCol = mWidth + finalCol % mWidth;
                    }
                    else
                    {
                        finalRow = finalRow % mHeight;
                        finalCol = finalCol % mWidth;
                    }

                    neighbourAddress = getAddress(finalRow, finalCol);

                    if(!checkState(neighbourAddress, PREY))
                    {
                        setState(neighbourAddress, PREY);

                        notifyNeighbors(finalRow, finalCol, false);

                        mNumberOfPreys++;
                    }

                    birthCount++;
                }
            }
        }
    }

    mNextStage = &GlobalCaPso::competitionOfPreys;
}

void GlobalCaPso::notifyNeighbors(const int& row, const int& col, const bool& death)
{
    int finalRow, finalCol;

    for(int nRow = row - mCompetitionRadius; nRow <= row + mCompetitionRadius; nRow++)
    {
        for(int nCol = col - mCompetitionRadius; nCol <= col + mCompetitionRadius; nCol++)
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

bool GlobalCaPso::checkState(int address, State state)
{
    return mLattice[address] & state;
}

void GlobalCaPso::setState(int address, State state)
{
    mLattice[address] |= state;
}

void GlobalCaPso::clearState(int address, State state)
{
    mLattice[address] &= ~state;
}
