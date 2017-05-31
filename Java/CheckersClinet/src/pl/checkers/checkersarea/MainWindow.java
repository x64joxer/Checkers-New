package pl.checkers.checkersarea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;

import Trace.Traces;
import pl.tcp.*;

public class MainWindow extends JFrame implements ActionListener 
{		
	public MainWindow() throws IOException
	{			
		Traces.Debug("Main Window Start!");
				
        JMenu jmFile = new JMenu("Game");
        JMenuItem jmiNew = new JMenuItem("New");        
        jmFile.add(jmiNew);
        jmb.add(jmFile);
        setJMenuBar(jmb);
        jmiNew.addActionListener(this);

		add(checkersArea);
		checkersArea.setVisible(true);
		checkersArea.setSize(1000, 1000);
		
		ServerConnection connection = new ServerConnection("localhost", 6000);
		synchronizer.SetServerConnection(connection);
		synchronizer.SetCheckerArea(checkersArea);
		synchronizer.Start();
		
		connection.Connect();
		
	}
	
	 public void actionPerformed(ActionEvent ae) 
	 {
		    String comStr = ae.getActionCommand();		    
		    
		    if (comStr == "New")
		    {
		    	synchronizer.ResetServerState();
		    }
     }
	private CheckersArea checkersArea = new CheckersArea();
	private Synchronizer synchronizer = new Synchronizer();
	private JMenuBar jmb = new JMenuBar();
}
