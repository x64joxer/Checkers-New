package pl.checkers.checkersarea;

import pl.notify.NotifyClass;
import pl.programvariables.ProgramVariables;
import Trace.Traces;
import pl.tcp.*;

public class Synchronizer implements Runnable
{

	public void SetServerConnection(final ServerConnection serCon)
	{
		if (ProgramVariables.GetTraceFlagForClass_Synchronizer()) Traces.Debug("LOG: Synchronizer: public void SetServerConnection(final ServerConnection serCon)");
		
		currentServerConnection = serCon;
		currentServerConnection.AddToStateChangeNotification(notifyVariable);
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
			
			notifyVariable.Wait();
		}
	}

	private volatile NotifyClass notifyVariable = new NotifyClass();	
	private volatile boolean continueLoop = true;
	private Thread mainThread;
	private volatile ServerConnection currentServerConnection = new ServerConnection();
}
