package pl.tcp;

import pl.tcp.*;
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import Trace.Traces;
import pl.notify.NotifyClass;
import pl.programvariables.ProgramVariables;
import pl.serverstate.*;
import pl.boards.*;

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
	
	public void SetMaxRetries(final int ret)
	{
		maxRetries = ret;
	}
	
	public void AddToStateChangeNotification(final NotifyClass notify)
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: public void AddToStateChangeNotification(final NotifyClass notify)");
		
		notifyStateChanged.AddToNotifyList(notify);
	}
	
	public void Connect()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: public void Connect()");
		
		connectionState = ServerConnectionState.CONNECTING;
		notifyStateChanged.NotifyAll();
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
				case CONNECTING:
					ConnectToHost();
					break;
					
				case CONNECTED:
					SendRegisterMessage();
					break;
				
				case REGISTERSEND_WAIT_FOR_OK:
					SendRegisterMessageWaitForOkMessage();	
					break;
					
				case GETSTATESEND:
					SendGetStateToServer();	
					break;
					
				case GETSTATESEND_WAIT_FOR_STATE:
					WaitForStateFromServer();	
					break;					
						
				case STATEUPDATED:
					WaitForTask();	
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
			notifyStateChanged.NotifyAll();
			
		} else
		{
			Traces.Debug("ERR: ServerConnection: Can not connect to server!");
			
			if (Reconnect() == true)
			{
				try {
					Thread.sleep(recconectionTime);
				} catch (InterruptedException e) {

				}
			}
			
			
		}	
	}
	
	private void SendRegisterMessage()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void SendRegisterMessage()");
		
	    prevousMessageid = MessageCoder.CreateMessageId();
	    String message = "";
	    message = MessageCoder.CreateRoleMessage(MessageCoder.ROLE_ENUM.CLIENT, prevousMessageid, message);	    
	    serverClient.Send(message);
	    connectionState = ServerConnectionState.REGISTERSEND_WAIT_FOR_OK;
	    notifyStateChanged.NotifyAll();
	    
	}
	
	private void SendRegisterMessageWaitForOkMessage()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void SendRegisterMessageWaitForOkMessage()");
		
		try 
		{
			notifyVariable.Wait(ProgramVariables.GetMaxTimeWaitToServer());
			
			String message = serverClient.GetMessage();
			
			
			HashMap<String, String> receivedMessage = new HashMap<String, String>();
			
			MessageCoder.MessageToMap(message, receivedMessage);
						
			if (receivedMessage.get(MessageCoder.ACTION).equals(MessageCoder.OK) == true)
			{
				if (prevousMessageid.equals(receivedMessage.get(MessageCoder.MESSAGE_ID)) == true)
				{
					if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Message OK recived.");
					
					connectionState = ServerConnectionState.GETSTATESEND;
					notifyStateChanged.NotifyAll();
				} else
				{
					Traces.Debug("ERR: ServerConnection: Wrong message ID!");
					
					if (Reconnect() == true)
					{
						try {
							Thread.sleep(recconectionTime);
						} catch (InterruptedException e) {

						}					
					}						
				}
				
			} else
			{
				Traces.Debug("ERR: ServerConnection: Wrong message received! Expected OK message! Received message: " + message);
				
				if (Reconnect() == true)
				{
					try {
						Thread.sleep(recconectionTime);
					} catch (InterruptedException e) {

					}					
				}												
			}			
			
		}
		catch(Exception e)
		{
			if (e.getMessage() == "Timeout!")
			{									
				Traces.Debug("ERR: ServerConnection: Tiemout! Messge OK not received after sending register message!");
				
				Reconnect();								
				
			}
		}		
	}
	
	private void SendGetStateToServer()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void SendGetStateToServer()");

	    prevousMessageid = MessageCoder.CreateMessageId();
	    String message = "";
	    message = MessageCoder.CreateGetServerStateMessage(prevousMessageid, message);
	    serverClient.Send(message);
	    connectionState = ServerConnectionState.GETSTATESEND_WAIT_FOR_STATE;
	    notifyStateChanged.NotifyAll();
	}

	private void WaitForStateFromServer()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: WaitForStateFromServer()");

		try 
		{
			notifyVariable.Wait(ProgramVariables.GetMaxTimeWaitToServer());
			
			String message = serverClient.GetMessage();
			
			
			HashMap<String, String> receivedMessage = new HashMap<String, String>();
			
			MessageCoder.MessageToMap(message, receivedMessage);
						
			if (receivedMessage.get(MessageCoder.ACTION).equals(MessageCoder.SERVER_STATE) == true)
			{
				if (prevousMessageid.equals(receivedMessage.get(MessageCoder.MESSAGE_ID)) == true)
				{
					if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Server state recived.");
					
					Board reveiwedBoard = new Board();
					MessageCoder.MapToBoard(receivedMessage, reveiwedBoard);
					
					currentServerState.SetBoard(reveiwedBoard);					
					
					if (receivedMessage.get(MessageCoder.IS_THINKING).equals("1") == true)
					{
						currentServerState.SetThinking(true);
					} else
					{
						currentServerState.SetThinking(false);	
					}
					
					currentServerState.SetStartTime(Long.parseLong(receivedMessage.get(MessageCoder.START_TIME)));
					currentServerState.SetMaxTime(Long.parseLong(receivedMessage.get(MessageCoder.MAX_IA_TIME)));
					currentServerState.SetTimeToEnd(Long.parseLong(receivedMessage.get(MessageCoder.TIME_TO_END)));
					currentServerState.SetlastServerError(receivedMessage.get(MessageCoder.SERVER_ERROR));
				
					if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Received board:");
					if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) reveiwedBoard.PrintDebug();
					
					if (receivedMessage.get(MessageCoder.WHITE_WINS).equals("1") == true)
					{
						currentServerState.SetWhiteWins();
					} else
					{
						currentServerState.SetBlackWins();
					}
					
					connectionState = ServerConnectionState.STATEUPDATED;
					notifyStateChanged.NotifyAll();
									
				} else
				{
					Traces.Debug("ERR: ServerConnection: Wrong message ID!");
					
					if (Reconnect() == true)
					{
						try {
							Thread.sleep(recconectionTime);
						} catch (InterruptedException e) {

						}					
					}						
				}
				
			} else
			{
				Traces.Debug("ERR: ServerConnection: Wrong message received! Expected OK message! Received message: " + message);
				
				if (Reconnect() == true)
				{
					try {
						Thread.sleep(recconectionTime);
					} catch (InterruptedException e) {

					}					
				}												
			}			
			
		}
		catch(Exception e)
		{
			if (e.getMessage() == "Timeout!")
			{									
				Traces.Debug("ERR: ServerConnection: Tiemout! Messge OK not received after sending register message!");
				
				Reconnect();								
				
			}
		}			    
	}
	
	private void WaitForTask()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private void WaitForTask()");
		
		notifyVariable.Wait();
	}
	
	private boolean Reconnect()
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: private boolean Reconnect()");
		
		if (maxRetries != 0)
		{
			if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Reconnecting...");
			
			connectionState = ServerConnectionState.CONNECTING;
			notifyStateChanged.NotifyAll();
			if (maxRetries > 0) maxRetries--;			
			serverClient.Close();
			
			return true;
		} else
		{
			if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: ServerConnection: Stop connecting!");
			
			connectionState = ServerConnectionState.CONCTERROR;
			notifyStateChanged.NotifyAll();
			continueLoop = false;
			return false;
		}
	}
	
	private TCPClient serverClient = new TCPClient();
	private int port = 0;
	private int maxRetries = -1; //-1 - unlimited
	private int recconectionTime = 5000;
	private String prevousMessageid = "";
	private String ipAddres = "";
	private volatile ServerConnectionState connectionState = ServerConnectionState.IDLE;
	private volatile NotifyClass notifyVariable = new NotifyClass();
	private volatile NotifyClass notifyStateChanged = new NotifyClass();
	boolean continueLoop = true;	
	
	private volatile ServerState currentServerState = new ServerState();
	
	public enum  ServerConnectionState 
	{ IDLE(0), 
	  CONNECTING(1),
	  CONNECTED(2),
	  REGISTERSEND_WAIT_FOR_OK(3),
	  GETSTATESEND(4),
	  GETSTATESEND_WAIT_FOR_STATE(4),
	  STATEUPDATED(5),
	  CONCTERROR(255); 
	
		private final int value;
		
	    private ServerConnectionState(int value) {
	        this.value = value;
	    }
	   
		public int getValue() {

			return value;
		}
	
	};
}
