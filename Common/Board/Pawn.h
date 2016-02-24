#ifndef PAWN_H
#define PAWN_H

#include "../Common/Traces/Traces.h"

class Pawn
{
    public:
        Pawn();
        Pawn(unsigned short xp, unsigned short yp, unsigned short deadp, unsigned short ponsp);
        void Clear();

        unsigned short x : 4;
        unsigned short y : 4;
        unsigned short dead : 1;
        unsigned short pons : 1;
};



#endif // PAWN_H
