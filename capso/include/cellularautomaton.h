#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H

#include <vector>

namespace capso
{
    class CellularAutomaton
    {
    public:
        CellularAutomaton(int width, int height);

        unsigned char* latticeData() const;

        int width() const;

        int height() const;

        void setCellState(int row, int col, unsigned char state);

        virtual void clear();

        virtual void initialize() = 0;

        virtual void nextGen() = 0;

        virtual ~CellularAutomaton() = 0;

    protected:
        std::vector<unsigned char> mLattice;
        int mWidth, mHeight, mStride;

        int getAddress(int row, int col);
    };

    inline int CellularAutomaton::getAddress(int row, int col)
    {
        return mStride * row + col;
    }
}

#endif // CELLULARAUTOMATON_H
