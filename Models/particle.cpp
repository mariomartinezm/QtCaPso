#include "particle.h"

Particle::Particle() : mTimeSinceLastMeal(0)
{
	setPosition(0, 0);
	setBestPosition(0, 0);
	setVelocity(0, 0);
}

const LatticePoint& Particle::position() const
{
    return mPosition;
}

void Particle::setPosition(int row, int col)
{
    mPosition.setRow(row);
    mPosition.setCol(col);
}

const LatticePoint& Particle::bestPosition() const
{
    return mBestPosition;
}

void Particle::setBestPosition(int row, int col)
{
    mBestPosition.setRow(row);
    mBestPosition.setCol(col);
}

const LatticePoint& Particle::velocity() const
{
    return mVelocity;
}

void Particle::setVelocity(int row, int col)
{
    mVelocity.setRow(row);
    mVelocity.setCol(col);
}

unsigned int Particle::timeSinceLastMeal() const
{
	return mTimeSinceLastMeal;
}

void Particle::increaseTimeSinceLastMeal()
{
	mTimeSinceLastMeal++;
}

void Particle::resetTimeSinceLastMeal()
{
	mTimeSinceLastMeal = 0;
}
