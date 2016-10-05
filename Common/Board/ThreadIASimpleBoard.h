#ifndef THREADIASIMPLEBOARD_H
#define THREADIASIMPLEBOARD_H

#include "Pawn.h"

class ThreadIASimpleBoard
{
    public:
        ThreadIASimpleBoard();
        friend class Board;

    private:
        Pawn white[12];
        Pawn black[12];
};

#endif // THREADIASIMPLEBOARD_H
