import pl.checkers.checkersarea.*;

public class CheckersClient 
{
	public static void main(String[] args)
	{														
		SwingConsole console = new SwingConsole();
		
		console.run(new MainWindow(), 1000, 800);
	}
}