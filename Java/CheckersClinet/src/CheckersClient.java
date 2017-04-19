import java.io.IOException;
import Trace.Traces;
import pl.checkers.checkersarea.*;

public class CheckersClient 
{
	public static void main(String[] args) throws IOException
	{	
		Traces.SetTraceFileNameMethod(Traces.TraceFileNameMode.OnlyThreadID);
		Traces.SetTraceFolder("trace/");
		SwingConsole console = new SwingConsole();
		
		console.run(new MainWindow(), 1000, 800);
	}
}