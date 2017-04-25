package pl.boards;

public class Pawn
{	

	public Pawn()
	{
	    Clear();
	}

	public Pawn(final Pawn val)
	{
	    x = val.x;
	    y = val.y;
	    dead = val.dead;
	    pons = val.pons;
	}
	
	public Pawn(int xp, int yp, boolean deadp, boolean ponsp)
	{
	    x = xp;
	    y = yp;
	    dead = deadp;
	    pons = ponsp;
	}
	
	public void Clear()
	{
	    dead = true;
	    pons = false;
	}

    public int x = 4;
    public int y = 4;
    public boolean dead = true;
    public boolean pons = true;
}
