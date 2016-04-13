#include "PawnPos.h"

PawnPos::PawnPos()
{

}

PawnPos::PawnPos(const unsigned short X, const unsigned short Y)
{
    x = X;
    y = Y;
}

unsigned short PawnPos::X() const
{
    return x;
}

unsigned short PawnPos::Y() const
{
    return y;
}

void PawnPos::SetX(const unsigned short X)
{
    if (((y+1) % 2 == 1)&&((X+1) % 2 == 1))
    {
          Traces() << "\n" << "ERROR! PawnPos::SetX(const unsigned short X) Try to set pawn on white field!";
    } else
    if (((y+1) % 2 == 0)&&((X+1) % 2 == 0))
    {
         Traces() << "\n" << "ERROR! PawnPos::SetX(const unsigned short X) Try to set pawn on white field!";
    } else
    {
        x = X;
    };
}

void PawnPos::SetY(const unsigned short Y)
{
    if (((Y+1) % 2 == 1)&&((x+1) % 2 == 1))
    {
          Traces() << "\n" << "ERROR! PawnPos::SetY(const unsigned short Y) Try to set pawn on white field!";
    } else
    if (((Y+1) % 2 == 0)&&((x+1) % 2 == 0))
    {
         Traces() << "\n" << "ERROR! PawnPos::SetY(const unsigned short Y) Try to set pawn on white field!";
    } else
    {
        y = Y;
    };
}

bool PawnPos::operator != (const PawnPos & data)
{
    if (X()==data.X())
    {
        if (Y()==data.Y())
        {
            return false;
        };
    };

    return true;
}
