package pl.tcp;

import java.util.HashMap;

import pl.boards.*;
import Trace.Traces;
import pl.programvariables.*;
import pl.serverstate.*;
import pl.tcp.*;

public class MessageCoder 
{
	
	public MessageCoder()
	{

	}
	
	public static boolean IntToBoolean(final int val)
	{
		if (val == 0) return false;
		return true;
	}		

	public static String KeyValuePairToString(final String key, final String value, final String source)
	{	       
	        return new String(source 
	        			+ "<" 
	        			+ key
	        			+ "><"
	        			+ value
	        			+ ">"
	        			);
	}

	public static String KeyValuePairToString(final String key, final int value, final String source)
	{
	    return KeyValuePairToString(key, String.valueOf(value), source);
	}

	public static String KeyValuePairToString(final String key, final long value, final String source)
	{
	    return KeyValuePairToString(key, String.valueOf(value), source);
	}

	public static String KeyValuePairToString(final String key, final boolean value, final String source)
	{
		if (value == true)
		{
			return KeyValuePairToString(key, "1", source);	
		}
		
	    return KeyValuePairToString(key, "0", source);
	}

	public static String ClearString()
	{	
	    return  "";
	}

	public static String InsertHeader(final String source)
	{
		String copy = new String(source);		
		
		if (source.length() > 4)
		{						    			    
			copy.replaceFirst("^[a-zA-Z0-9]{4,4}", "AAAA");
		} else
		{
			copy = "AAAA";		   
		}
		
	    return copy;
	}


	public static String InsertLenMessageHeader(final String source)
	{
		String copy = new String(source);
		int size = source.length() - BufferSize();
		
		if (source.length() > 4)
		{			
			String header = String.valueOf((char)((size & 0x000F) + 65)) +
			  				String.valueOf((char)(((size & 0x00F0) >> 4) + 65)) +
			   				String.valueOf((char)(((size & 0x0F00) >> 8) + 65)) +
			   				String.valueOf((char)(((size & 0xF000) >> 12) + 65));
			
		    copy = copy.replaceFirst("[a-zA-Z0-9]{4,4}", header);
			    
			    
			System.out.print(copy);    
		} else
		{
			copy = "AAAA";		   
		}
		
	    return copy;
	}

	public static String InsertLenMessageHeader(final int val, final String source)
	{
		String copy = new String(source);		
		
		if (source.length() > 4)
		{						    
			
			String header = String.valueOf((char)((val & 0x000F) + 65)) +
	  				String.valueOf((char)(((val & 0x00F0) >> 4) + 65)) +
	   				String.valueOf((char)(((val & 0x0F00) >> 8) + 65)) +
	   				String.valueOf((char)(((val & 0xF000) >> 12) + 65));
	
			copy = copy.replaceFirst("[a-zA-Z0-9]{4,4}", header);			
		} else
		{
			copy = String.valueOf((val & 0x000F) + 65) +
    		 	   String.valueOf(((val & 0x00F0) >> 4) + 65) +
    		 	   String.valueOf(((val & 0x0F00) >> 8) + 65) +
    		 	   String.valueOf(((val & 0xF000) >> 12) + 65);		   
		}
		
	    return copy;
	}

	int HeaderToVal(final String source)
	{
		 char[] dest = new char[4];
		 source.getChars(0, 3, dest, 0);
		
	     int result;
	     result =  (dest[0] - 65);
	     result += (dest[1] - 65) << 4;
	     result += (dest[2] - 65) << 8;
	     result += (dest[3] - 65) << 12;

	     return result;
	}

