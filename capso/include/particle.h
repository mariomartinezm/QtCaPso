#pragma once

#include "latticepoint.h"

struct Particle
{
    capso::LatticePoint position;
    capso::LatticePoint bestPosition;
    capso::LatticePoint velocity;
    unsigned int timeSinceLastMeal;
};
