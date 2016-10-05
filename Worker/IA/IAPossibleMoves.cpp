#include "IAPossibleMoves.h"

IAPossibleMoves::IAPossibleMoves()
{

}

bool IAPossibleMoves::CanIGrabBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    bool flag = CheckHitTopLeftBlack(pos.X(), pos.Y(), board) | CheckHitTopRightBlack(pos.X(), pos.Y(), board);
    bool flagPons = CheckHitBottomLeftBlack(pos.X(), pos.Y(), board) | CheckHitBottomRightBlack(pos.X(), pos.Y(), board);

    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitBottomLeftBlack(pos.X(), pos.Y(), board) " << CheckHitBottomLeftBlack(pos.X(), pos.Y(), board);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitBottomRightBlack(pos.X(), pos.Y(), board" << CheckHitBottomRightBlack(pos.X(), pos.Y(), board);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitTopLeftBlack(pos.X(), pos.Y(), board) " << CheckHitTopLeftBlack(pos.X(), pos.Y(), board);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitTopRightBlack(pos.X(), pos.Y(), board" << CheckHitTopRightBlack(pos.X(), pos.Y(), board);

    bool otherFlag = false;
    for (unsigned short i=0; i < board.GetNumberOfBlack();i++)
    {
        if (i!=number)
        {
            PawnPos pos = board.GetBlackPawnPos(i);
            otherFlag = otherFlag | CheckHitTopLeftBlack(pos.X(), pos.Y(), board) | CheckHitTopRightBlack(pos.X(), pos.Y(), board);

            if (board.GetWhitePawnPons(i))
            {
                otherFlag = otherFlag | CheckHitBottomLeftBlack(pos.X(), pos.Y(), board) | CheckHitBottomRightBlack(pos.X(), pos.Y(), board);
            };
        };
    };

    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "otherFlag " << otherFlag;

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
        return CheckPutTopLeftBlack(pos.X(), pos.Y(), board) | CheckPutTopRightBlack(pos.X(), pos.Y(), board);
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
        return CheckPutTopLeftBlack(pos.X(), pos.Y(), board) | CheckPutTopRightBlack(pos.X(), pos.Y(), board) | CheckPutBottomLeftBlack(pos.X(), pos.Y(), board) | CheckPutBottomRightBlack(pos.X(), pos.Y(), board);
    };
}


bool IAPossibleMoves::CheckHitTopLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x-1, y - 1)) return 0;
    if (!board.IsWhitePawnOnPos(x-1,y-1))
    {
        return 0;
    } else
    {
        if (OutOfBoardBlack(x-2, y - 2)) return 0;
        if (board.IsPawnOnPos(x-2,y-2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckHitTopRightBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x+1, y -1)) return 0;
    if (!board.IsWhitePawnOnPos(x+1,y-1))
    {
        return 0;
    } else
    {
        if (OutOfBoardBlack(x +2, y -2)) return 0;
        if (board.IsPawnOnPos(x+2,y-2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckHitBottomLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x-1, y + 1)) return 0;
    if (!board.IsWhitePawnOnPos(x-1,y+1))
    {
        return 0;
    } else
    {
        if (OutOfBoardBlack(x-2, y + 2)) return 0;
        if (board.IsPawnOnPos(x-2,y+2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckHitBottomRightBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x+1, y +1)) return 0;
    if (!board.IsWhitePawnOnPos(x+1,y+1))
    {
        return 0;
    } else
    {
        if (OutOfBoardBlack(x +2, y +2)) return 0;
        if (board.IsPawnOnPos(x+2,y+2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckPutTopLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x-1, y -1)) return 0;
    if (board.IsPawnOnPos(x-1, y -1)) return 0;
    return true;
}

bool IAPossibleMoves::CheckPutTopRightBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x+1, y -1)) return 0;
    if (board.IsPawnOnPos(x+1, y -1)) return 0;
    return true;
}

bool IAPossibleMoves::CheckPutBottomLeftBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x-1, y +1)) return 0;    
    if (board.IsPawnOnPos(x-1, y +1)) return 0;    
    return true;
}

bool IAPossibleMoves::CheckPutBottomRightBlack(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardBlack(x+1, y +1)) return 0;
    if (board.IsPawnOnPos(x+1, y +1)) return 0;
    return true;
}

