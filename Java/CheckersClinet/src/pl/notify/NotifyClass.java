package pl.notify;

import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;
import Trace.Traces;
import pl.programvariables.*;

public class NotifyClass 
{

	public synchronized void Wait()
	{
		if (ProgramVariables.GetTraceFlagForClass_NotifyClass()) Traces.Debug("public synchronized void Wait()"); 
		
		try 
		{
			wait();
		} 
		catch (InterruptedException e) 
		{
			e.printStackTrace();
		}
	}
	
	public synchronized void AddToNotifyList(NotifyClass val)
	{
		if (ProgramVariables.GetTraceFlagForClass_NotifyClass()) Traces.Debug("public synchronized void AddToNotifyList(NotifyClass val)");
		
		list.add(val);
	}
	
	public synchronized void DelFromNotifyList(NotifyClass val)
	{
		if (ProgramVariables.GetTraceFlagForClass_NotifyClass()) Traces.Debug("public synchronized void DelFromNotifyList(NotifyClass val)");
		
		list.remove(val);
	}

	public synchronized void NotifyMe()
	{
		if (ProgramVariables.GetTraceFlagForClass_NotifyClass()) Traces.Debug("public synchronized void NotifyMe()");
		
		notifyAll();
	}
	
	public synchronized void NotifyAll()
	{
		if (ProgramVariables.GetTraceFlagForClass_NotifyClass()) Traces.Debug("public synchronized void NotifyAll()");
		
		for (Iterator<NotifyClass> i = list.iterator(); i.hasNext();) 
		{
			NotifyClass item = i.next();
			item.NotifyMe();		    
		}
	}
	
	private List<NotifyClass> list = new ArrayList<NotifyClass>();
}
