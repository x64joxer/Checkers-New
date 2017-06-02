package pl.checkers.checkersarea;

import pl.boards.Board;
import pl.notify.NotifyClass;
import pl.programvariables.ProgramVariables;
import Trace.Traces;
import pl.serverstate.ServerState;
import pl.tcp.*;

public class Synchronizer implements Runnable
{

	public void ResetServerState()
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void ResetServerState()");
		
		currentServerConnection.ResetServerState();
	}
	
	public void SetServerConnection(final ServerConnection serCon)
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void SetServerConnection(final ServerConnection serCon)");
		
		currentServerConnection = serCon;
		currentServerConnection.AddToStateChangeNotification(notifyVariable);		
	}

	public void SetCheckerArea(final CheckersArea checArea)
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void SetCheckerArea(final CheckersArea checArea)");
		
		currentCheckersArea = checArea;
		currentCheckersArea.AddToStateChangeNotification(notifyVariable);
	}
	
	public void Start()
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void Start()");
		
		mainThread = new Thread(this);
		mainThread.start();
	}

	public void Stop()
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void Stop()");
		
		continueLoop = false;
		notifyVariable.NotifyMe();
		
		try 
		{
			mainThread.join();
		} catch (InterruptedException e) 
		{
			//TODO Print something?	
			e.printStackTrace();
		}
	}
	
	public void run() 
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void run()");
		
		while(continueLoop == true)
		{
			if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: Wait for state change...");
			
			switch (currentServerConnection.GetConnectionState())
			{
				case CONNECTING:
					
					break;
					
				case CONNECTED:
					
					break;
				
				case REGISTERSEND_WAIT_FOR_OK:
						
					break;
					
				case GETSTATESEND:
						
					break;
					
				case GETSTATESEND_WAIT_FOR_STATE:
						
					break;					
						
				case STATEUPDATED:
					if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: STATEUPDATED");
					
					currentCheckersArea.SetSeverState(currentServerConnection.GetServerState());	
					break;
					
				default:
					Traces.Debug("ERR: Synchronizer: Wrong connection state!");					
				
			} 			
			
			notifyVariable.Wait();			
						
			if (currentCheckersArea.GetCursorState() == CheckersArea.CursorState.WaitForSerwerStateUpdate)
			{
				if (jobSend == false)
				{
					if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: Send job!");
					
					currentServerConnection.SendJobToServer(currentCheckersArea.GetServerState());
					jobSend = true;
					//TODO add catch exception when server will be in wrong state
				}
			}
		}
	}

	private volatile NotifyClass notifyVariable = new NotifyClass();	
	private volatile boolean continueLoop = true;
	private volatile boolean jobSend = false;
	private Thread mainThread;
	private volatile ServerConnection currentServerConnection = new ServerConnection();
	private volatile CheckersArea currentCheckersArea = new CheckersArea();
}
