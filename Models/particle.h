#ifndef PARTICLE_H
#define PARTICLE_H

#include "latticepoint.h"

class Particle
{
public:
    Particle();

	const LatticePoint& position() const;
    void setPosition(int row, int col);

    const LatticePoint& bestPosition() const;
    void setBestPosition(int row, int col);

    const LatticePoint& velocity() const;
    void setVelocity(int row, int col);

	const unsigned int timeSinceLastMeal() const;
	void increaseTimeSinceLastMeal();
	void resetTimeSinceLastMeal();

private:
    LatticePoint mPosition;
    LatticePoint mBestPosition;
    LatticePoint mVelocity;
	unsigned int mTimeSinceLastMeal;
};

#endif // PARTICLE_H
