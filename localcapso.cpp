#include <algorithm>
#include <ctime>
#include <QtGlobal>
#include "localcapso.h"

CaPso::CaPso(int width, int height)
    : CellularAutomata(width, height),
      // Containers
      mTemp(width*height),
      mPreyHistogram(width * height),
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
      // PSO parameters
      mInitialSwarmSize(3),
      mPFactor(2.0),
      mGFactor(2.0),
      mMigrationTime(5),
      mMigrationCount(0),
      mInitialInertiaWeight(0.9),
      mCurrentInertiaWeight(mInitialInertiaWeight),
      mFinalInertiaWeight(0.2),
      INERTIA_STEP((mInitialInertiaWeight - mFinalInertiaWeight)  / mMigrationTime),
      mMaxSpeed(10)
{
    initialize();
}

void CaPso::initialize()
{
    // Reset the prey histogram
	std::transform(mPreyHistogram.begin(), mPreyHistogram.end(), mPreyHistogram.begin(), [](unsigned char)
	{
		return 0;
	});

    // Reset the number of preys and predators
    mNumberOfPreys = 0;
    mNumberOfPredators = 0;

    // Randomly create preys
    for(int row = 0; row < mHeight; row++)
    {
        for(int col = 0; col < mWidth; col++)
        {
            if(mDistReal_0_1(mRandom) < mInitialAlivePercentage)
            {
                mLattice[getAddress(row, col)] = PREY;

                notifyNeighbors(row, col, false);

                mNumberOfPreys++;
            }
            else
            {
                mLattice[getAddress(row, col)] = EMPTY;
            }
        }
    }

    // Erase the particles from the lattice
	std::for_each(mParticles.begin(), mParticles.end(), [this](Particle& p)
	{
		LatticePoint position = p.position();
		mLattice[p.position().row(), p.position().col()] &= ~PREDATOR;
	});

    // Destroy all particles
    mParticles.clear();

    std::uniform_int_distribution<int> randomRow(0, mWidth - 1);
    std::uniform_int_distribution<int> randomCol(0, mHeight - 1);
    std::uniform_int_distribution<int> randomVel(0, mMaxSpeed);

    // Create and render all particles
    for(int i = 0; i < mInitialSwarmSize; i++)
    {
        Particle particle;
        particle.setPosition(randomRow(mRandom), randomCol(mRandom));
        particle.setBestPosition(particle.position().row(), particle.position().col());
        particle.setVelocity(randomVel(mRandom), randomVel(mRandom));
        mParticles.push_back(particle);

        mLattice[getAddress(particle.position().row(),
                            particle.position().col())] |= PREDATOR;
    }

    // Set the initial size for the population of predators
    mNumberOfPredators = mInitialSwarmSize;

    // Reset the migration counter
    mMigrationCount = 0;

    mNextStage = &CaPso::competenceOfPreys;
}

void CaPso::nextGen()
{
    (this->*mNextStage)();
}

void CaPso::setCompetenceFactor(double value)
{
    mCompetenceFactor = value;
}

void CaPso::setInitialAlivePreys(double value)
{
    mInitialAlivePercentage = value;
}

void CaPso::setPreyMeanOffspring(int value)
{
    mPreyMeanOffspring = value;
}

void CaPso::setPreyReproductionRadius(int value)
{
    mPreyReproductionRadius = value;
}

void CaPso::setPredatorMeanOffspring(int value)
{
    mPredatorMeanOffspring = value;
}

void CaPso::setPredatorReproductionRadius(int value)
{
    mPredatorReproductionRadius = value;
}
 
void CaPso::setSearchRadius(int value)
{
    mSearchRadius = value;

	NEIGHBORHOOD_SIZE = (2 * mSearchRadius + 1)*(2 * mSearchRadius + 1) - 1;
}

void CaPso::setInitialSwarmSize(int value)
{
    mInitialSwarmSize = value;
}

void CaPso::setPFactor(double value)
{
    mPFactor = value;
}

