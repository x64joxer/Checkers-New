#ifndef PAWNPOS_H
#define PAWNPOS_H

#include "../Traces/Traces.h"

class PawnPos
{
    public:
        PawnPos();
        PawnPos(const unsigned short X, const unsigned short Y);
        unsigned short X() const;
        unsigned short Y() const;
        void SetX(const unsigned short X);
        void SetY(const unsigned short Y);

        bool operator != (const PawnPos &);

    private:
        unsigned short x : 4;
        unsigned short y : 4;
};

#endif // PAWNPOS_H
