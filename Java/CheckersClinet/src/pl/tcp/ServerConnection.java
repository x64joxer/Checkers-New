package pl.tcp;

import pl.tcp.*;
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;
import Trace.Traces;
import pl.notify.NotifyClass;
import pl.programvariables.ProgramVariables;

public class ServerConnection implements Runnable
{
	public ServerConnection()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: public ServerConnection()");
	}

	public ServerConnection(final String ip, final int tcpPort)
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: public ServerConnection(final String ip, final int tcpPort)");
		
		port = tcpPort;
		ipAddres = ip;	
	}
	
	public void Connect()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: public void Connect()");
		
		connect();
	}

	private void connect()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void connect()");
		
		new Thread(this).start();

	}
	
	public void run() 
	{	
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: public void run()");
		
		serverClient = new TCPClient(ipAddres, port);
		
		continueLoop = true;
		
		while(continueLoop)
		{
			switch (connectionState)
			{
				case NOTCONNECTED:
					ConnectToHost();
					
				case CONNECTED:
					SendRegisterMessage();
				
				case REGISTERSEND_WAIT_FOR_OK:
					SendRegisterMessageWaitForOkMessage();	

				break;
				
				default:
					Traces.Debug("ERR: ServerConnection: Wrong connection state!");
					continueLoop = false;
				
			} 
			
		}
	}
	
	private void ConnectToHost()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void ConnectToHost()");
		
		serverClient = new TCPClient(ipAddres, port);
		serverClient.AddNotification(notifyVariable);
		serverClient.Connect();
		notifyVariable.Wait();
		
		if (serverClient.IsConnected())
		{
			if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Connected to server!");
			connectionState = ServerConnectionState.CONNECTED;
			
		} else
		{
			Traces.Debug("ERR: ServerConnection: Can not connect to server!");
			connectionState = ServerConnectionState.CONCTERROR;
			continueLoop = false;			
		}	
	}
	
	private void SendRegisterMessage()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void SendRegisterMessage()");
		
	    String prevousMessageid = MessageCoder.CreateMessageId();
	    String message = "";
	    message = MessageCoder.CreateRoleMessage(MessageCoder.ROLE_ENUM.CLIENT, prevousMessageid, message);	    
	    serverClient.Send(message + "\n");
	    connectionState = ServerConnectionState.REGISTERSEND_WAIT_FOR_OK;
	    
	}
	
	private void SendRegisterMessageWaitForOkMessage()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void SendRegisterMessageWaitForOkMessage()");
		
		try 
		{
			notifyVariable.Wait(ProgramVariables.GetMaxTimeWaitToServer());
			
			
			if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Message OK recived.");
			
			connectionState = ServerConnectionState.STATESEND;
			
		}
		catch(Exception e)
		{
			if (e.getMessage() == "Timeout!")
			{
				connectionState = ServerConnectionState.CONCTERROR;			
				serverClient.Close();
				continueLoop = false;
				
				Traces.Debug("ERR: ServerConnection: Tiemout! Messge OK not received after sending register message!");
			}
		}		
	}
	
	private TCPClient serverClient = new TCPClient();
	private int port = 0;
	private String ipAddres = "";
	private ServerConnectionState connectionState = ServerConnectionState.NOTCONNECTED;
	private volatile NotifyClass notifyVariable = new NotifyClass();	
	boolean continueLoop = true;
	
	public enum  ServerConnectionState 
	{ NOTCONNECTED(0), CONNECTED(1), REGISTERSEND_WAIT_FOR_OK(2), STATESEND(3), CONCTERROR(255); 
	
		private final int value;
		
	    private ServerConnectionState(int value) {
	        this.value = value;
	    }
	   
		public int getValue() {

			return value;
		}
	
	};
}