	public static void MessageToMap(final String source, HashMap<String, String> dest)
	{    
	    int i = 0;
	    int begin = 0;
	    int end = 0;
	    int begin_val = 0;
	    int end_val = 0;
	    String keyString = "";
	    boolean key = false;

	    dest.clear();

	    while (i < source.length())
	    {
	        if (key == false)
	        {	        	
	            if (source.substring(i, i+1).equals("<") == true) 
	            {	            	
	            	begin = i;
	            }
	            if (source.substring(i, i+1).equals(">") == true)
	            {
	                end = i;
	                key = true;
	            };
	        } else
	        {
	            if (source.substring(i, i+1).equals("<") == true) begin_val = i;
	            if (source.substring(i, i+1).equals(">") == true)
	            {                
	                end_val = i;
	                key = false;
	                keyString = source.substring(begin + 1, end);                	                
	                dest.put(keyString, source.substring(begin_val + 1, end_val));	                
	            };
	        }

	        if (keyString.equals(MESSAGE_END) == true) break;

	        i++;
	    };

	}

	public static String BoardToString(final Board board, final int numberOfBoard, String dest)
	{		
		
	    if (ProgramVariables.GetTraceFlagForClass_MessageCoder()) Traces.Debug("LOG: BoardToString(final Board board, String dest, final int numberOfBoard)");

	    dest = KeyValuePairToString(PREVIOUS_MURDER, board.GetPreviousMurder(), dest);
	    dest = KeyValuePairToString(WHITE_PATCH_END, board.GetWhitePatchEnd(), dest);
	    dest = KeyValuePairToString(BLACK_WHITE, board.Black(), dest);

	    //Origin

	    Board temp = board.GetOrigin();
	    String prefix = String.valueOf(numberOfBoard) + "_";
	    PawnPos pawnPos;
	    int num;

	    num = temp.GetNumberOfBlack();
	    dest = KeyValuePairToString(prefix + ORIGIN_NUMBER_BLACK, num, dest);

	    if (num > 0)
	    {
	        for (int i = 0; i<num; i++)
	        {
	            pawnPos = temp.GetBlackPawnPos(i);
	            dest = KeyValuePairToString(prefix + ORIGIN_BLACK_X + String.valueOf(i), pawnPos.X(), dest);
	            dest = KeyValuePairToString(prefix + ORIGIN_BLACK_Y + String.valueOf(i), pawnPos.Y(), dest);
	            dest = KeyValuePairToString(prefix + ORIGIN_BLACK_PONS + String.valueOf(i), temp.GetBlackPawnPons(i), dest);
	        }
	    }

	    num = temp.GetNumberOfWhite();
	    dest = KeyValuePairToString(prefix + ORIGIN_NUMBER_WHITE, num, dest);

	    if (num > 0)
	    {
	        for (int i = 0; i<num; i++)
	        {
	            pawnPos = temp.GetWhitePawnPos(i);
	            dest = KeyValuePairToString(prefix + ORIGIN_WHITE_X + String.valueOf(i), pawnPos.X(), dest);
	            dest = KeyValuePairToString(prefix + ORIGIN_WHITE_Y + String.valueOf(i), pawnPos.Y(), dest);
	            dest = KeyValuePairToString(prefix + ORIGIN_WHITE_PONS + String.valueOf(i), temp.GetWhitePawnPons(i), dest);
	        }
	    }


	    //Board

	    num = board.GetNumberOfBlack();
	    dest = KeyValuePairToString(prefix + NUMBER_BLACK, num, dest);

	    if (ProgramVariables.GetTraceFlagForClass_MessageCoder()) board.PrintDebug();

	    if (num > 0)
	    {
	        for (int i = 0; i<num; i++)
	        {            
	            pawnPos = board.GetBlackPawnPos(i);
	            dest = KeyValuePairToString(prefix + BLACK_X + String.valueOf(i), pawnPos.X(), dest);            
	            dest = KeyValuePairToString(prefix + BLACK_Y + String.valueOf(i), pawnPos.Y(), dest);           
	            dest = KeyValuePairToString(prefix + BLACK_PONS + String.valueOf(i), board.GetBlackPawnPons(i), dest);
	        }
	    }

	    num = board.GetNumberOfWhite();
	    dest = KeyValuePairToString(prefix + NUMBER_WHITE, num, dest);

	    if (num > 0)
	    {
	        for (int i = 0; i<num; i++)
	        {
	            pawnPos = board.GetWhitePawnPos(i);
	            dest = KeyValuePairToString(prefix + WHITE_X + String.valueOf(i), pawnPos.X(), dest);
	            dest = KeyValuePairToString(prefix + WHITE_Y + String.valueOf(i), pawnPos.Y(), dest);
	            dest = KeyValuePairToString(prefix + WHITE_PONS + String.valueOf(i), board.GetWhitePawnPons(i), dest);
	        }
	    }

	    dest = KeyValuePairToString(MESSAGE_END, 0, dest);
	    
	    return dest;
	}

	
	public static Board MapToBoard(final HashMap<String, String> dest, final Board board) throws NullPointerException
	{
	    try
	    {
	        board.Clear();
	        
	        board.SetPreviousMurder(Integer.parseInt(dest.get(PREVIOUS_MURDER)));
	        
	        if (Integer.parseInt(dest.get(WHITE_PATCH_END)) == 0)
	        {
	        	board.SetWhitePatchEnd(false);
	        } else
	        {
	        	board.SetWhitePatchEnd(true);	
	        }	        

	        if (Integer.parseInt(dest.get(BLACK_WHITE)) == 0)
	        {
	            board.StartBlack();
	        } else
	        {
	            board.StartWhite();
	        }

	        Board origin = new Board();
	        String prefix = "1_";

	        String temp = "";
	        String tempX = "";
	        String tempY = "";
	        String tempPons = "";

	        temp = dest.get(prefix + ORIGIN_NUMBER_BLACK);
	        int numBlack = Integer.parseInt(temp);
	        temp = dest.get(prefix + ORIGIN_NUMBER_WHITE);
	        int numWhite = Integer.parseInt(temp);
	       
	        if (numBlack > 0)
	        {
	            for (int i = 0; i<numBlack; i++)
	            {
	                tempX = dest.get(prefix + ORIGIN_BLACK_X + String.valueOf(i));
	                tempY = dest.get(prefix + ORIGIN_BLACK_Y + String.valueOf(i));
	                tempPons = dest.get(prefix + ORIGIN_BLACK_PONS + String.valueOf(i));

	                origin.AddBlackPawn(Integer.parseInt(tempX),
	                					Integer.parseInt(tempY),
	                                    false,
	                                    IntToBoolean(Integer.parseInt(tempPons)));
	            }
	        }


	        if (numWhite > 0)
	        {
	            for (int i = 0; i<numWhite; i++)
	            {
	                tempX = dest.get(prefix + ORIGIN_WHITE_X + String.valueOf(i));
	                tempY = dest.get(prefix + ORIGIN_WHITE_Y + String.valueOf(i));
	                tempPons = dest.get(prefix + ORIGIN_WHITE_PONS + String.valueOf(i));

	                origin.AddWhitePawn(Integer.parseInt(tempX),
	                					Integer.parseInt(tempY),
	                                    false,
	                                    IntToBoolean(Integer.parseInt(tempPons)));
	            }
	        }

	        board.SetOrigin(origin);

	        temp = dest.get(prefix + NUMBER_BLACK);
	        numBlack = Integer.parseInt(temp);
	        temp = dest.get(prefix + NUMBER_WHITE);
	        numWhite = Integer.parseInt(temp);

	        if (numBlack > 0)
	        {
	            for (int i = 0; i<numBlack; i++)
	            {
	                tempX = dest.get(prefix + BLACK_X + String.valueOf(i));
	                tempY = dest.get(prefix + BLACK_Y + String.valueOf(i));
	                tempPons = dest.get(prefix + BLACK_PONS + String.valueOf(i));

	                board.AddBlackPawn(Integer.parseInt(tempX),
	                					Integer.parseInt(tempY),
	                                    false,
	                                    IntToBoolean(Integer.parseInt(tempPons)));
	            }
	        }

	        if (numWhite > 0)
	        {
	            for (int i = 0; i<numWhite; i++)
	            {
	                tempX = dest.get(prefix + WHITE_X + String.valueOf(i));
	                tempY = dest.get(prefix + WHITE_Y + String.valueOf(i));
	                tempPons = dest.get(prefix + WHITE_PONS + String.valueOf(i));

	                board.AddWhitePawn(Integer.parseInt(tempX),
	                					Integer.parseInt(tempY),
	                                    false,
	                                    IntToBoolean(Integer.parseInt(tempPons)));
	            }
	        }
	        
	        return board;
	        
	    } catch(NullPointerException e) 
	    {
	    	throw e;
	    }	    	   
	}

