#include <algorithm>
#include "swarm.h"

using std::make_shared;
using std::shared_ptr;

Swarm::Swarm(float cognitiveFactor, float socialFactor, int maxSpeed)
	: mCognitiveFactor(cognitiveFactor),
	mSocialFactor(socialFactor),
	mMaxSpeed(maxSpeed)
{
}

Swarm::~Swarm()
{
}

std::list<shared_ptr<Particle>>::iterator Swarm::begin()
{
	return mParticles.begin();
}

std::list<shared_ptr<Particle>>::iterator Swarm::end()
{
	return mParticles.end();
}

void Swarm::initialize(int size, int width, int height, std::mt19937& random)
{
	mParticles.clear();

	std::uniform_int_distribution<int> randomRow(0, height - 1);
	std::uniform_int_distribution<int> randomCol(0, width - 1);

	for(int i = 0; i < size; i++)
	{
		auto particle = make_shared<Particle>();
		particle->setPosition(randomRow(random), randomCol(random));
		particle->setBestPosition(randomRow(random), randomCol(random));
		mParticles.push_back(particle);
	}
}

void Swarm::add(std::list<shared_ptr<Particle>>& newParticles)
{
	mParticles.insert(mParticles.end(), newParticles.begin(), newParticles.end());
}

std::list<shared_ptr<Particle>, std::allocator<shared_ptr<Particle>>>::iterator Swarm::erase(std::list<shared_ptr<Particle>,
		std::allocator<shared_ptr<Particle>>>::const_iterator it)
{
	return mParticles.erase(it);
}

bool Swarm::empty() const
{
	return mParticles.empty();
}