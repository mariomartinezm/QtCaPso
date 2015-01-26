#ifndef SWARM_H
#define SWARM_H

#include <list>
#include <random>
#include <memory>
#include <QtGlobal>
#include "particle.h"
#include "util.h"

class Swarm
{
public:
    Swarm(const std::vector<unsigned char>& lattice,
          const std::vector<unsigned char>& densities,
          int maxSpeed, float cognitiveFactor, float socialFactor,
          int socialRadius, int width, int height, int particleState);

    ~Swarm();

    float cognitiveFactor() const { return mCognitiveFactor; }
    float socialFactor() const { return mSocialFactor; }
    int maxSpeed() const { return mMaxSpeed; }
    float inertiaWeight() const { return mInertiaWeight; }

    void setCognitiveFactor(float factor) { mCognitiveFactor = factor; }
    void setSocialFactor(float factor) { mSocialFactor = factor; }
    void setMaxSpeed(int speed) { mMaxSpeed = speed; }

    std::list<std::shared_ptr<Particle>>::iterator begin();
    std::list<std::shared_ptr<Particle>>::iterator end();

    void initialize(int size, int width, int height, util::Random &random);
    void nextGen();

    void add(std::list<std::shared_ptr<Particle>>& newParticles);

#if defined(Q_OS_LINUX)
    std::list<std::shared_ptr<Particle>, std::allocator<std::shared_ptr<Particle>>>::iterator erase(std::list<std::shared_ptr<Particle>,
        std::allocator<std::shared_ptr<Particle>>>::iterator it);
#elif defined(__MINGW32__)
    std::list<std::shared_ptr<Particle>, std::allocator<std::shared_ptr<Particle>>>::iterator erase(std::list<std::shared_ptr<Particle>,
        std::allocator<std::shared_ptr<Particle>>>::iterator it);
#else
    std::list<std::shared_ptr<Particle>, std::allocator<std::shared_ptr<Particle>>>::iterator erase(std::list<std::shared_ptr<Particle>,
        std::allocator<std::shared_ptr<Particle>>>::const_iterator it);
#endif

    bool empty() const;

private:
    std::list<std::shared_ptr<Particle>> mParticles;

    const std::vector<unsigned char>& mLattice;
    const std::vector<unsigned char>& mDensities;
    int mMaxSpeed;
    float mCognitiveFactor;
    float mSocialFactor;
    float mInertiaWeight;
    int mSocialRadius;
    int mWidth;
    int mHeight;
    int mParticleState;
};

#endif // SWARM_H
