#include "PossibleMoves.h"

PossibleMoves::PossibleMoves()
{

}

bool PossibleMoves::CanIGrab(const unsigned short number, const Board & board)
{
    PawnPos pos = board.GetBlackPawnPos(number);
    bool flag = CheckHitTopLeft(pos.X(), pos.Y(), board) | CheckHitTopRight(pos.X(), pos.Y(), board);
    bool flagPons = CheckHitBottomLeft(pos.X(), pos.Y(), board) | CheckHitBottomRight(pos.X(), pos.Y(), board);

    Traces() << "\n" << "CheckHitBottomLeft(pos.X(), pos.Y(), board) " << CheckHitBottomLeft(pos.X(), pos.Y(), board);
    Traces() << "\n" << "CheckHitBottomRight(pos.X(), pos.Y(), board" << CheckHitBottomRight(pos.X(), pos.Y(), board);
    Traces() << "\n" << "CheckHitTopLeft(pos.X(), pos.Y(), board) " << CheckHitTopLeft(pos.X(), pos.Y(), board);
    Traces() << "\n" << "CheckHitTopRight(pos.X(), pos.Y(), board" << CheckHitTopRight(pos.X(), pos.Y(), board);

    bool otherFlag = false;
    for (unsigned short i=0; i < board.GetNumberOfBlack();i++)
    {
        if (i!=number)
        {
            PawnPos pos = board.GetBlackPawnPos(i);
            otherFlag = otherFlag | CheckHitTopLeft(pos.X(), pos.Y(), board) | CheckHitTopRight(pos.X(), pos.Y(), board);

            if (board.GetBlackPawnPons(i))
            {
                otherFlag = otherFlag | CheckHitBottomLeft(pos.X(), pos.Y(), board) | CheckHitBottomRight(pos.X(), pos.Y(), board);
            };
        };
    };

    Traces() << "\n" << "otherFlag " << otherFlag;

    if (!board.GetBlackPawnPons(number))
    {
        if (!flag)
        {
            if (otherFlag)
            {
                return false;
            };
        };

        if (flag) return 1;
        return CheckPutTopLeft(pos.X(), pos.Y(), board) | CheckPutTopRight(pos.X(), pos.Y(), board);
    } else
    {
        if (!(flag | flagPons))
        {
            if (otherFlag)
            {
                return false;
            };
        };

        if (flag | flagPons) return 1;
        return CheckPutTopLeft(pos.X(), pos.Y(), board) | CheckPutTopRight(pos.X(), pos.Y(), board) | CheckPutBottomLeft(pos.X(), pos.Y(), board) | CheckPutBottomRight(pos.X(), pos.Y(), board);
    };
}


bool PossibleMoves::CheckHitTopLeft(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x-1, y - 1)) return 0;
    if (!board.IsWhitePawnOnPos(x-1,y-1))
    {
        return 0;
    } else
    {
        if (OutOfBoard(x-2, y - 2)) return 0;
        if (board.IsPawnOnPos(x-2,y-2)) return 0;
        return 1;
    };
}

bool PossibleMoves::CheckHitTopRight(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x+1, y -1)) return 0;
    if (!board.IsWhitePawnOnPos(x+1,y-1))
    {
        return 0;
    } else
    {
        if (OutOfBoard(x +2, y -2)) return 0;
        if (board.IsPawnOnPos(x+2,y-2)) return 0;
        return 1;
    };
}

bool PossibleMoves::CheckHitBottomLeft(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x-1, y + 1)) return 0;
    if (!board.IsWhitePawnOnPos(x-1,y+1))
    {
        return 0;
    } else
    {
        if (OutOfBoard(x-2, y + 2)) return 0;
        if (board.IsPawnOnPos(x-2,y+2)) return 0;
        return 1;
    };
}

bool PossibleMoves::CheckHitBottomRight(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x+1, y +1)) return 0;
    if (!board.IsWhitePawnOnPos(x+1,y+1))
    {
        return 0;
    } else
    {
        if (OutOfBoard(x +2, y +2)) return 0;
        if (board.IsPawnOnPos(x+2,y+2)) return 0;
        return 1;
    };
}

bool PossibleMoves::CheckPutTopLeft(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x-1, y -1)) return 0;
    if (board.IsPawnOnPos(x-1, y -1)) return 0;
    return true;
}

bool PossibleMoves::CheckPutTopRight(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x+1, y -1)) return 0;
    if (board.IsPawnOnPos(x+1, y -1)) return 0;
    return true;
}

bool PossibleMoves::CheckPutBottomLeft(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x-1, y +1)) return 0;
    if (board.IsPawnOnPos(x-1, y +1)) return 0;
    return true;
}

bool PossibleMoves::CheckPutBottomRight(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoard(x+1, y +1)) return 0;
    if (board.IsPawnOnPos(x+1, y +1)) return 0;
    return true;
}

bool PossibleMoves::CanIPutHere(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    bool ponsFlag = board.GetBlackPawnPons(number);

    bool killFlag = CheckHitTopLeft(pos.X(), pos.Y(), board) | CheckHitTopRight(pos.X(), pos.Y(), board);
    if (ponsFlag)
    {
        killFlag = killFlag | CheckHitBottomLeft(pos.X(), pos.Y(), board) | CheckHitBottomRight(pos.X(), pos.Y(), board);
    };


    if ((x == pos.X()-2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopLeft(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()+2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopRight(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()-1)&&(y == pos.Y()-1))
    {
        if (CheckPutTopLeft(pos.X(), pos.Y(), board))
        {
            if (!killFlag) return 1;
        };
    };

    if ((x == pos.X()+1)&&(y == pos.Y()-1))
    {
        if (CheckPutTopRight(pos.X(), pos.Y(), board))
        {
            if (!killFlag) return 1;
        };
    };

    if (ponsFlag)
    {
        if ((x == pos.X()-2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomLeft(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()+2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomRight(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()-1)&&(y == pos.Y()+1))
        {
            if (CheckPutBottomLeft(pos.X(), pos.Y(), board))
            {
                if (!killFlag) return 1;
            };
        };

        if ((x == pos.X()+1)&&(y == pos.Y()+1))
        {
            if (CheckPutBottomRight(pos.X(), pos.Y(), board))
            {
               if (!killFlag) return 1;
            };
        };
    };

    return false;
}

bool PossibleMoves::IsKill(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board, unsigned short &killed) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    bool ponsFlag = board.GetBlackPawnPons(number);

    if ((x == pos.X()-2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopLeft(pos.X(), pos.Y(), board))
        {
            killed = board.GetWhitePawnNumber(pos.X()-1, pos.Y() -1);
            return 1;
        };
    };

    if ((x == pos.X()+2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopRight(pos.X(), pos.Y(), board))
        {
            killed = board.GetWhitePawnNumber(pos.X()+1, pos.Y() -1);
            return 1;
        };
    };

    if (ponsFlag)
    {
        if ((x == pos.X()-2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomLeft(pos.X(), pos.Y(), board))
            {
                killed = board.GetWhitePawnNumber(pos.X()-1, pos.Y() +1);
                return 1;
            };
        };

        if ((x == pos.X()+2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomRight(pos.X(), pos.Y(), board))
            {
                killed = board.GetWhitePawnNumber(pos.X()+1, pos.Y() +1);
                return 1;
            };
        };
    };

    return 0;
}

bool PossibleMoves::OutOfBoard(const unsigned short x, const unsigned short y) const
{
    if (x>7) return 1;
    if (y>7) return 1;

    return 0;
}


