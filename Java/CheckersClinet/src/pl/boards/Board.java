package pl.boards;

import java.io.IOException;

import Trace.Traces;
import pl.boards.*;
import pl.programvariables.*;

public class Board 
{
	public Board()
	{
	    Clear();
	}

	public void Clear()
	{
	    SetPreviousMurder(12);
	    SetNullBoard(false);    
	    whitePatchEnd = true;

	    for (int i =0; i<12; i++)
	    {
	    	black[i] = new Pawn();
	    	white[i] = new Pawn();
	    	
	        black[i].dead = true;
	        white[i].dead = true;
	    }
	}

	public void Mirror()
	{
	    Pawn temp;

	    for (int i =0; i<12; i++)
	    {
	        temp = black[i];
	        black[i] = white[i];
	        white[i] = temp;
	    }

	    for (int i =0; i<12; i++)
	    {
	        white[i].x = (7 - white[i].x);
	        white[i].y = (7 - white[i].y);
	    }

	    for (int i =0; i<12; i++)
	    {
	        black[i].x =  (7 - black[i].x);
	        black[i].y =  (7 - black[i].y);
	    }
	}

	public void AddBlackPawn(final int x, final int y, final boolean dead, final boolean pons)
	{
	    if (((y+1) % 2 == 1)&&((x+1) % 2 == 1))
	    {
	          Traces.Debug("\n" + "ERROR! oard::AddBlackPawn(final int x, final int y, final boolean dead, final boolean pons) Try to set pawn on white field!");
	    } else
	    if (((y+1) % 2 == 0)&&((x+1) % 2 == 0))
	    {
	         Traces.Debug("\n" + "ERROR! oard::AddBlackPawn(final int x, final int y, final boolean dead, final boolean pons) Try to set pawn on white field!");
	    } else
	    if (IsPawnOnPos(x,y))
	    {
	        Traces.Debug("\n" + "ERROR! oard::AddBlackPawn(final int x, final int y, final boolean dead, final boolean pons) Try to set pawn on another pawn!");
	    } else
	    if (numberOfBlack() > 12)
	    {
	        Traces.Debug("\n" + "ERROR! oard::AddBlackPawn(final int x, final int y, final boolean dead, final boolean pons) To many pons!");
	    } else
	    {
	        int num = numberOfBlack();
	        black[num].x = x;
	        black[num].y = y;
	        black[num].dead = dead;
	        black[num].pons = pons;
	    }
	}

	public void AddWhitePawn(final int x, final int y, final boolean dead, final boolean pons)
	{
	    if (((y+1) % 2 == 1)&&((x+1) % 2 == 1))
	    {
	          Traces.Debug("\n" + "ERROR! oard::AddWhitePawn(final int x, final int y, final boolean dead, final boolean pons) Try to set pawn on white field!");
	    } else
	    if (((y+1) % 2 == 0)&&((x+1) % 2 == 0))
	    {
	         Traces.Debug("\n" + "ERROR! oard::AddWhitePawn(final int x, final int y, final boolean dead, final boolean pons) Try to set pawn on white field!");
	    } else
	    if (IsPawnOnPos(x,y))
	    {
	        Traces.Debug("\n" + "ERROR! oard::AddWhitePawn(final int x, final int y, final boolean dead, final boolean pons) Try to set pawn on another pawn!");
	    } else
	    if (numberOfBlack() > 12)
	    {
	        Traces.Debug("\n" + "ERROR! oard::AddWhitePawn(final int x, final int y, final boolean dead, final boolean pons) To many pons!");
	    } else
	    {
	        int num = numberOfWhite();
	        white[num].x = x;
	        white[num].y = y;
	        white[num].dead = dead;
	        white[num].pons = pons;
	    }
	}

