#pragma once

#include "latticepoint.h"


namespace capso
{
    struct Particle
    {
        capso::LatticePoint position;
        capso::LatticePoint bestPosition;
        capso::LatticePoint velocity;
        unsigned int timeSinceLastMeal;
    };
}