void CaPso::setGFactor(double value)
{
    mGFactor = value;
}

void CaPso::setMitrationTime(int value)
{
    mMigrationTime = value;
}

void CaPso::setInitialInertialWeight(double value)
{
    mInitialInertiaWeight = value;
}

void CaPso::setFinalInertiaWeight(double value)
{
    mFinalInertiaWeight = value;
}

int CaPso::numberOfPreys() const
{
    return mNumberOfPreys;
}

int CaPso::numberOfPredators() const
{
    return mNumberOfPredators;
}

void CaPso::competenceOfPreys()
{
    std::copy(mPreyHistogram.begin(), mPreyHistogram.end(), mTemp.begin());

    int currentAddress;
    double deathProbability;

    for(int row = 0; row < mHeight; row++)
    {
        for(int col = 0; col < mWidth; col++)
        {
            currentAddress = getAddress(row, col);

            if(mLattice[currentAddress] & PREY)
            {
                deathProbability = mTemp[currentAddress] *
                        mCompetenceFactor / NEIGHBORHOOD_SIZE;

                if(mDistReal_0_1(mRandom) <= deathProbability)
                {
                    // Only kill the prey
                    mLattice[currentAddress] &= ~PREY;

                    notifyNeighbors(row, col, true);

                    mNumberOfPreys--;
                }
            }
        }
    }

    mNextStage = &CaPso::migration;
}

void CaPso::migration()
{
    int finalRow, finalCol, neighbourAddress;
    int pRow, pCol, bestRow, bestCol, bestAddress;

    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

	std::for_each(mParticles.begin(), mParticles.end(), [&, this](Particle& p)
	{
		pRow = bestRow = p.position().row();
		pCol = bestCol = p.position().col();

		bestAddress = getAddress(pRow, pCol);

		// Clear the previous position of the predator
		mLattice[getAddress(pRow, pCol)] &= ~PREDATOR;

		// Get the best position among the neighbors of the predator
		for(int nRow = pRow - mSearchRadius; nRow <= pRow + mSearchRadius; nRow++)
		{
			for(int nCol = pCol - mSearchRadius; nCol <= pCol + mSearchRadius; nCol++)
			{
				if(nRow == pRow && nCol == pCol)
				{
					continue;
				}

				finalRow = (mHeight + nRow) % mHeight;
				finalCol = (mWidth + nCol) % mWidth;

				neighbourAddress = getAddress(finalRow, finalCol);

				if(mTemp[neighbourAddress] & PREDATOR)
				{
					// Update if necessary
					if(mPreyHistogram[bestAddress] < mPreyHistogram[neighbourAddress])
					{
						bestRow = nRow;
						bestCol = nCol;

						bestAddress = neighbourAddress;
					}
				}
			}
		}

		double r1 = mDistReal_0_1(mRandom);
		double r2 = mDistReal_0_1(mRandom);

		// Get the new velocity
		int velRow = (int)(mCurrentInertiaWeight * p.velocity().row() +
			mPFactor * r1 * (p.bestPosition().row() - pRow) +
			mGFactor * r2 * (bestRow - pRow));
		int velCol = (int)(mCurrentInertiaWeight * p.velocity().col() +
			mPFactor * r1 * (p.bestPosition().col() - pCol) +
			mGFactor * r2 * (bestCol - pCol));

		// Adjust speed
		double speed = sqrt((double)(velRow * velRow + velCol * velCol));

		while(speed > mMaxSpeed)
		{
			velRow *= 0.9;
			velCol *= 0.9;

			speed = sqrt((double)(velRow * velRow + velCol * velCol));
		}

		p.setVelocity(velRow, velCol);

		// Move the particle
		velRow += pRow;
		velCol += pCol;

		// Adjust position
		velRow = (mHeight + velRow) % mHeight;
		velCol = (mWidth + velCol) % mWidth;

		p.setPosition(velRow, velCol);

		// If necessary, update the particle's best know position
		if(mPreyHistogram[getAddress(pRow, pCol)] < mPreyHistogram[getAddress(velRow, velCol)])
		{
			p.setBestPosition(velRow, velCol);
		}
	});

	// Render the particles at their new positions
	std::for_each(mParticles.begin(), mParticles.end(), [this](Particle& p)
	{
		mLattice[getAddress(p.position().row(), p.position().col())] |= PREDATOR;
	});

    // Decrease the inertia weight and increase the migration counter
    mCurrentInertiaWeight -= INERTIA_STEP;
    mMigrationCount++;

    // If migration has ended, point to the next stage and reset the inertia
    // weight and migration count
    if(mMigrationCount == mMigrationTime)
    {
		mNextStage = &CaPso::reproductionOfPredators;
        mMigrationCount = 0;
        mCurrentInertiaWeight = mInitialInertiaWeight;
    }
}