bool IAPossibleMoves::CanIPutHereBlack(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    bool ponsFlag = board.GetBlackPawnPons(number);

    if ((x == pos.X()-2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopLeftBlack(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()+2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopRightBlack(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()-1)&&(y == pos.Y()-1))
    {
        if (CheckPutTopLeftBlack(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()+1)&&(y == pos.Y()-1))
    {
        if (CheckPutTopRightBlack(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if (ponsFlag)
    {
        if ((x == pos.X()-2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomLeftBlack(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()+2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomRightBlack(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()-1)&&(y == pos.Y()+1))
        {
            if (CheckPutBottomLeftBlack(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()+1)&&(y == pos.Y()+1))
        {
            if (CheckPutBottomRightBlack(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };
    };

    return false;
}

bool IAPossibleMoves::IsKillBlack(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board, unsigned short &killed) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    bool ponsFlag = board.GetBlackPawnPons(number);

    if ((x == pos.X()-2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopLeftBlack(pos.X(), pos.Y(), board))
        {
            killed = board.GetWhitePawnNumber(pos.X()-1, pos.Y() -1);
            return 1;
        };
    };

    if ((x == pos.X()+2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopRightBlack(pos.X(), pos.Y(), board))
        {
            killed = board.GetWhitePawnNumber(pos.X()+1, pos.Y() -1);
            return 1;
        };
    };

    if (ponsFlag)
    {
        if ((x == pos.X()-2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomLeftBlack(pos.X(), pos.Y(), board))
            {
                killed = board.GetWhitePawnNumber(pos.X()-1, pos.Y() +1);
                return 1;
            };
        };

        if ((x == pos.X()+2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomRightBlack(pos.X(), pos.Y(), board))
            {
                killed = board.GetWhitePawnNumber(pos.X()+1, pos.Y() +1);
                return 1;
            };
        };
    };

    return 0;
}

bool IAPossibleMoves::OutOfBoardBlack(const unsigned short x, const unsigned short y) const
{
    if (x>7) return 1;
    if (y>7) return 1;

    return 0;
}

bool IAPossibleMoves::CanIGrabWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    bool flag = CheckHitTopLeftWhite(pos.X(), pos.Y(), board) | CheckHitTopRightWhite(pos.X(), pos.Y(), board);
    bool flagPons = CheckHitBottomLeftWhite(pos.X(), pos.Y(), board) | CheckHitBottomRightWhite(pos.X(), pos.Y(), board);

    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitBottomLeftWhite(pos.X(), pos.Y(), board) " << CheckHitBottomLeftWhite(pos.X(), pos.Y(), board);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitBottomRightWhite(pos.X(), pos.Y(), board" << CheckHitBottomRightWhite(pos.X(), pos.Y(), board);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitTopLeftWhite(pos.X(), pos.Y(), board) " << CheckHitTopLeftWhite(pos.X(), pos.Y(), board);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "CheckHitTopRightWhite(pos.X(), pos.Y(), board" << CheckHitTopRightWhite(pos.X(), pos.Y(), board);

    bool otherFlag = false;
    for (unsigned short i=0; i < board.GetNumberOfWhite();i++)
    {
        if (i!=number)
        {
            PawnPos pos = board.GetWhitePawnPos(i);
            otherFlag = otherFlag | CheckHitTopLeftWhite(pos.X(), pos.Y(), board) | CheckHitTopRightWhite(pos.X(), pos.Y(), board);

            if (board.GetBlackPawnPons(i))
            {
                otherFlag = otherFlag | CheckHitBottomLeftWhite(pos.X(), pos.Y(), board) | CheckHitBottomRightWhite(pos.X(), pos.Y(), board);
            };
        };
    };

    Traces() << "\n" << "otherFlag " << otherFlag;

    if (!board.GetWhitePawnPons(number))
    {
        if (!flag)
        {
            if (otherFlag)
            {
                return false;
            };
        };

        if (flag) return 1;
        return CheckPutTopLeftWhite(pos.X(), pos.Y(), board) | CheckPutTopRightWhite(pos.X(), pos.Y(), board);
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
        return CheckPutTopLeftWhite(pos.X(), pos.Y(), board) | CheckPutTopRightWhite(pos.X(), pos.Y(), board) | CheckPutBottomLeftWhite(pos.X(), pos.Y(), board) | CheckPutBottomRightWhite(pos.X(), pos.Y(), board);
    };
}


bool IAPossibleMoves::CheckHitTopLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x-1, y - 1)) return 0;
    if (!board.IsBlackPawnOnPos(x-1,y-1))
    {
        return 0;
    } else
    {
        if (OutOfBoardWhite(x-2, y - 2)) return 0;
        if (board.IsPawnOnPos(x-2,y-2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckHitTopRightWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x+1, y -1)) return 0;
    if (!board.IsBlackPawnOnPos(x+1,y-1))
    {
        return 0;
    } else
    {
        if (OutOfBoardWhite(x +2, y -2)) return 0;
        if (board.IsPawnOnPos(x+2,y-2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckHitBottomLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x-1, y + 1)) return 0;
    if (!board.IsBlackPawnOnPos(x-1,y+1))
    {
        return 0;
    } else
    {
        if (OutOfBoardWhite(x-2, y + 2)) return 0;
        if (board.IsPawnOnPos(x-2,y+2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckHitBottomRightWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x+1, y +1)) return 0;
    if (!board.IsBlackPawnOnPos(x+1,y+1))
    {
        return 0;
    } else
    {
        if (OutOfBoardWhite(x +2, y +2)) return 0;
        if (board.IsPawnOnPos(x+2,y+2)) return 0;
        return 1;
    };
}

bool IAPossibleMoves::CheckPutTopLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x-1, y -1)) return 0;
    if (board.IsPawnOnPos(x-1, y -1)) return 0;
    return true;
}

bool IAPossibleMoves::CheckPutTopRightWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x+1, y -1)) return 0;
    if (board.IsPawnOnPos(x+1, y -1)) return 0;
    return true;
}

bool IAPossibleMoves::CheckPutBottomLeftWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x-1, y +1)) return 0;
    if (board.IsPawnOnPos(x-1, y +1)) return 0;
    return true;
}

bool IAPossibleMoves::CheckPutBottomRightWhite(const unsigned short x, const unsigned short y, const Board & board) const
{
    if (OutOfBoardWhite(x+1, y +1)) return 0;
    if (board.IsPawnOnPos(x+1, y +1)) return 0;
    return true;
}

//

bool IAPossibleMoves::CheckHitTopLeftWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckHitTopLeftWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckHitTopRightWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckHitTopRightWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckHitBottomLeftWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckHitBottomLeftWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckHitBottomRightWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckHitBottomRightWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutTopLeftWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckPutTopLeftWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutTopRightWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckPutTopRightWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutBottomLeftWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckPutBottomLeftWhite(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutBottomRightWhite(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    return CheckPutBottomRightWhite(pos.X(),pos.Y(),board);
}

void IAPossibleMoves::KillHitTopLeftWhite(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitTopLeftWhite(" << number << ", board)";
    PawnPos pos = board.GetWhitePawnPos(number);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: " << board.GetBlackPawnNumber(pos.X()-1,pos.Y()-1);
    board.RemoveBlackPawn(board.GetBlackPawnNumber(pos.X()-1,pos.Y()-1));

    board.SetWhitePawnPos(number,pos.X()-2,pos.Y()-2);

}

void IAPossibleMoves::KillHitTopRightWhite(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitTopRightWhite(" << number << ", board)";
    PawnPos pos = board.GetWhitePawnPos(number);
    board.RemoveBlackPawn(board.GetBlackPawnNumber(pos.X()+1,pos.Y()-1));
    board.SetWhitePawnPos(number,pos.X()+2,pos.Y()-2);
}

void IAPossibleMoves::KillHitBottomLeftWhite(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitBottomLeftWhite(" << number << ", board)";
    PawnPos pos = board.GetWhitePawnPos(number);
    board.RemoveBlackPawn(board.GetBlackPawnNumber(pos.X()-1,pos.Y()+1));
    board.SetWhitePawnPos(number,pos.X()-2,pos.Y()+2);
}

void IAPossibleMoves::KillHitBottomRightWhite(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitBottomRightWhite(" << number << ", board)";
    PawnPos pos = board.GetWhitePawnPos(number);
    board.RemoveBlackPawn(board.GetBlackPawnNumber(pos.X()+1,pos.Y()+1));
    board.SetWhitePawnPos(number,pos.X()+2,pos.Y()+2);
}
//
//NEW

bool IAPossibleMoves::CheckHitTopLeftBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckHitTopLeftBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckHitTopRightBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckHitTopRightBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckHitBottomLeftBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckHitBottomLeftBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckHitBottomRightBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckHitBottomRightBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutTopLeftBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckPutTopLeftBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutTopRightBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckPutTopRightBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutBottomLeftBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckPutBottomLeftBlack(pos.X(),pos.Y(),board);
}

bool IAPossibleMoves::CheckPutBottomRightBlack(const unsigned short number, const Board & board) const
{
    PawnPos pos = board.GetBlackPawnPos(number);
    return CheckPutBottomRightBlack(pos.X(),pos.Y(),board);
}

void IAPossibleMoves::KillHitTopLeftBlack(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitTopLeftBlack(" << number << ", board)";
    PawnPos pos = board.GetBlackPawnPos(number);
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: " << board.GetWhitePawnNumber(pos.X()-1,pos.Y()-1);
    board.RemoveWhitePawn(board.GetWhitePawnNumber(pos.X()-1,pos.Y()-1));

    board.SetBlackPawnPos(number,pos.X()-2,pos.Y()-2);

}

void IAPossibleMoves::KillHitTopRightBlack(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitTopRightBlack(" << number << ", board)";
    PawnPos pos = board.GetBlackPawnPos(number);
    board.RemoveWhitePawn(board.GetWhitePawnNumber(pos.X()+1,pos.Y()-1));
    board.SetBlackPawnPos(number,pos.X()+2,pos.Y()-2);
}

void IAPossibleMoves::KillHitBottomLeftBlack(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitBottomLeftBlack(" << number << ", board)";
    PawnPos pos = board.GetBlackPawnPos(number);
    board.RemoveWhitePawn(board.GetWhitePawnNumber(pos.X()-1,pos.Y()+1));
    board.SetBlackPawnPos(number,pos.X()-2,pos.Y()+2);
}

void IAPossibleMoves::KillHitBottomRightBlack(const unsigned short number, Board &board)
{
    TRACE_FLAG_FOR_CLASS_PossibleMoves Traces() << "\n" << "LOG: IAPossibleMoves::KillHitBottomRightBlack(" << number << ", board)";
    PawnPos pos = board.GetBlackPawnPos(number);
    board.RemoveWhitePawn(board.GetWhitePawnNumber(pos.X()+1,pos.Y()+1));
    board.SetBlackPawnPos(number,pos.X()+2,pos.Y()+2);
}
//NEW

bool IAPossibleMoves::CanIPutHereWhite(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    bool ponsFlag = board.GetWhitePawnPons(number);

    if ((x == pos.X()-2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopLeftWhite(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()+2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopRightWhite(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()-1)&&(y == pos.Y()-1))
    {
        if (CheckPutTopLeftWhite(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if ((x == pos.X()+1)&&(y == pos.Y()-1))
    {
        if (CheckPutTopRightWhite(pos.X(), pos.Y(), board))
        {
            return 1;
        };
    };

    if (ponsFlag)
    {
        if ((x == pos.X()-2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomLeftWhite(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()+2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomRightWhite(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()-1)&&(y == pos.Y()+1))
        {
            if (CheckPutBottomLeftWhite(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };

        if ((x == pos.X()+1)&&(y == pos.Y()+1))
        {
            if (CheckPutBottomRightWhite(pos.X(), pos.Y(), board))
            {
                return 1;
            };
        };
    };

    return false;
}

bool IAPossibleMoves::IsKillWhite(const unsigned short number, const unsigned short x, const unsigned short y, const Board & board, unsigned short &killed) const
{
    PawnPos pos = board.GetWhitePawnPos(number);
    bool ponsFlag = board.GetWhitePawnPons(number);

    if ((x == pos.X()-2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopLeftWhite(pos.X(), pos.Y(), board))
        {
            killed = board.GetBlackPawnNumber(pos.X()-1, pos.Y() -1);
            return 1;
        };
    };

    if ((x == pos.X()+2)&&(y == pos.Y()-2))
    {
        if (CheckHitTopRightWhite(pos.X(), pos.Y(), board))
        {
            killed = board.GetBlackPawnNumber(pos.X()+1, pos.Y() -1);
            return 1;
        };
    };

    if (ponsFlag)
    {
        if ((x == pos.X()-2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomLeftWhite(pos.X(), pos.Y(), board))
            {
                killed = board.GetBlackPawnNumber(pos.X()-1, pos.Y() +1);
                return 1;
            };
        };

        if ((x == pos.X()+2)&&(y == pos.Y()+2))
        {
            if (CheckHitBottomRightWhite(pos.X(), pos.Y(), board))
            {
                killed = board.GetBlackPawnNumber(pos.X()+1, pos.Y() +1);
                return 1;
            };
        };
    };

    return 0;
}

bool IAPossibleMoves::OutOfBoardWhite(const unsigned short x, const unsigned short y) const
{
    if (x>7) return 1;
    if (y>7) return 1;

    return 0;
}
