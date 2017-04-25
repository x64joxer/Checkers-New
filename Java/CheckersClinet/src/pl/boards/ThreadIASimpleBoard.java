package pl.boards;

import pl.boards.Pawn;

public class ThreadIASimpleBoard 
{	
	public ThreadIASimpleBoard()
	{
		
	}
	
	public ThreadIASimpleBoard(final ThreadIASimpleBoard val)
	{
		for(int i = 0; i < 12; i++)
		{
			white[i] = new Pawn(val.white[i]);
			black[i] = new Pawn(val.black[i]);
		}
	}
	
    public Pawn[] white = new Pawn[12];
    public Pawn[] black = new Pawn[12];
}