	public void SetWhitePawnPos(final int number,final int x,final int y)
	{
	    if (number>numberOfWhite()-1)
	    {
	        Traces.Debug("\n" + "ERROR! SetWhitePawnPos(int number,int x, int y) Requesting white pawn not exist!");
	    } else
	    if (((y+1) % 2 == 1)&&((x+1) % 2 == 1))
	    {
	          Traces.Debug("\n" + "ERROR! SetWhitePawnPos(int number,int x, int y) Try to set pawn on white field!");
	    } else
	    if (((y+1) % 2 == 0)&&((x+1) % 2 == 0))
	    {
	         Traces.Debug("\n" + "ERROR! SetWhitePawnPos(int number,int x, int y) Try to set pawn on white field!");
	    } else
	    if (IsPawnOnPos(x,y))
	    {
	        Traces.Debug("\n" + "ERROR! SetWhitePawnPos(int number,int x, int y) Try to set pawn on another pawn!");
	    } else
	    {        	        
	        white[number].x = x;
	        white[number].y = y;
	        
	        if (y == 7)
	        {
	        	white[number].pons = true;
	        };
	    };
	}

	public void SetBlackPawnPos(final int number,final int x,final int y)
	{
	    if (number>numberOfBlack()-1)
	    {
	        Traces.Debug("\n" + "ERROR! SetBlackPawnPos(int number,int x, int y) Requesting black pawn not exist!");
	    } else
	    if (((y+1) % 2 == 1)&&((x+1) % 2 == 1))
	    {
	        Traces.Debug("\n" + "ERROR! SetBlackPawnPos(int number,int x, int y) Try to set pawn on white field!");
	    } else
	    if (((y+1) % 2 == 0)&&((x+1) % 2 == 0))
	    {
	        Traces.Debug("\n" + "ERROR! SetBlackPawnPos(int number,int x, int y) Try to set pawn on white field!");
	    } else
	    if (IsPawnOnPos(x,y))
	    {
	        Traces.Debug("\n" + "ERROR! SetBlackPawnPos(int number,int x, int y) Try to set pawn on another pawn!");
	    } else
	    {	        	       
	        black[number].x = x;
	        black[number].y = y;
	        
	        if (y == 0)
	        {
	        	black[number].pons = true;
	        };
	    };
	}

	public void PutWhiteTopLeftPawn(final int number)
	{
	    if (!GetWhitePawnPons(number))
	    {
	        Traces.Debug("\n" + "ERROR! Board::PutWhiteTopLeftPawn(final int number) Pawn is not pons!");
	    } else
	    {
	        PawnPos tempPos = GetWhitePawnPos(number);
	        SetWhitePawnPos(number, (tempPos.X() - 1), (tempPos.Y()-1));
	    };
	}

	public void PutWhiteTopRightPawn(final int number)
	{
	    if (!GetWhitePawnPons(number))
	    {
	        Traces.Debug("\n" + "ERROR! Board::PutWhiteTopRightPawn(final int number) Pawn is not pons!");
	    } else
	    {
	        PawnPos tempPos = GetWhitePawnPos(number);
	        SetWhitePawnPos(number, tempPos.X() +1, tempPos.Y()-1);
	    };
	}

	public void PutWhiteBottomLeftPawn(final int number)
	{
	    PawnPos tempPos = GetWhitePawnPos(number);
	    SetWhitePawnPos(number, tempPos.X() -1, tempPos.Y()+1);
	}

	public void PutWhiteBottomRightPawn(final int number)
	{
	    PawnPos tempPos = GetWhitePawnPos(number);
	    SetWhitePawnPos(number, tempPos.X() +1, tempPos.Y()+1);
	}

	public void PutBlackTopLeftPawn(final int number)
	{
	        PawnPos tempPos = GetBlackPawnPos(number);
	        SetBlackPawnPos(number, tempPos.X() -1, tempPos.Y()-1);
	}

	public void PutBlackTopRightPawn(final int number)
	{
	        PawnPos tempPos = GetBlackPawnPos(number);
	        SetBlackPawnPos(number, tempPos.X() +1, tempPos.Y()-1);
	}

	public void PutBlackBottomLeftPawn(final int number)
	{
	    if (!GetBlackPawnPons(number))
	    {
	        Traces.Debug("\n" + "ERROR! Board::PutBlackBottomLeftPawn(final int number) Pawn is not pons!");
	    } else
	    {
	        PawnPos tempPos = GetBlackPawnPos(number);
	        SetBlackPawnPos(number, tempPos.X() -1, tempPos.Y()+1);
	    };
	}

