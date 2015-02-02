#include "latticepoint.h"

LatticePoint::LatticePoint()
{
    mRow = 0;
    mCol = 0;
}

LatticePoint::LatticePoint(int row, int col)
{
    mRow = row;
    mCol = col;
}

int LatticePoint::row() const
{
    return mRow;
}

void LatticePoint::setRow(int row)
{
    mRow = row;
}

int LatticePoint::col() const
{
    return mCol;
}

void LatticePoint::setCol(int col)
{
    mCol = col;
}
