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

LocalCaPso::LocalCaPso(int width, int height)
	: CellularAutomaton(width, height),
	mPredatorSwarm(1.0, 2.0, 10),
	// Containers
	mTemp(width * height),
	mPreyDensities(width * height),
	mNumberOfPreys(0),
	mNumberOfPredators(0),
	mRandom(time(NULL)),
	// Real uniform distribution
	mDistReal_0_1(0.0, 1.0),
	// Model paremeters
	mInitialAlivePercentage(0.3),
	mCompetenceFactor(0.3),
	mPreyMeanOffspring(10),
	mPreyReproductionRadius(2),
	mPredatorMeanOffspring(10),
	mPredatorReproductionRadius(2),
	mSearchRadius(3),
	NEIGHBORHOOD_SIZE((2 * mSearchRadius + 1)*(2 * mSearchRadius + 1) - 1),
	// Pso parameters
	mInitialSwarmSize(3),
	mMigrationTime(5),
	mMigrationCount(0),
	mInitialInertiaWeight(0.9),
	mCurrentInertiaWeight(mInitialInertiaWeight),
	mFinalInertiaWeight(0.2),
	INERTIA_STEP((mInitialInertiaWeight - mFinalInertiaWeight)  / mMigrationTime)
{
    initialize();
}

void LocalCaPso::initialize()
{
	clear();

	// Create and render predators
	mPredatorSwarm.initialize(mInitialSwarmSize, mWidth, mHeight, mRandom);

	for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [this](weak_ptr<Particle> wp)
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
			if(mDistReal_0_1(mRandom) < mInitialAlivePercentage)
			{
				setState(getAddress(row, col), PREY);

				notifyNeighbors(row, col, false);
				
				mNumberOfPreys++;
			}
		}
	}

	// Reset the migration counter
	mMigrationCount = 0;

	mNextStage = &LocalCaPso::competenceOfPreys;
}

void LocalCaPso::clear()
{
	CellularAutomaton::clear();

	// Reset the densities
	std::transform(mPreyDensities.begin(), mPreyDensities.end(), mPreyDensities.begin(), [](unsigned char)
	{
		return 0;
	});

	mNumberOfPreys = 0;
	mNumberOfPredators = 0;
}

void LocalCaPso::nextGen()
{
    (this->*mNextStage)();
}

void LocalCaPso::setCompetenceFactor(float value)
{
    mCompetenceFactor = value;
}

void LocalCaPso::setInitialAlivePreys(float value)
{
    mInitialAlivePercentage = value;
}

void LocalCaPso::setPreyMeanOffspring(int value)
{
    mPreyMeanOffspring = value;
}

void LocalCaPso::setPreyReproductionRadius(int value)
{
    mPreyReproductionRadius = value;
}

void LocalCaPso::setPredatorMeanOffspring(int value)
{
    mPredatorMeanOffspring = value;
}

void LocalCaPso::setPredatorReproductionRadius(int value)
{
    mPredatorReproductionRadius = value;
}
 
void LocalCaPso::setSearchRadius(int value)
{
    mSearchRadius = value;

	NEIGHBORHOOD_SIZE = (2 * mSearchRadius + 1)*(2 * mSearchRadius + 1) - 1;
}

void LocalCaPso::setInitialSwarmSize(int value)
{
    mInitialSwarmSize = value;
}

void LocalCaPso::setCognitiveFactor(float value)
{
	mPredatorSwarm.setCognitiveFactor(value);
}

void LocalCaPso::setSocialFactor(float value)
{
	mPredatorSwarm.setSocialFactor(value);
}

void LocalCaPso::setMaximumSpeed(int value)
{
	mPredatorSwarm.setMaxSpeed(value);
}

void LocalCaPso::setMitrationTime(int value)
{
    mMigrationTime = value;
}

void LocalCaPso::setInitialInertialWeight(float value)
{
    mInitialInertiaWeight = value;
}

void LocalCaPso::setFinalInertiaWeight(float value)
{
    mFinalInertiaWeight = value;
}

int LocalCaPso::numberOfPreys() const
{
    return mNumberOfPreys;
}

int LocalCaPso::numberOfPredators() const
{
    return mNumberOfPredators;
}

void LocalCaPso::competenceOfPreys()
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
					mCompetenceFactor / NEIGHBORHOOD_SIZE;

				if(mDistReal_0_1(mRandom) <= deathProbability)
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
}