	public void PutBlackBottomRightPawn(final int number)
	{
	    if (!GetBlackPawnPons(number))
	    {
	        Traces.Debug("\n" + "ERROR! Board::PutBlackBottomRightPawn(final int number) Pawn is not pons!");
	    } else
	    {
	        PawnPos tempPos = GetBlackPawnPos(number);
	        SetBlackPawnPos(number, tempPos.X() +1, tempPos.Y()+1);
	    };
	}

	public void RemoveBlackPawn(final int number)
	{
	    if (number>numberOfBlack()-1)
	    {
	        Traces.Debug("\n" + "ERROR! Board::RemoveBlackPawn(final int number) Requesting white " + String.valueOf(number) + " pawn not exist!");
	    };

	    eraseBlack(number);
	}

	public void RemoveWhitePawn(final int number)
	{
	    if (number>numberOfWhite()-1)
	    {
	        Traces.Debug("\n" + "ERROR! Board::RemoveWhitePawn(final int number) Requesting white " + String.valueOf(number) + " pawn not exist!");
	    };

	    eraseWhite(number);
	}

	public PawnPos GetWhitePawnPos(final int number)
	{
	    if (number>numberOfWhite()-1)
	    {
	        Traces.Debug("\n" + "ERROR! GetWhitePawnPos(final int number) Requesting " + String.valueOf(number) + " white pawn not exist!");
	    } else        
	    {
	        Pawn temp = white[number];
	        return new PawnPos(temp.x, temp.y);
	    };
	    return new PawnPos(0,0);
	}	

	public PawnPos GetBlackPawnPos(final int number)
	{
	    if (number>numberOfBlack()-1)
	    {        
	        Traces.Debug("\n" + "ERROR! GetBlackPawnPos(final int number) Requesting black " + String.valueOf(number) + " pawn not exist!");
	    } else
	    {
	        Pawn temp = black[number];
	        return new PawnPos(temp.x, temp.y);        
	    };
	    return new PawnPos(0,0);
	}

	public boolean GetWhitePawnPons(final int number)
	{
	    if (number>numberOfWhite()-1)
	    {
	        Traces.Debug("\n" + "ERROR! GetWhitePawnPons(final int number) Requesting white " + String.valueOf(number) + " pawn not exist!");
	    } else 
	    {
	        Pawn temp = white[number];
	        return temp.pons;
	    };
	    return false;
	}

	public int GetWhitePawnNumber(final int x, final int y)
	{
	    PawnPos temp;
	    for (int i =0;i<GetNumberOfWhite();i++)
	    {
	       temp = GetWhitePawnPos(i);
	       if ((temp.X() == x)&&(temp.Y() == y))
	       {
	            return i;
	       };
	    };

	    Traces.Debug("\n" + "ERROR! Board::GetWhitePawnNumber(final int x, final int y) Requesting White pawn not exist!");

	    return 0;
	}

	public int GetBlackPawnNumber(final int x, final int y)
	{
	    PawnPos temp;
	    for (int i =0;i<GetNumberOfBlack();i++)
	    {
	       temp = GetBlackPawnPos(i);
	       if ((temp.X() == x)&&(temp.Y() == y))
	       {
	            return i;
	       };
	    };

	    Traces.Debug("\n" + "ERROR! Board::GetBlackPawnNumber(final int x, final int y) Requesting black pawn not exist!");

	    return 0;
	}

	public boolean GetBlackPawnPons(final int number)
	{
	    if (number>numberOfBlack()-1)
	    {        
	        Traces.Debug("\n" + "ERROR! GetBlackPawnPons(final int number) Requesting black pawn not exist!");
	    } else
	    {
	        Pawn temp = black[number];
	        return temp.pons;
	    };
	    return false;
	}

	public boolean SetWhitePawnPons(final int number, final boolean flag)
	{
	    if (number>numberOfWhite()-1)
	    {
	        Traces.Debug("\n" + "ERROR! Board::SetWhitePawnPons(final int number, final boolean flag) Requesting white pawn not exist!");
	    } else
	    {
	        white[number].pons = flag;	        
	        return false;
	    };

	    return true;
	}

