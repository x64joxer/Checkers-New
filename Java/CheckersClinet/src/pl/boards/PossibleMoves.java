package pl.boards;

import Trace.Traces;
import pl.programvariables.*;

public class PossibleMoves 
{

	public PossibleMoves()
	{

	}

	public boolean CanIGrab(final int number, final Board  board)
	{
	    PawnPos pos = board.GetBlackPawnPos(number);
	    boolean flag = CheckHitTopLeft(pos.X(), pos.Y(), board) | CheckHitTopRight(pos.X(), pos.Y(), board);
	    boolean flagPons = CheckHitBottomLeft(pos.X(), pos.Y(), board) | CheckHitBottomRight(pos.X(), pos.Y(), board);

	    if (ProgramVariables.GetTraceFlagForClass_PossibleMoves())
	    {
	    	Traces.Debug("\n" + "CheckHitBottomLeft(pos.X(), pos.Y(), board) " + String.valueOf(CheckHitBottomLeft(pos.X(), pos.Y(), board)));
	    	Traces.Debug("\n" + "CheckHitBottomRight(pos.X(), pos.Y(), board" + String.valueOf(CheckHitBottomRight(pos.X(), pos.Y(), board)));
	    	Traces.Debug("\n" + "CheckHitTopLeft(pos.X(), pos.Y(), board) " + String.valueOf(CheckHitTopLeft(pos.X(), pos.Y(), board)));
	    	Traces.Debug("\n" + "CheckHitTopRight(pos.X(), pos.Y(), board" + String.valueOf(CheckHitTopRight(pos.X(), pos.Y(), board)));
	    }


	    boolean otherFlag = false;
	    for (int i=0; i < board.GetNumberOfBlack();i++)
	    {
	        if (i!=number)
	        {
	            PawnPos pos2 = board.GetBlackPawnPos(i);
	            otherFlag = otherFlag | CheckHitTopLeft(pos2.X(), pos2.Y(), board) | CheckHitTopRight(pos2.X(), pos2.Y(), board);

	            if (board.GetBlackPawnPons(i))
	            {
	                otherFlag = otherFlag | CheckHitBottomLeft(pos2.X(), pos2.Y(), board) | CheckHitBottomRight(pos2.X(), pos2.Y(), board);
	            };
	        };
	    };

	    if (ProgramVariables.GetTraceFlagForClass_PossibleMoves()) Traces.Debug("\n" + "otherFlag " + String.valueOf(otherFlag));

	    if (!board.GetBlackPawnPons(number))
	    {
	        if (!flag)
	        {
	            if (otherFlag)
	            {
	                return false;
	            };
	        };

	        if (flag) return true;
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

	        if (flag | flagPons) return true;
	        return CheckPutTopLeft(pos.X(), pos.Y(), board) | CheckPutTopRight(pos.X(), pos.Y(), board) | CheckPutBottomLeft(pos.X(), pos.Y(), board) | CheckPutBottomRight(pos.X(), pos.Y(), board);
	    }
	}


