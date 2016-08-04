#ifndef PARTICLE_H
#define PARTICLE_H

#include "latticepoint.h"

struct Particle
{
    LatticePoint position;
    LatticePoint bestPosition;
    LatticePoint velocity;
    unsigned int timeSinceLastMeal;
};

#endif // PARTICLE_H
