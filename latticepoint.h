#ifndef LATTICEPOINT_H
#define LATTICEPOINT_H

class LatticePoint
{
public:
    LatticePoint();

    LatticePoint(int row, int col);

    int row() const;
    void setRow(int row);

    int col() const;
    void setCol(int col);

private:
    int mRow;
    int mCol;
};

#endif // LATTICEPOINT_H
