package pl.checkers.checkersarea;
import javax.swing.JFrame;

public class MainWindow extends JFrame 
{		
	public MainWindow()
	{			
		add(checkersArea);
		checkersArea.setVisible(true);
		checkersArea.setSize(1000, 1000);
	}
	
	
	private CheckersArea checkersArea = new CheckersArea();	
}