	public static String CreateConnectedMessage(String dest)
	{
	    dest = InsertHeader(dest);
	    dest = KeyValuePairToString(ACTION, CONNECTED, dest);
	    dest = KeyValuePairToString(MESSAGE_END, 0, dest);
	    dest = InsertLenMessageHeader(dest);
	    
	    return dest;
	}

	public static String CreateCloseConnectionMessage(String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, CLOSE_CNNECTION, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);    
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateTimeToSendResultToClientsMessage(String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, TIME_TO_SEND_RESULT_TO_CLIENTS, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateTimeoutMessage(String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, TIMEOUT, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateCanNotMoveMessage(String dest, final String jobId)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, CAN_NOT_MOVE, dest);
		dest = KeyValuePairToString(JOB_ID, jobId, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateStartAnalyseWorkAndReturnNResultFast(final long respTime, final int numOfResultToReturnFast, final Board board, final String id, final String jobId, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, START_ANALYSE_FAST, dest);
		dest = KeyValuePairToString(MAX_TIME, respTime, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);    
		dest = KeyValuePairToString(JOB_ID, jobId, dest);    
		dest = KeyValuePairToString(IS_FIRST_WORKER, true, dest);
		dest = KeyValuePairToString(NUM_OF_BOARD_TO_RETURN_FAST, numOfResultToReturnFast, dest);
		dest = BoardToString(board, 1, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateStartAnalyseWork(final long respTime , final Board board, final String id, final String jobId, final boolean isFirstWorker, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, START_ANALYSE, dest);
		dest = KeyValuePairToString(MAX_TIME, respTime, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(JOB_ID, jobId, dest);
		dest = KeyValuePairToString(IS_FIRST_WORKER, isFirstWorker, dest);
		dest = BoardToString(board, 1, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateResetServerStateMessage(final Board board, final String id, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, RESET_SERVER_STATE, dest);
		dest = BoardToString(board, 1, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateStartMessage(final long respTime, final int numberOfBoard, final String id, final String jobId, final Board board, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, START_WORK, dest);
		dest = KeyValuePairToString(MAX_TIME, respTime, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(JOB_ID, jobId, dest);
		dest = KeyValuePairToString(NUM_OF_BOARD, numberOfBoard, dest);
		dest = BoardToString(board, 1, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateBestResultMessage(final Board board, final String id, final String jobId, long numOfAnalysed, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, BEST_RESULT, dest);
		dest = KeyValuePairToString(NUM_OF_ANALYSED, numOfAnalysed, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(JOB_ID, jobId, dest);    
		dest = BoardToString(board, 1, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateGetServerStateMessage(final String id, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, GET_SERVER_STATE, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateStateMessage(final Peers.STATE stat, final int numOfThread, final String id, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, SET_STATE, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(STATE, stat.getValue(), dest);
		dest = KeyValuePairToString(NUM_OF_THREAD, numOfThread, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);        
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateServerStateMessage(final ServerState serverState, final String id, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, SERVER_STATE, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);

		dest = KeyValuePairToString(IS_THINKING, serverState.IsThinking(), dest);
		dest = KeyValuePairToString(MAX_IA_TIME, serverState.GetMaxTime(), dest);
		dest = KeyValuePairToString(START_TIME, serverState.GetStartTime(), dest);
		dest = KeyValuePairToString(TIME_TO_END, serverState.GetTimeToEnd(), dest);
		dest = KeyValuePairToString(SERVER_ERROR, serverState.GetLastServerError(), dest);
		dest = KeyValuePairToString(WHITE_WINS, serverState.IsWhiteWins(), dest);
		dest = KeyValuePairToString(BLACK_WINS, serverState.IsBlackWins(), dest);
		dest = BoardToString(serverState.GetBoard(), 1, dest);

		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateRoleMessage(final ROLE_ENUM role, final String id, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, SET_ROLE, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(ROLE, role.getValue(), dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String CreateStopAnalyse(final String id, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, STOP_ANALYSE, dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static void CreateOkMessage(final String  id, String dest)
	{
	    InsertHeader(dest);
	    KeyValuePairToString(ACTION, OK, dest);
	    KeyValuePairToString(MESSAGE_ID, id, dest);
	    KeyValuePairToString(MESSAGE_END, 0, dest);
	    InsertLenMessageHeader(dest);
	}

	public static String CreateNotOkMessage(final String id, NOT_OK_REASON reason, String dest)
	{
		dest = InsertHeader(dest);
		dest = KeyValuePairToString(ACTION, NOT_OK, dest);
		dest = KeyValuePairToString(REASON, reason.getValue(), dest);
		dest = KeyValuePairToString(MESSAGE_ID, id, dest);
		dest = KeyValuePairToString(MESSAGE_END, 0, dest);
		dest = InsertLenMessageHeader(dest);
		
		return dest;
	}

	public static String GetNextKey(final String debug_key)
	{
	    if (debugMode)
	    {
	        return new String(debug_key);
	    } else
	    {
	        nextKey++;
	        return new String(String.valueOf(nextKey) + "_");
	    }
	}

	synchronized public static String CreateMessageId()
	{
	    return String.valueOf(++messageId);    
	}
	
	
    static int MaxMessageSize() { return 4048; }
    static int MaxMessageConnectionCloseSize() { return 4048; }
    static int MaxMessageConnectedSize() { return 4048; }
    static int MaxMessageTimeToSendResultToClientsSize() { return 4048; }
    static int MaxMessageTimeoutSize() { return 4048; }
    static int BufferSize() { return 4; }
	
    public enum ROLE_ENUM 
    { CLIENT(0), WORKER(1);
		private final int value;
	
	    private ROLE_ENUM(int value) {
	        this.value = value;
	    }
	   
		public int getValue() {
	
			return value;
		}
    };
    
    public enum NOT_OK_REASON 
    { BUSY(1); 
		private final int value;
		
	    private NOT_OK_REASON(int value) {
	        this.value = value;
	    }
	   
		public int getValue() {
	
			return value;
		}    
    };
    
    private static int nextKey = 0;
    private static boolean debugMode = true;
    private static long messageId = 0;
    
    public static String ACTION = GetNextKey("ACTION");
    public static String OK = GetNextKey("OK");
    public static String NOT_OK = GetNextKey("NOT_OK");
    public static String SERVER_STATE = GetNextKey("SERVER_STATE");
    public static String MESSAGE_ID = GetNextKey("MESSAGE_ID");
    public static String MESSAGE_END = GetNextKey("MESSAGE_END");
    public static String CLOSE_CNNECTION = GetNextKey("CLOSE_CNNECTION");
    public static String CONNECTED = GetNextKey("CONNECTED");
    public static String START_ANALYSE = GetNextKey("START_ANALYSE");
    public static String START_ANALYSE_FAST = GetNextKey("START_ANALYSE_FAST");
    public static String TIMEOUT = GetNextKey("TIMEOUT");
    public static String TIME_TO_SEND_RESULT_TO_CLIENTS = GetNextKey("TIME_TO_SEND_RESULT_TO_CLIENTS");
    public static String RESET_SERVER_STATE = GetNextKey("RESET_SERVER_STATE");
    public static String SERVER_ERROR = GetNextKey("SERVER_ERROR");
    public static String CAN_NOT_MOVE = GetNextKey("CAN_NOT_MOVE");
    public static String WHITE_WINS = GetNextKey("WHITE_WINS");
    public static String BLACK_WINS = GetNextKey("BLACK_WINS");

    public static String STOP_ANALYSE = GetNextKey("STOP_ANALYSE");
    public static String START_WORK = GetNextKey("START_WORK");
    public static String SET_STATE = GetNextKey("SET_STATE");
    public static String SET_ROLE = GetNextKey("SET_ROLE");
    public static String GET_SERVER_STATE  = GetNextKey("GET_SERVER_STATE");
    public static String BEST_RESULT = GetNextKey("BEST_RESULT");

    public static String IS_THINKING = GetNextKey("IS_THINKING");
    public static String MAX_IA_TIME = GetNextKey("MAX_IA_TIME");
    public static String START_TIME = GetNextKey("START_TIME");
    public static String TIME_TO_END = GetNextKey("TIME_TO_END");
    public static String IS_FIRST_WORKER = GetNextKey("IS_FIRST_WORKER");

    public static String REASON = GetNextKey("REASON");
    public static String MAX_TIME = GetNextKey("MAX_TIME");
    public static String NUM_OF_ANALYSED = GetNextKey("NUM_OF_ANALYSED");
    public static String NUM_OF_BOARD = GetNextKey("NUM_OF_BOARD");
    public static String STATE = GetNextKey("STATE");
    public static String ROLE = GetNextKey("STATE");
    public static String JOB_ID = GetNextKey("JOB_ID");
    public static String NUM_OF_THREAD = GetNextKey("NUM_OF_THREAD");

    public static String PREVIOUS_MURDER = GetNextKey("PREVIOUS_MURDER");
    public static String WHITE_PATCH_END = GetNextKey("WHITE_PATCH_END");
    public static String BLACK_WHITE = GetNextKey("BLACK_WHITE");

    public static String NUM_OF_BOARD_TO_RETURN_FAST = GetNextKey("NUM_OF_BOARD_TO_RETURN_FAST");

    public static String ORIGIN_NUMBER_BLACK = GetNextKey("ORIGIN_NUMBER_BLACK");
    public static String ORIGIN_NUMBER_WHITE = GetNextKey("ORIGIN_NUMBER_WHITE");
    public static String ORIGIN_WHITE_X = GetNextKey("ORIGIN_WHITE_X_");
    public static String ORIGIN_WHITE_Y = GetNextKey("ORIGIN_WHITE_Y_");
    public static String ORIGIN_BLACK_X = GetNextKey("ORIGIN_BLACK_X_");
    public static String ORIGIN_BLACK_Y = GetNextKey("ORIGIN_BLACK_Y_");
    public static String ORIGIN_WHITE_PONS = GetNextKey("ORIGIN_WHITE_PONS_");
    public static String ORIGIN_BLACK_PONS = GetNextKey("ORIGIN_BLACK_PONS_");

    public static String NUMBER_BLACK = GetNextKey("NUMBER_BLACK");
    public static String NUMBER_WHITE = GetNextKey("NUMBER_WHITE");
    public static String WHITE_X = GetNextKey("WHITE_X_");
    public static String WHITE_Y = GetNextKey("WHITE_Y_");
    public static String BLACK_X = GetNextKey("BLACK_X_");
    public static String BLACK_Y = GetNextKey("BLACK_Y_");
    public static String WHITE_PONS = GetNextKey("WHITE_PONS_");
    public static String BLACK_PONS = GetNextKey("BLACK_PONS_");
	    
}
