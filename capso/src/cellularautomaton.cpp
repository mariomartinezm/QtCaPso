#include <algorithm>
#include "cellularautomaton.h"

capso::CellularAutomaton::CellularAutomaton(int width, int height)
    : mLattice(width * height)
{
    // Set the lattice's width, height and stride
    mWidth = width;
    mHeight = height;
    mStride = width;
}

unsigned char* capso::CellularAutomaton::latticeData() const
{
    return const_cast<unsigned char*>(mLattice.data());
}

int capso::CellularAutomaton::width() const
{
    return mWidth;
}

int capso::CellularAutomaton::height() const
{
    return mHeight;
}

void capso::CellularAutomaton::setCellState(int row, int col, unsigned char state)
{
    mLattice[getAddress(row, col)] = state;
}

void capso::CellularAutomaton::clear()
{
    std::transform(mLattice.begin(), mLattice.end(), mLattice.begin(), [](unsigned char)
    {
        return 0;
    });
}

capso::CellularAutomaton::~CellularAutomaton()
{

}