	public boolean SetBlackPawnPons(final int number, final boolean flag)
	{
	    if (number>numberOfBlack()-1)
	    {
	        Traces.Debug("\n" + "ERROR! Board::SetWhitePawnPons(final int number, final boolean flag) Requesting white pawn not exist!");
	    } else
	    {
	        black[number].pons = flag;	        
	        return false;
	    };

	    return true;
	}

	public boolean IsPawnOnPos(final int x, final int y)
	{
	    if (numberOfWhite() > 0)
	    {
	        for (int i=0;i <numberOfWhite();i++)
	        {
	            if ((white[i].x == x)&&(white[i].y == y))
	            {
	                return true;
	            };
	        };
	    }

	    if (numberOfBlack() > 0)
	    {
	        for (int i=0;i <numberOfBlack();i++)
	        {
	            if ((black[i].x == x)&&(black[i].y == y))
	            {
	                return true;
	            };
	        };
	    }

	    return false;
	}

	public boolean IsWhitePawnOnPos(final int x, final int y)
	{
	    for (int i=0;i <numberOfWhite();i++)
	    {
	        if ((white[i].x == x)&&(white[i].y == y))
	        {
	            return true;
	        };
	    };

	    return false;
	}

	public boolean IsBlackPawnOnPos(final int x, final int y)
	{
	    for (int i=0;i <numberOfBlack();i++)
	    {
	        if ((black[i].x == x)&&(black[i].y == y))
	        {
	            return true;
	        };
	    };

	    return false;
	}

	public int GetNumberOfWhite()
	{
	   return numberOfWhite();
	}

	public int GetNumberOfBlack()
	{
	    return numberOfBlack();
	}

	public void RemovePawnFrom(final int x, final int y)
	{

	    if (((y+1) % 2 == 1)&&((x+1) % 2 == 1))
	    {
	        Traces.Debug("\n" + "ERROR! Board::RemovePonsFrom(final int x, final int y) Try to set pawn on white field!");
	    } else
	    if (((y+1) % 2 == 0)&&((x+1) % 2 == 0))
	    {
	        Traces.Debug("\n" + "ERROR! Board::RemovePonsFrom(final int x, final int y) Try to set pawn on white field!");
	    };

	    int counter = 0;
	    boolean flag = false;

	    for (int i=0;i <numberOfWhite();i++)
	    {
	        if ((white[i].x == x)&&(white[i].y == y))
	        {
	            eraseWhite(counter);
	            flag = true;
	        };
	        counter ++;
	    };

	    counter = 0;
	    for (int i=0;i <numberOfBlack();i++)
	    {
	        if ((black[i].x == x)&&(black[i].y == y))
	        {
	            eraseBlack(counter);
	            flag = true;
	        };
	        counter++;
	    };

	    if (!flag) Traces.Debug("\n" + "ERROR! Board::RemovePonsFrom(final int x, final int y) Requesting pawn that not exist!");
	}

	public void SetPreviousMurder(final int number)
	{
	    if (number<=12)
	    {
	        previousMurder = number;
	    } else
	    {
	        Traces.Debug("\n" + "ERROR! IADecisionTree::SetPreviousMurder(final int number) Out of range 0-12! Trying to set " + String.valueOf(number));
	    };
	}

	public int GetPreviousMurder()
	{
	    return previousMurder;
	}

	public int GetResult()
	{
	    int result;

	    result = numberOfWhite();
	    result+= -(numberOfBlack() - 12);

	    return result;
	}

	public double GetPercentageResult()
	{
		
	    if (ProgramVariables.GetTraceFlagForClass_Board()) { Traces.Debug("\n" + "LOG: Board::GetPercentageResult()"); };
	    if (numberOfWhite() == 0) return 12;
	    if (numberOfBlack() == 0) return 0;
	    if (ProgramVariables.GetTraceFlagForClass_Board()) { Traces.Debug("\n" + "LOG: Result = " + String.valueOf((double)numberOfBlack() / (double)numberOfWhite())); };
	    return (double)numberOfBlack() / (double) numberOfWhite();
	}

