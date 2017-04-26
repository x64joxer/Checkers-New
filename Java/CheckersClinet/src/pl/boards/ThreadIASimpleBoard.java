package pl.boards;

import pl.boards.Pawn;

public class ThreadIASimpleBoard 
{	
	public ThreadIASimpleBoard()
	{
		for(int i = 0; i < 12; i++)
		{
			white[i] = new Pawn();
			black[i] = new Pawn();
		}		
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
