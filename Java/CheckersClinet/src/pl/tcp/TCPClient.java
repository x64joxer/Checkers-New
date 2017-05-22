package pl.tcp;

import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.List;
import Trace.Traces;
import pl.notify.NotifyClass;
import pl.programvariables.ProgramVariables;

public class TCPClient implements Runnable 
{
	public TCPClient() 
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient public TCPClient()");
	}

	public TCPClient(final String ip, final int tcpPort) 
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient public TCPClient(final String ip, final int tcpPort)");
		
		port = tcpPort;
		ipAddres = ip;
	}
	
	synchronized public void AddNotification(final NotifyClass n)
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient synchronized public void AddNotification(final NotifyClass n)");
		
		mynotify.AddToNotifyList(n);
	}
	
	synchronized public void DelNotification(final NotifyClass n)
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient synchronized public void DelNotification(final NotifyClass n)");
		
		mynotify.DelFromNotifyList(n);
	}
	
	synchronized public void Close()
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient synchronized public void Close()");
		
		try 
		{
			connected = ConnectionState.DISCONNECTED;
			clientSocket.close();
		} catch (IOException e) {
			Traces.Debug("ERR: Close socket error!");
			e.printStackTrace();
		}
		
	}
	
	public ConnectionState GetConnectionState()
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient private ConnectionState GetConnectionState()");
		
		return connected;
	}
	
	private boolean IntToBoolean(final int val)
	{
		if (val == 0) return false;
		return true;
	}
	
	synchronized public String GetMessage()
	{	
		String firstMessage = "";
		
		try
		{
			firstMessage = incomingMessageList.remove(0);
		} 
		catch(IndexOutOfBoundsException e)
		{
			Traces.Debug("ERR: No message in message queue!");
		}
		
		return firstMessage;
	}
	
	public void Connect() 
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient public void Connect()");
		
		new Thread(this).start();
	}	
	
	public boolean IsConnected()
	{				
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient public boolean IsConnected()");
		
		if (connected == ConnectionState.CONNECTED) return true;
		
		return false;
	}
	
	synchronized public void Send(final String message)
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient synchronized public void Send(final String message)");
		
		try 
		{
			if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: Sending message: " + message);
			outToServer.writeBytes(message);			
		} catch (IOException e) 
		{
			Traces.Debug("ERROR: Write message error! " +  e.getMessage());
			
			mynotify.NotifyAll();
			
		}		
	}
	
	private void connect()
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient private void connect()");
		
		//TODO can be removed?
		BufferedReader inFromUser = new BufferedReader( new InputStreamReader(System.in));
		
		try 
		{			
			clientSocket = new Socket(ipAddres, port);
			outToServer = new DataOutputStream(clientSocket.getOutputStream());			
			inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			
			if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient Socket connected!");
			connected = ConnectionState.CONNECTED;
			mynotify.NotifyAll();
			
			String message = new String();
			
			while(true)
			{
				if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient Waiting for message...");				
								
				message = inFromServer.readLine();				
				
				if (message == null) 
				{	
					connected = ConnectionState.DISCONNECTED;		
					Traces.Debug("ERR: Read message error! Message is null! Socket closed!");
					mynotify.NotifyAll();
					break;
				
				} else
				{					
					if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient Message receivd: " + message);
					incomingMessageList.add(message);
					mynotify.NotifyAll();
				}
				
			}			
			
		} 
		catch (IOException e) 
		{			
			connected = ConnectionState.CONERROR;
			Traces.Debug("ERR: Read message error! " + e.getMessage()); 
			mynotify.NotifyAll();							
		}		
	}
	
	public void run() 
	{
		if (ProgramVariables.GetTraceFlagForClass_TCPClient()) Traces.Debug("LOG: TCPClient public void run()");
		
		connect();
	}
	
	private int port = 0;
	private String ipAddres = "";
	private volatile ConnectionState connected = ConnectionState.NOTCONNECTED;
	private Socket clientSocket = new Socket();
	private DataOutputStream outToServer;
	private BufferedReader inFromServer;
	private volatile NotifyClass mynotify = new NotifyClass();
	private List<String> incomingMessageList = new ArrayList<String>();
	
	public enum  ConnectionState 
	{ NOTCONNECTED(0), CONNECTED(1), DISCONNECTED(2), CONERROR(3); 
	
		private final int value;
		
	    private ConnectionState(int value) 
	    {
	        this.value = value;
	    }
	   
		public int getValue() {

			return value;
		}
	
	};
}