	public void PrintDebug()
	{
	    String line;
	    boolean flag = false;
	    
	    if (ProgramVariables.GetTraceFlagForClass_Board()) { Traces.Debug("LOG: White= " + String.valueOf(GetNumberOfWhite())); };
	    if (ProgramVariables.GetTraceFlagForClass_Board()) { Traces.Debug("LOG: Black= " + String.valueOf(GetNumberOfBlack())); };

	    for (int y=0; y<8; y++)
	    {

	        line ="|";

	        for (int x=0; x<8; x++)
	        {

	            if (IsWhitePawnOnPos(x,y))
	            {
	                int number = GetWhitePawnNumber(x,y);
	                if (GetWhitePawnPons(number))
	                {
	                    line += "W";
	                } else
	                {
	                    line += "w";
	                };
	            } else
	            if (IsBlackPawnOnPos(x,y))
	            {
	                int number = GetBlackPawnNumber(x,y);
	                if (GetBlackPawnPons(number))
	                {
	                    line += "B";
	                } else
	                {
	                    line += "b";
	                };
	            } else
	            if (flag)
	            {
	                line+="\\";
	            }
	            else
	            {
	                line+=" ";
	            };

	            line+="|";
	            flag = !flag;
	        };

	        Traces.Debug(line);
	        flag = !flag;
	    };
	}

	public Board(final String data)
	{
	    //| |w| |w| |w| |w|
	    //|w| |w| |w| |w| |
	    //| |w| |w| |w| |w|
	    //|\| |\| |\| |\| |
	    //| |\| |\| |\| |b|
	    //|b| |b| |b| |\| |
	    //| |b| |b| |b| |b|
	    //|b| |b| |b| |b| |

		Clear();
		
	    clearWhite();
	    clearBlack();

	    for (int y=0;y<8;y++)
	    {
	        int realPositionX;
	        if ((y+1)%2 == 1)
	        {
	            realPositionX = 0;
	        } else
	        {
	            realPositionX = 0;
	        };

	        for (int x=0;x<17;x++)
	        {
	           int position = (y*17)+x;

	           if (data.codePointAt(position) == 'w')
	           {
	             white[numberOfWhite()] = new Pawn(realPositionX,y, false, false);
	             realPositionX += 1;
	           } else
	           if (data.codePointAt(position) == 'W')
	           {
	             white[numberOfWhite()] = new Pawn(realPositionX, y, false, true);

	             realPositionX += 1;
	           } else
	           if (data.codePointAt(position) == 'b')
	           {
	             black[numberOfBlack()] = new Pawn(realPositionX, y, false, false);

	             realPositionX += 1;
	           } else
	           if (data.codePointAt(position) == 'B')
	           {
	             black[numberOfBlack()] = new Pawn(realPositionX, y, false, true);

	             realPositionX += 1;
	           }else
	           if (data.codePointAt(position) == ' ')
	           {
	               realPositionX += 1;
	           } else
	           if (data.codePointAt(position) == '\\')
	           {
	               realPositionX += 1;
	           };

	        };
	    };		
	}

	public boolean equals(Object other)
	{
	    if (other == null) return false;
	    if (other == this) return true;
	    if (!(other instanceof Board))return false;
	    Board data = (Board)other;

	    if (ProgramVariables.GetTraceFlagForClass_Board()) {	    	
		    Traces.Debug("\n" + "LOG: Board::operator ==(Board data)"); 
	    };

	    if (GetNumberOfBlack()!=data.GetNumberOfBlack())
	    {
	      return false;
	    };
	    if (GetNumberOfWhite()!=data.GetNumberOfWhite())
	    {
	      return false;
	    };

	    boolean flag;

	    if (GetNumberOfWhite()>0)
	    {
	        for (int i=0;i<GetNumberOfWhite();i++)
	        {
	            flag = false;
	            for (int i2=0;i2<GetNumberOfWhite();i2++)
	            {
	                if (!(GetWhitePawnPos(i).equals(data.GetWhitePawnPos(i2))))
	                {		                	
	                    if (GetWhitePawnPons(i)==data.GetWhitePawnPons(i2))
	                    {
	                      flag = true;
	                    };
	                };
	            };
	            if (!flag) return false;
	        };
	    };

	    if (GetNumberOfBlack()>0)
	    {
	        for (int i=0;i<GetNumberOfBlack();i++)
	        {
	             flag = false;
	            for (int i2=0;i2<GetNumberOfBlack();i2++)
	            {
	                if (!(GetBlackPawnPos(i) != data.GetBlackPawnPos(i2)))
	                {
	                    if (GetBlackPawnPons(i)==data.GetBlackPawnPons(i2))
	                    {
	                      flag = true;
	                     };
	                };
	            };
	            if (!flag) return false;
	        };
	    };
	    
	    return true;	    	   
	}

