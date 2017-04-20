package pl.programvariables;

public class ProgramVariables 
{
	
	public static void SetServerIP(final String ip) 
    { 
    	serverIP = ip; 
    }
	public static void SetServerPort(final int port) 
    { 
    	serverPort = port;
    }

    public static void SetNumOfAnalyded(final long val) 
    { 
    	numOfAnalysded = val; 
    }
    
    public static long GetNumOfAnalyded() 
    { 
    	return numOfAnalysded; 
    }
    
    public static void IncreaseNumOfAnalyded(final long val) 
    { 
    	numOfAnalysded+= val; 
    }    	
	
	public static long GetSecondsSinceEpoch()
	{	    
	    return System.currentTimeMillis() / 1000;
	}

	public static long GetMaxTimeWaitToWorkers()
	{
	    return 5;
	}

	public static long GetMaxTimeWaitToServer()
	{
	    return 5000;
	}

	String GetServerIP()
	{
	    return serverIP;
	}

	public static int GetServerPort()
	{
	   return serverPort;
	}

	public static int GetRecconectingTime()
	{
	    return 5000;
	}

	public static long GetMaxSecondsToEnd()
	{
	    return 3;
	}

	public static long GetMaxMilisecondsForIa()
	{    
	    return maxTimeForIA;
	}

	public static void SetMaxMilisecondsForIa(final long maxTime)
	{
	    maxTimeForIA = maxTime;
	}

	//Trace flags

	public static boolean GetTraceFlagForClass_Board()
	{
	    return traceFlagForClass_Board;
	}

	public static boolean GetTraceFlagForClass_Pawn()
	{
	    return traceFlagForClass_Pawn;
	}

	public static boolean GetTraceFlagForClass_PawnPos()
	{
	    return traceFlagForClass_PawnPos;
	}

	public static boolean GetTraceFlagForClass_ThreadIASimpleBoard()
	{
	    return traceFlagForClass_ThreadIASimpleBoard;
	}

	public static boolean GetTraceFlagForClass_Peers()
	{
	    return traceFlagForClass_Peers;
	}

	public static boolean GetTraceFlagForClass_MessageCoder()
	{
	    return traceFlagForClass_MessageCoder;
	}

	public static boolean GetTraceFlagForClass_ServerState()
	{
	    return traceFlagForClass_ServerState;
	}

	public static boolean GetTraceFlagForClass_CheckerArea()
	{
	    return traceFlagForClass_CheckerArea;
	}

	public static boolean GetTraceFlagForClass_TCPHandler()
	{
	    return traceFlagForClass_TCPHandler;
	}

	public static boolean GetTraceFlagForClass_PossibleMoves()
	{
	    return traceFlagForClass_PossibleMoves;
	}

	public static final int K4 =  4 * 1024;
	private static String serverIP = "127.0.0.1";
	private static int serverPort = 6000;
	private static long maxTimeForIA = 5000;

	private static boolean traceFlagForClass_Board = false;
	private static boolean traceFlagForClass_Pawn = false;
	private static boolean traceFlagForClass_PawnPos = false;
	private static boolean traceFlagForClass_ThreadIASimpleBoard = false;
	private static boolean traceFlagForClass_Peers = false;
	private static boolean traceFlagForClass_MessageCoder = false;
	private static boolean traceFlagForClass_ServerState = false;
	private static boolean traceFlagForClass_CheckerArea = false;
	private static boolean traceFlagForClass_TCPHandler = false;
	private static boolean traceFlagForClass_PossibleMoves = false;


	private static long numOfAnalysded = 0;
	
}