void CaPso::reproductionOfPredators()
{
    std::copy(mLattice.begin(), mLattice.end(), mTemp.begin());

#if defined(Q_WS_X11)
	std::uniform_int_distribution<int> randomOffset(-mPredatorReproductionRadius, mPredatorReproductionRadius);
#else
	std::uniform_int_distribution<int> randomOffset(-mPredatorReproductionRadius - 1, mPredatorReproductionRadius);
#endif

    std::vector<Particle> newParticles;
    int pRow, pCol, finalRow, finalCol, birthCount;

	std::for_each(mParticles.begin(), mParticles.end(), [&, this](Particle& p)
	{
		pRow = p.position().row();
		pCol = p.position().col();

		birthCount = 0;

		while(birthCount < mPredatorMeanOffspring)
		{
			// Obtain an offset
			finalRow = randomOffset(mRandom);
			finalCol = randomOffset(mRandom);

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

			if(!(mLattice[getAddress(finalRow, finalCol)] & PREDATOR))
			{
				// Create a new particle
				Particle particle;
				particle.setPosition(finalRow, finalCol);
				particle.setBestPosition(p.bestPosition().row(), p.bestPosition().col());

				mLattice[getAddress(finalRow, finalCol)] |= PREDATOR;

				newParticles.push_back(particle);

				mNumberOfPredators++;
			}

			birthCount++;
		}
	});

    mParticles.insert(mParticles.end(), newParticles.begin(), newParticles.end());

	mNextStage = &CaPso::predatorsDeath;
}

void CaPso::predatorsDeath()
{
    int currentAddress;

    for(auto it = mParticles.begin(); it != mParticles.end();)
    {
        currentAddress = getAddress((*it).position().row(), (*it).position().col());

        if(!(mLattice[currentAddress] & PREY))
        {
            mLattice[currentAddress] &= ~PREDATOR;

			// Since the iterator is invalidated when erase() is called, we must
			// use its return value (an iterator pointing to the element following
			// the element that has just been erased) to keep a valid iterator.
            it = mParticles.erase(it);

            mNumberOfPredators--;
        }
        else
        {
            it++;
        }
    }

	mNextStage = &CaPso::predatorsFeeding;
}

void CaPso::predatorsFeeding()
{
    int pRow, pCol, currentAddress;

	std::for_each(mParticles.begin(), mParticles.end(), [&, this](Particle& p)
	{
		pRow = p.position().row();
		pCol = p.position().col();
		currentAddress = getAddress(pRow, pCol);

		// Kill the prey in the current cell
		if(mLattice[currentAddress] & PREY)
		{
			mLattice[currentAddress] &= ~PREY;

			notifyNeighbors(pRow, pCol, true);

			mNumberOfPreys--;
		}
	});

	mNextStage = &CaPso::reproductionOfPreys;
}

void CaPso::reproductionOfPreys()
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

    mNextStage = &CaPso::competenceOfPreys;
}

void CaPso::notifyNeighbors(const int& row, const int& col, const bool& death)
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
                mPreyHistogram[getAddress(finalRow, finalCol)]--;
            }
            else
            {
                mPreyHistogram[getAddress(finalRow, finalCol)]++;
            }
        }
    }
}