	public int numberOfWhite()
	{
	    int size = 0;

	    for (int i=0;i<12;i++)
	    {
	        if (!white[i].dead) size++;
	    };
	    return size;
	}

	public int numberOfBlack()
	{
	    int size = 0;

	    for (int i=0;i<12;i++)
	    {
	        if (!black[i].dead) size++;
	    };
	    return size;
	}

	public void eraseWhite(int num)
	{
	    int numW = numberOfWhite();
	    if (numW == 1)
	    {
	        white[0].dead = true;
	    } else
	    if (num == numW)
	    {
	        white[num].dead = true;
	    } else
	    {
	        for (int i=num;i<numW;i++)
	        {
	            white[i] = white[i+1];
	        };
	        white[numW-1].dead = true;
	    };
	}

	public void eraseBlack(int num)
	{
	    int numB = numberOfBlack();
	    if (numB == 1)
	    {
	        black[0].dead = true;
	    } else
	    if (num == numB)
	    {
	        black[num].dead = true;
	    } else
	    {
	        for (int i=num;i<numB;i++)
	        {
	            black[i] = black[i+1];
	        };
	        black[numB-1].dead = true;
	    };
	}

	public void clearWhite()
	{
	    for (int i=0;i<12;i++)
	    {
	        white[i].dead = true;
	    };
	}

	public void clearBlack()
	{
	    for (int i=0;i<12;i++)
	    {
	        black[i].dead = true;
	    };
	}

	public boolean GetWhitePatchEnd()
	{
		if (ProgramVariables.GetTraceFlagForClass_Board()) { Traces.Debug("\n" + "LOG: public boolean GetWhitePatchEnd() " + String.valueOf(whitePatchEnd)); };
	    return whitePatchEnd;
	}

	public void SetWhitePatchEnd(final boolean flag)
	{
	    if (ProgramVariables.GetTraceFlagForClass_Board()) { Traces.Debug("\n" + "LOG: public void SetWhitePatchEnd(final boolean flag) " + String.valueOf(flag)); };
	    whitePatchEnd = flag;
	}

	public void CopyTo(ThreadIASimpleBoard data)
	{
	    for (int i=0;i<12;i++)
	    {
	        data.black[i] = black[i];
	        data.white[i] = white[i];
	    };
	}

	public void CopyFrom(final ThreadIASimpleBoard data)
	{
	    for (int i=0;i<12;i++)
	    {
	        black[i] = data.black[i];
	        white[i] = data.white[i];
	    };
	}

	public void SetOrigin(Board data)
	{
	    data.CopyTo(origin);
	}

	public Board GetOrigin()
	{
	    Board temp = new Board();
	    temp.CopyFrom(origin);
	    return temp;
	}

	public void StartBlack()
	{
	    blackWhite = false;
	}

	public void StartWhite()
	{
	    blackWhite = true;
	}

	public boolean Black()
	{
	    return !blackWhite;
	}

	public boolean White()
	{
	    return blackWhite;
	}

	public boolean GetNullBoard()
	{
	    return nullBoard;
	}

	public void SetNullBoard(final boolean flag)
	{
	    nullBoard = flag;
	}
	
    private int previousMurder;
    private ThreadIASimpleBoard origin;
    private boolean whitePatchEnd;

    private boolean blackWhite;
    private boolean nullBoard;

    private Pawn[] white = new Pawn[12];
    private Pawn[] black = new Pawn[12];  
}
