package pl.serverstate;

import pl.boards.*;
import Trace.Traces;
import pl.programvariables.*;

public class ServerState 
{
	public ServerState() 
	{
						
		current = new Board(
		"| |w| |w| |w| |w|" +
		"|w| |w| |w| |w| |" +
		"| |w| |w| |w| |w|" +
		"| | | | | | | | |" +
		"| | | | | | | | |" +
		"|b| |b| |b| |b| |" +
		"| |b| |b| |b| |b|" +
		"|b| |b| |b| |b| |");
				
	}
	
	public ServerState(final Board tmpCurrent,
            final boolean tmpThinking,
            final long tmpStartTime,
            final long tmpMaxTime,
            final long tmpTimeToEnd,
            final String serverError,
            boolean whitwins,
            boolean blackwins) 
	{		
            current = tmpCurrent;
            thinking = tmpThinking;
            startTime = tmpStartTime;
            maxTime = tmpMaxTime;
            timeToEnd = tmpTimeToEnd;
            lastServerError = serverError;
            whiteWins = whitwins;
            blackWins = blackwins;
	}
	
	public ServerState(final ServerState data)
	{	    
	    if (ProgramVariables.GetTraceFlagForClass_ServerState()) Traces.Debug("LOG: final ServerState ServerState::operator=(final ServerState  & data)");
	
	    current.Copy(data.current);
	    thinking = data.thinking;
	    startTime = data.startTime;
	    maxTime = data.maxTime;
	    timeToEnd = data.timeToEnd;
	    lastServerError = data.lastServerError;
	    whiteWins = data.whiteWins;
	    blackWins = data.blackWins;

	}
	
	public void Copy(final ServerState data)
	{
	    if (ProgramVariables.GetTraceFlagForClass_ServerState()) Traces.Debug("LOG: final ServerState ServerState::operator=(final ServerState  & data)");
	    	    
	    current.Copy(data.GetBoard());	    	   
	    thinking = data.thinking;
	    startTime = data.startTime;
	    maxTime = data.maxTime;
	    timeToEnd = data.timeToEnd;
	    lastServerError = data.lastServerError;
	    whiteWins = data.whiteWins;
	    blackWins = data.blackWins;
	}
	
    public synchronized void SetBoard(final Board  board) {  current.Copy(board); }
    public synchronized Board GetBoard() {  return new Board(current); }
    public synchronized void SetPreviousBoard(final Board  board) {  previousBoard = board; }
    public synchronized Board GetPrviousBoard() {  return previousBoard; }
    public synchronized void SetThinking(final boolean flag) {  thinking = flag; }
    public synchronized boolean IsThinking() {  return thinking; }
    public synchronized void SetStartTime(final long val) {  startTime = val; }
    public synchronized void SetMaxTime(final long val) {  maxTime = val; }
    public synchronized void SetMaxTimeForWorkers(final long val) {  maxTimeForWorkers = val; }
    public synchronized void SetTimeToEnd(final long val) {  timeToEnd = val; }
    public synchronized long GetStartTime() {  return startTime; }
    public synchronized long GetMaxTime() { return maxTime; }
    public synchronized long GetMaxTimeForWorkers() { return maxTimeForWorkers; }
    public synchronized long GetTimeToEnd() { return startTime; }
    public synchronized void SetlastServerError(final String error) { lastServerError = error; }
    public synchronized String GetLastServerError() { return lastServerError; }
    public synchronized void SetWhiteWins() { whiteWins = true; blackWins = false; }
    public synchronized void SetBlackWins() { whiteWins = false; blackWins = true; }
    public synchronized boolean IsWhiteWins() { return whiteWins; }
    public synchronized boolean IsBlackWins() { return blackWins; }
    public synchronized void ClearWins() { whiteWins = false; blackWins =false; }
	
	public static String NO_SERVER_ERROR_TEXT = new String("NO_ERROR");
    private Board current;
    private Board previousBoard = new Board();
    private boolean thinking = false;
    private long startTime = 0;
    private long maxTime = 0;
    private long maxTimeForWorkers = 0;
    private long timeToEnd = 0;
    private boolean whiteWins = false;
    private boolean blackWins = false;
    private String lastServerError = ServerState.NO_SERVER_ERROR_TEXT;
     
}
