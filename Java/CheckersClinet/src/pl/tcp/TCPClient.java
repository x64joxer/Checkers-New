package pl.tcp;

import java.io.*;
import java.net.*;

public class TCPClient 
{
	public TCPClient() 
	{

	}
	
	public void Connect() 
	{
		BufferedReader inFromUser = new BufferedReader( new InputStreamReader(System.in));
		try {
			Socket clientSocket = new Socket("localhost", 6000);
			DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
			BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			System.out.print("start");
			System.out.print(inFromServer.read());
			System.out.print("stop");
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			System.out.print(e.getMessage());
			e.printStackTrace();
		}
	}
}
