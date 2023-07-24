#include <algorithm>
#include <memory>
#include "swarm.h"

using std::for_each;
using std::list;
using std::make_shared;
using std::shared_ptr;
using std::weak_ptr;

capso::Swarm::Swarm(float cognitiveFactor, float socialFactor, float inertiaWeight,
             int maxSpeed, int socialRadius,
             std::vector<unsigned char> &lattice,
             std::vector<unsigned char> &densities,
             std::vector<unsigned char> &temp,
             int width, int height, int particleState, RandomNumber &random)
    : mCognitiveFactor(cognitiveFactor),
      mSocialFactor(socialFactor),
      mInertiaWeight(inertiaWeight),
      mMaxSpeed(maxSpeed),
      mSocialRadius(socialRadius),
      mLattice(lattice),
      mDensities(densities),
      mTemp(temp),
      mWidth(width),
      mHeight(height),
      mParticleState(particleState),
      mRandom(random)
{

}


capso::Swarm::~Swarm()
{
}

list<shared_ptr<Particle>>::iterator capso::Swarm::begin()
{
    return mParticles.begin();
}

list<shared_ptr<Particle>>::iterator capso::Swarm::end()
{
    return mParticles.end();
}

void capso::Swarm::initialize(unsigned int size)
{
    mParticles.clear();

    for(unsigned int i = 0; i < size; i++)
    {
        auto particle = make_shared<Particle>();
        particle->position.row = mRandom.GetRandomInt(0, mHeight - 1);
        particle->position.col = mRandom.GetRandomInt(0, mWidth - 1);
        particle->bestPosition.row = mRandom.GetRandomInt(0, mHeight - 1);
        particle->bestPosition.col = mRandom.GetRandomInt(0, mWidth - 1);
        particle->timeSinceLastMeal = 0;

        mParticles.push_back(particle);
    }
}

//void Swarm::initialize(unsigned int size)
//{
//    mParticles.clear();

//    auto particle = make_shared<Particle>();
//    particle->position.row = mHeight / 2;
//    particle->position.col = mWidth  / 2;
//    particle->bestPosition.row = mRandom.GetRandomInt(0, mHeight - 1);
//    particle->bestPosition.col = mRandom.GetRandomInt(0, mWidth  - 1);
//    particle->timeSinceLastMeal = 0;

//    mParticles.push_back(particle);

//    const unsigned int radius = 7;

//    for(unsigned int i = 0; i < size - 1; i++)
//    {
//        // Get an offset
//        unsigned int row = mRandom.GetRandomInt(-radius, radius);
//        unsigned int col = mRandom.GetRandomInt(-radius, radius);

//        auto neighbor = make_shared<Particle>();
//        neighbor->position.row = particle->position.row + row;
//        neighbor->position.col = particle->position.row + col;
//        neighbor->bestPosition.row = particle->bestPosition.row;
//        neighbor->bestPosition.col = particle->bestPosition.col;
//        neighbor->timeSinceLastMeal = 0;

//        mParticles.push_back(neighbor);
//    }
//}

void capso::Swarm::nextGen()
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

    for_each(mParticles.begin(), mParticles.end(),
             [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position.row;
            int pCol = p->position.col;

            int bestRow = pRow;
            int bestCol = pCol;

            int bestAddress = mWidth * bestRow + bestCol;

            // Clear the previous position of the predator
            mLattice[bestAddress] &= ~mParticleState;

            // Get the best position among the neighbors of the particle
            for(int nRow = pRow - mSocialRadius; nRow <= pRow + mSocialRadius; nRow++)
            {
                for(int nCol = pCol - mSocialRadius; nCol <= pCol + mSocialRadius; nCol++)
                {
                    // Ignore the particle at the center of the neighborhood
                    if(nRow == pRow && nCol == pCol)
                    {
                        continue;
                    }

                    // Obtain the absolute position of the neighbour
                    int absRow = (mHeight + nRow) % mHeight;
                    int absCol = (mWidth + nCol) % mWidth;

                    int neighbourAddress = mWidth * absRow + absCol;

                    // Is the neighbor a particle?
                    if(mTemp[neighbourAddress] & mParticleState)
                    {
                        // Yes, then compare its fitness with the fitness of our
                        // current position. Is it better?
                        if(mDensities[bestAddress] < mDensities[neighbourAddress])
                        {
                            // Yes, update the best known position
                            bestRow = absRow;
                            bestCol = absCol;

                            bestAddress = neighbourAddress;
                        }
                    }
                }
            }

            float r1 = mRandom.GetRandomFloat();
            float r2 = mRandom.GetRandomFloat();

            int currentVelRow = p->velocity.row;
            int currentVelCol = p->velocity.col;

            validateVector(currentVelRow, currentVelCol);

            int cognitiveVelRow = p->bestPosition.row - pRow;
            int cognitiveVelCol = p->bestPosition.col - pCol;

            validateVector(cognitiveVelRow, cognitiveVelCol);

            int socialVelRow = bestRow - pRow;
            int socialVelCol = bestCol - pCol;

            validateVector(socialVelRow, socialVelCol);

            // Get the new velocity
            int velRow = (int)(mInertiaWeight * currentVelRow +
                mCognitiveFactor * r1 * cognitiveVelRow +
                mSocialFactor * r2 * socialVelRow);

            int velCol = (int)(mInertiaWeight * currentVelCol +
                mCognitiveFactor * r1 * cognitiveVelCol +
                mSocialFactor * r2 * socialVelCol);

            // Adjust speed
            float speed = sqrt((float)(velRow * velRow + velCol * velCol));

            while(speed > mMaxSpeed)
            {
                velRow *= (int)(0.9);
                velCol *= (int)(0.9);

                speed = sqrt((float)(velRow * velRow + velCol * velCol));
            }

            // Move the particle
            int posRow = pRow + velRow;
            int posCol = pCol + velCol;

            // Adjust position
            posRow = (mHeight + posRow) % mHeight;
            posCol = (mWidth + posCol) % mWidth;

            // Is the destination already occupied?
            if(!(mLattice[mWidth * posRow + posCol] & mParticleState))
            {
                // No, then update the particle's position
                p->position.row = posRow;
                p->position.col = posCol;
                p->velocity.row = velRow;
                p->velocity.col = velCol;

                // Render the particle at its new position
                mLattice[mWidth * posRow + posCol] |= mParticleState;

                // If necessary, update the particle's best known position
                if(mDensities[mWidth * p->bestPosition.row + p->bestPosition.col] <
                    mDensities[mWidth * posRow + posCol])
                {
                    p->bestPosition.row = posRow;
                    p->bestPosition.col = posCol;
                }
            }
            else
            {
                // Restore the particle's previous position
                mLattice[mWidth * pRow + pCol] |= mParticleState;
            }
        }
    });
}

void capso::Swarm::add(list<shared_ptr<Particle>>& newParticles)
{
    mParticles.insert(mParticles.end(), newParticles.begin(), newParticles.end());
}

list<shared_ptr<Particle>, std::allocator<shared_ptr<Particle>>>::iterator capso::Swarm::erase(list<shared_ptr<Particle>,
        std::allocator<shared_ptr<Particle>>>::iterator it)
{
    return mParticles.erase(it);
}

bool capso::Swarm::empty() const
{
    return mParticles.empty();
}
