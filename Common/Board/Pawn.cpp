#include "Pawn.h"

Pawn::Pawn()
{
    Clear();
}

Pawn::Pawn(unsigned short xp, unsigned short yp, unsigned short deadp, unsigned short ponsp)
{
    x = xp;
    y = yp;
    dead = deadp;
    pons = ponsp;
}

void Pawn::Clear()
{
    dead = 1;
    pons = 0;
}

