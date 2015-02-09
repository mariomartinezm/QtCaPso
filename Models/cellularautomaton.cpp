#include <algorithm>
#include "cellularautomaton.h"

CellularAutomaton::CellularAutomaton(int width, int height)
    : mLattice(width * height)
{
    // Set the lattice's width, height and stride
    mWidth = width;
    mHeight = height;
    mStride = width;
}

unsigned char* CellularAutomaton::latticeData() const
{
    return const_cast<unsigned char*>(mLattice.data());
}

int CellularAutomaton::width() const
{
    return mWidth;
}

int CellularAutomaton::height() const
{
    return mHeight;
}

void CellularAutomaton::setCellState(int row, int col, unsigned char state)
{
    mLattice[getAddress(row, col)] = state;
}

void CellularAutomaton::clear()
{
	std::transform(mLattice.begin(), mLattice.end(), mLattice.begin(), [](unsigned char)
	{
		return 0;
	});
}

CellularAutomaton::~CellularAutomaton()
{

}
