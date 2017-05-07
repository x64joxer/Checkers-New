package pl.checkers.checkersarea;
import java.io.IOException;
import javax.swing.JFrame;
import Trace.Traces;
import pl.tcp.*;

public class MainWindow extends JFrame 
{		
	public MainWindow() throws IOException
	{			
		Traces.Debug("Main Window Start!");
		
		add(checkersArea);
		checkersArea.setVisible(true);
		checkersArea.setSize(1000, 1000);
		
		ServerConnection connection = new ServerConnection("localhost", 6000);
		connection.Connect();
		
	}
	
	
	private CheckersArea checkersArea = new CheckersArea();	
}
