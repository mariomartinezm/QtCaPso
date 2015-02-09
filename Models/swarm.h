#ifndef SWARM_H
#define SWARM_H

#include <list>
#include <random>
#include <memory>
#include <QtGlobal>
#include "particle.h"
#include "randomnumber.h"

class Swarm
{
public:
    Swarm(float cognitiveFactor, float socialFactor, float inertiaWeight,
          int maxSpeed, int socialRadius,
          std::vector<unsigned char>& lattice,
          std::vector<unsigned char>& densities,
          std::vector<unsigned char> &temp,
          int width, int height, int particleState, RandomNumber& random);

    ~Swarm();

    float cognitiveFactor() const { return mCognitiveFactor; }
    float socialFactor() const { return mSocialFactor; }
    float inertiaWeight() const { return mInertiaWeight; }
    int maxSpeed() const { return mMaxSpeed; }

    void setCognitiveFactor(float factor) { mCognitiveFactor = factor; }
    void setSocialFactor(float factor) { mSocialFactor = factor; }
    void setInertiaWeight(float inertiaWeight) { mInertiaWeight = inertiaWeight; }
    void setMaxSpeed(int speed) { mMaxSpeed = speed; }
    void setSocialRadius(int radius) { mSocialRadius = radius; }

    std::list<std::shared_ptr<Particle>>::iterator begin();
    std::list<std::shared_ptr<Particle>>::iterator end();

    void initialize(int size);
    void nextGen();

    void add(std::list<std::shared_ptr<Particle>>& newParticles);

#if defined(Q_OS_LINUX) || defined(__MINGW32__)
    std::list<std::shared_ptr<Particle>, std::allocator<std::shared_ptr<Particle>>>::iterator erase(std::list<std::shared_ptr<Particle>,
        std::allocator<std::shared_ptr<Particle>>>::iterator it);
#else
    std::list<std::shared_ptr<Particle>, std::allocator<std::shared_ptr<Particle>>>::iterator erase(std::list<std::shared_ptr<Particle>,
        std::allocator<std::shared_ptr<Particle>>>::const_iterator it);
#endif

    bool empty() const;

private:
    std::list<std::shared_ptr<Particle>> mParticles;

    float mCognitiveFactor;
    float mSocialFactor;
    float mInertiaWeight;
    int mMaxSpeed;
    int mSocialRadius;

    std::vector<unsigned char>& mLattice;
    std::vector<unsigned char>& mDensities;
    std::vector<unsigned char>& mTemp;

    int mWidth;
    int mHeight;
    int mParticleState;
    RandomNumber& mRandom;
};

#endif // SWARM_H
