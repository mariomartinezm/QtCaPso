#include <algorithm>
#include <memory>
#include "swarm.h"

using std::for_each;
using std::list;
using std::make_shared;
using std::shared_ptr;
using std::weak_ptr;

Swarm::Swarm(const std::vector<unsigned char> &lattice,
             const std::vector<unsigned char> &densities,
             int maxSpeed, float cognitiveFactor, float socialFactor,
             int socialRadius, int width, int height, int particleState)
    : mLattice(lattice),
      mDensities(densities),
      mMaxSpeed(maxSpeed),
      mCognitiveFactor(cognitiveFactor),
      mSocialFactor(socialFactor),
      mSocialRadius(socialRadius),
      mWidth(width),
      mHeight(height),
      mParticleState(particleState)
{
}

Swarm::~Swarm()
{
}

list<shared_ptr<Particle>>::iterator Swarm::begin()
{
    return mParticles.begin();
}

list<shared_ptr<Particle>>::iterator Swarm::end()
{
    return mParticles.end();
}

void Swarm::initialize(int size, int width, int height, util::Random &random)
{
    mParticles.clear();

    for(int i = 0; i < size; i++)
    {
        auto particle = make_shared<Particle>();
        particle->setPosition(random.GetRandomInt(0, height - 1), random.GetRandomInt(0, width - 1));
        particle->setBestPosition(random.GetRandomInt(0, height - 1), random.GetRandomInt(0, width - 1));
        mParticles.push_back(particle);
    }
}

void Swarm::nextGen()
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

    for_each(mParticles.begin(), mParticles.end(),
             [&, this](weak_ptr<Particle> wp)
    {
        if(auto p = wp.lock())
        {
            int pRow = p->position().row();
            int pCol = p->position().col();

            int bestRow = pRow;
            int bestCol = pCol;

            int bestAddress = mWidth * bestRow + bestCol;

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
                    if(mLattice[neighbourAddress] & mParticleState)
                    {
                        // Yes, then compare its fitness with the fitness of our
                        // current position. Is it better?
                        if(mDensities[bestAddress] < mDensities[neighbourAddress])
                        {
                            // Yes, update the best known position
                            bestRow = absRow;
                            bestCol = absCol;
                        }
                    }
                }
            }
        }
    });
}

void Swarm::add(list<shared_ptr<Particle>>& newParticles)
{
    mParticles.insert(mParticles.end(), newParticles.begin(), newParticles.end());
}

#if defined(Q_OS_LINUX) || defined (__MINGW32__)
list<shared_ptr<Particle>, std::allocator<shared_ptr<Particle>>>::iterator Swarm::erase(list<shared_ptr<Particle>,
        std::allocator<shared_ptr<Particle>>>::iterator it)
#else
list<shared_ptr<Particle>, std::allocator<shared_ptr<Particle>>>::iterator Swarm::erase(list<shared_ptr<Particle>,
        std::allocator<shared_ptr<Particle>>>::const_iterator it)
#endif
{
    return mParticles.erase(it);
}

bool Swarm::empty() const
{
    return mParticles.empty();
}