void LocalCaPso::migration()
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

	copy(mLattice.begin(), mLattice.end(), mTemp.begin());

	for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
	{
		if(auto p = wp.lock())
		{
			int pRow = p->position().row();
			int pCol = p->position().col();

			int bestRow = pRow;
			int bestCol = pCol;

			int bestAddress = getAddress(bestRow, bestCol);

			// Clear the previous position of the predator
			clearState(bestAddress, PREDATOR);

			// Get the best position among the neighbors of the predator
			for(int nRow = pRow - mSearchRadius; nRow <= pRow + mSearchRadius; nRow++)
			{
				for(int nCol = pCol - mSearchRadius; nCol <= pCol + mSearchRadius; nCol++)
				{
					if(nRow == pRow && nCol == pCol)
					{
						continue;
					}

					int finalRow = (mHeight + nRow) % mHeight;
					int finalCol = (mWidth + nCol) % mWidth;

					int neighbourAddress = getAddress(finalRow, finalCol);

					if(checkState(neighbourAddress, PREDATOR))
					{
						// Update if necessary
						if(mPreyDensities[bestAddress] < mPreyDensities[neighbourAddress])
						{
							bestRow = nRow;
							bestCol = nCol;

							bestAddress = neighbourAddress;
						}
					}
				}
			}

			float r1 = mDistReal_0_1(mRandom);
			float r2 = mDistReal_0_1(mRandom);

			// Get the new velocity
			int velRow = (int)(mCurrentInertiaWeight * p->velocity().row() +
				mPredatorSwarm.cognitiveFactor() * r1 * (p->bestPosition().row() - pRow) +
				mPredatorSwarm.socialFactor() * r2 * (bestRow - pRow));
			int velCol = (int)(mCurrentInertiaWeight * p->velocity().col() +
				mPredatorSwarm.cognitiveFactor() * r1 * (p->bestPosition().col() - pCol) +
				mPredatorSwarm.socialFactor() * r2 * (bestCol - pCol));

			// Adjust speed
			double speed = sqrt((double)(velRow * velRow + velCol * velCol));

			while(speed > mPredatorSwarm.maxSpeed())
			{
				velRow *= 0.9;
				velCol *= 0.9;

				speed = sqrt((double)(velRow * velRow + velCol * velCol));
			}

			// Move the particle
			int posRow = pRow + velRow;
			int posCol = pCol + velCol;

			// Adjust position
			posRow = (mHeight + posRow) % mHeight;
			posCol = (mWidth + posCol) % mWidth;

			// Check if destination is not already occupied
			if(!checkState(getAddress(posRow, posCol), PREDATOR))
			{
				p->setPosition(posRow, posCol);
				p->setVelocity(velRow, velCol);

				// Render the particle at its new position
				setState(getAddress(posRow, posCol), PREDATOR);

				// If necessary, update the particle's best known position
				if(mPreyDensities[getAddress(p->bestPosition().row(), p->bestPosition().col())] < 
					mPreyDensities[getAddress(posRow, posCol)])
				{
					p->setBestPosition(posRow, posCol);
				}
			}
			else
			{
				// Restore the particle's previous position
				setState(getAddress(pRow, pCol), PREDATOR);
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
		mNextStage = &LocalCaPso::reproductionOfPredators;
		mMigrationCount = 0;
		mCurrentInertiaWeight = mInitialInertiaWeight;
	}
}

void LocalCaPso::reproductionOfPredators()
{
    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

#if defined(Q_WS_X11)
	std::uniform_int_distribution<int> randomOffset(-mPredatorReproductionRadius, mPredatorReproductionRadius);
#else
	std::uniform_int_distribution<int> randomOffset(-mPredatorReproductionRadius - 1, mPredatorReproductionRadius);
#endif

	list<shared_ptr<Particle>> newParticles;

	for_each(mPredatorSwarm.begin(), mPredatorSwarm.end(), [&, this](weak_ptr<Particle> wp)
	{
		if(auto p = wp.lock())
		{
			int pRow = p->position().row();
			int pCol = p->position().col();

			int birthCount = 0;

			while(birthCount < mPredatorMeanOffspring)
			{
				// Obtain an offset
				int finalRow = randomOffset(mRandom);
				int finalCol = randomOffset(mRandom);

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

	mNextStage = &LocalCaPso::predatorsDeath;
}

void LocalCaPso::predatorsDeath()
{
    int currentAddress;

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

	mNextStage = &LocalCaPso::predation;
}

void LocalCaPso::predation()
{
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

	mNextStage = &LocalCaPso::reproductionOfPreys;
}

void LocalCaPso::reproductionOfPreys()
{
    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

#if defined(Q_WS_X11)
	std::uniform_int_distribution<int> randomOffset(-mPreyReproductionRadius, mPreyReproductionRadius);
#else
	std::uniform_int_distribution<int> randomOffset(-mPreyReproductionRadius - 1, mPreyReproductionRadius);
#endif

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
                    finalRow = randomOffset(mRandom);
                    finalCol = randomOffset(mRandom);

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

    mNextStage = &LocalCaPso::competenceOfPreys;
}

void LocalCaPso::notifyNeighbors(const int& row, const int& col, const bool& death)
{
    int finalRow, finalCol;

    for(int nRow = row - mSearchRadius; nRow <= row + mSearchRadius; nRow++)
    {
        for(int nCol = col - mSearchRadius; nCol <= col + mSearchRadius; nCol++)
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