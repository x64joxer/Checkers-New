package pl.boards;

import java.io.IOException;

import Trace.*;

public class PawnPos 
{
	public PawnPos()
	{

	}

	public PawnPos(final PawnPos val)
	{
		x = val.x;
		y = val.y;
	}	

	public PawnPos(final int x2, final int y2) 
	{
		x = x2;
	    y = y2;
	}	

	public int X()
	{
	    return x;
	}

	public int Y()
	{
	    return y;
	}

	public void SetX(final int X)
	{
	    if (((y+1) % 2 == 1)&&((X+1) % 2 == 1))
	    {
	          Traces.Debug("\n" + "ERROR! PawnPos::SetX(final int X) Try to set pawn on white field!");
	    } else
	    if (((y+1) % 2 == 0)&&((X+1) % 2 == 0))
	    {
	         Traces.Debug("\n" + "ERROR! PawnPos::SetX(final int X) Try to set pawn on white field!");
	    } else
	    {
	        x = X;
	    };
	}

	public void SetY(final int Y)
	{
	    if (((Y+1) % 2 == 1)&&((x+1) % 2 == 1))
	    {
	          Traces.Debug("\n" + "ERROR! PawnPos::SetY(final int Y) Try to set pawn on white field!");
	    } else
	    if (((Y+1) % 2 == 0)&&((x+1) % 2 == 0))
	    {
	         Traces.Debug("\n" + "ERROR! PawnPos::SetY(final int Y) Try to set pawn on white field!");
	    } else
	    {
	        y = Y;
	    };
	}

	public boolean equals(Object other)
	{
	    if (other == null) return false;
	    if (other == this) return true;
	    if (!(other instanceof PawnPos))return false;
	    PawnPos data = (PawnPos)other;

	    if (X()==data.X())
	    {
	        if (Y()==data.Y())
	        {
	            return false;
	        };
	    };
	    
	    return true;
	}
	
    private int x = 4;
    private int y = 4;
}