	public boolean CheckHitTopLeft(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x-1, y - 1)) return false;
	    if (!board.IsWhitePawnOnPos(x-1,y-1))
	    {
	        return false;
	    } else
	    {
	        if (OutOfBoard(x-2, y - 2)) return false;
	        if (board.IsPawnOnPos(x-2,y-2)) return false;
	        return true;
	    }
	    	    
	}

	public boolean CheckHitTopRight(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x+1, y -1)) return false;
	    if (!board.IsWhitePawnOnPos(x+1,y-1))
	    {
	        return false;
	    } else
	    {
	        if (OutOfBoard(x +2, y -2)) return false;
	        if (board.IsPawnOnPos(x+2,y-2)) return false;
	        return true;
	    }
	}

	public boolean CheckHitBottomLeft(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x-1, y + 1)) return false;
	    if (!board.IsWhitePawnOnPos(x-1,y+1))
	    {
	        return false;
	    } else
	    {
	        if (OutOfBoard(x-2, y + 2)) return false;
	        if (board.IsPawnOnPos(x-2,y+2)) return false;
	        return true;
	    }
	}

	public boolean CheckHitBottomRight(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x+1, y +1)) return false;
	    if (!board.IsWhitePawnOnPos(x+1,y+1))
	    {
	        return false;
	    } else
	    {
	        if (OutOfBoard(x +2, y +2)) return false;
	        if (board.IsPawnOnPos(x+2,y+2)) return false;
	        return true;
	    }
	}

	public boolean CheckPutTopLeft(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x-1, y -1)) return false;
	    if (board.IsPawnOnPos(x-1, y -1)) return false;
	    return true;
	}

	public boolean CheckPutTopRight(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x+1, y -1)) return false;
	    if (board.IsPawnOnPos(x+1, y -1)) return false;
	    return true;
	}

	public boolean CheckPutBottomLeft(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x-1, y +1)) return false;
	    if (board.IsPawnOnPos(x-1, y +1)) return false;
	    return true;
	}

	public boolean CheckPutBottomRight(final int x, final int y, final Board  board)
	{
	    if (OutOfBoard(x+1, y +1)) return false;
	    if (board.IsPawnOnPos(x+1, y +1)) return false;
	    return true;
	}

	public boolean CanIPutHere(final int number, final int x, final int y, final Board  board)
	{
	    PawnPos pos = board.GetBlackPawnPos(number);
	    boolean ponsFlag = board.GetBlackPawnPons(number);

	    boolean killFlag = CheckHitTopLeft(pos.X(), pos.Y(), board) | CheckHitTopRight(pos.X(), pos.Y(), board);
	    if (ponsFlag)
	    {
	        killFlag = killFlag | CheckHitBottomLeft(pos.X(), pos.Y(), board) | CheckHitBottomRight(pos.X(), pos.Y(), board);
	    };


	    if ((x == pos.X()-2)&&(y == pos.Y()-2))
	    {
	        if (CheckHitTopLeft(pos.X(), pos.Y(), board))
	        {
	            return true;
	        };
	    };

	    if ((x == pos.X()+2)&&(y == pos.Y()-2))
	    {
	        if (CheckHitTopRight(pos.X(), pos.Y(), board))
	        {
	            return true;
	        };
	    };

	    if ((x == pos.X()-1)&&(y == pos.Y()-1))
	    {
	        if (CheckPutTopLeft(pos.X(), pos.Y(), board))
	        {
	            if (!killFlag) return true;
	        };
	    };

	    if ((x == pos.X()+1)&&(y == pos.Y()-1))
	    {
	        if (CheckPutTopRight(pos.X(), pos.Y(), board))
	        {
	            if (!killFlag) return true;
	        };
	    };

	    if (ponsFlag)
	    {
	        if ((x == pos.X()-2)&&(y == pos.Y()+2))
	        {
	            if (CheckHitBottomLeft(pos.X(), pos.Y(), board))
	            {
	                return true;
	            };
	        };

	        if ((x == pos.X()+2)&&(y == pos.Y()+2))
	        {
	            if (CheckHitBottomRight(pos.X(), pos.Y(), board))
	            {
	                return true;
	            };
	        };

	        if ((x == pos.X()-1)&&(y == pos.Y()+1))
	        {
	            if (CheckPutBottomLeft(pos.X(), pos.Y(), board))
	            {
	                if (!killFlag) return true;
	            };
	        };

	        if ((x == pos.X()+1)&&(y == pos.Y()+1))
	        {
	            if (CheckPutBottomRight(pos.X(), pos.Y(), board))
	            {
	               if (!killFlag) return true;
	            };
	        };
	    };

	    return false;
	}

	public boolean IsKill(final int number, final int x, final int y, final Board  board, int killed)
	{
	    PawnPos pos = board.GetBlackPawnPos(number);
	    boolean ponsFlag = board.GetBlackPawnPons(number);

	    if ((x == pos.X()-2)&&(y == pos.Y()-2))
	    {
	        if (CheckHitTopLeft(pos.X(), pos.Y(), board))
	        {
	            killed = board.GetWhitePawnNumber(pos.X()-1, pos.Y() -1);
	            return true;
	        };
	    };

	    if ((x == pos.X()+2)&&(y == pos.Y()-2))
	    {
	        if (CheckHitTopRight(pos.X(), pos.Y(), board))
	        {
	            killed = board.GetWhitePawnNumber(pos.X()+1, pos.Y() -1);
	            return true;
	        };
	    };

	    if (ponsFlag)
	    {
	        if ((x == pos.X()-2)&&(y == pos.Y()+2))
	        {
	            if (CheckHitBottomLeft(pos.X(), pos.Y(), board))
	            {
	                killed = board.GetWhitePawnNumber(pos.X()-1, pos.Y() +1);
	                return false;
	            };
	        };

	        if ((x == pos.X()+2)&&(y == pos.Y()+2))
	        {
	            if (CheckHitBottomRight(pos.X(), pos.Y(), board))
	            {
	                killed = board.GetWhitePawnNumber(pos.X()+1, pos.Y() +1);
	                return true;
	            };
	        };
	    };

	    return false;
	}

	public boolean OutOfBoard(final int x, final int y)
	{
	    if (x>7) return true;
	    if (y>7) return true;

	    return false;
	}
	
}
