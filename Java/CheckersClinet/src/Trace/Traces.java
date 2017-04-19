package Trace;

import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.util.HashMap;
import java.util.HashSet;
import java.io.*;
import java.util.Map;
import java.util.Set;

public class Traces 
{
	public static void Debug(String text) throws IOException
	{
		mutex.lock();
		StringToFile(text);
		mutex.unlock();
	}	
	
	public static void SetTraceFolder(final String dir)
	{
	    traceFolder = dir;
	    CreateTraceFolder();
	}

	public static void SetTraceFileNameMethod(final Traces.TraceFileNameMode mode)
	{
	    fileNameModeForTrace = mode;
	}

	public static String CreateStringWithSpaces(final String text, final int numOfSpaces)
	{
	    int tmpNumOfSpaces = numOfSpaces - text.length(); 
	    if (tmpNumOfSpaces < 0) tmpNumOfSpaces = 0;
	    String spaces = "";

	    for(int i = 0;i < tmpNumOfSpaces;++i)
	    {
	        spaces += " ";
	    }

	    return text + spaces;
	}

	public static String CreateStringWithSpaces(final long val, final int numOfSpaces)
	{
	    String text = String.valueOf(val);
	    int tmpNumOfSpaces = numOfSpaces - text.length();
	    if (tmpNumOfSpaces < 0) tmpNumOfSpaces = 0;
	    String spaces = "";

	    for (int i = 0;i < tmpNumOfSpaces;++i)
	    {
	        spaces += " ";
	    }

	    return text + spaces;
	}

	public static String CreateStringWithSpaces(final int val, final int numOfSpaces)
	{
	    String text = String.valueOf(val);
	    int tmpNumOfSpaces = numOfSpaces - text.length();
	    if (tmpNumOfSpaces < 0) tmpNumOfSpaces = 0;
	    String spaces = "";

	    for (int i = 0;i < tmpNumOfSpaces;++i)
	    {
	        spaces += " ";
	    }

	    return text + spaces;
	}

	public static String CreateStringWithSpaces(final float val, final int numOfSpaces)
	{
	    String text = String.valueOf(val);
	    int tmpNumOfSpaces = numOfSpaces - text.length();
	    if (tmpNumOfSpaces < 0) tmpNumOfSpaces = 0;
	    String spaces = "";
	    for (int i = 0;i < tmpNumOfSpaces;++i)
	    {
	        spaces += " ";
	    }

	    return text + spaces;
	}

	public static long GetMilisecondsSinceEpoch()
	{    
	    return System.currentTimeMillis();
	}

	public static long GetCurrentDateLL()
	{	    
		return System.currentTimeMillis();
	}


	public static void TurnOnTraces()
	{
	    traceOn = true;
	}

	public static void TurnOffTraces()
	{
	    traceOn = false;
	}

	public static boolean GetTraceFlag()
	{
	    return traceOn;
	}

	public static void RemoveThreadID()
	{	    	   
	    mutex.lock();
	    idText.remove(GetThreadId());
	    theardsId.remove(GetThreadId());
	    mutex.unlock();
	}
	
	public static String GetCurrentDate()
	{		
	    String result = "";
	    long milliseconds_since_epoch = System.currentTimeMillis();
	    long counter = 1000000000;
	    
	    while (counter != 1)
	    {
	        result+= String.valueOf(milliseconds_since_epoch / counter);
	        
	        result+=",";
	        milliseconds_since_epoch = milliseconds_since_epoch % counter;
	        counter = counter / 1000;
	        
	    };
	    result+= String.valueOf(milliseconds_since_epoch);	    
	    return result;
	}

	
    private static void StringToFile(String log) throws IOException
    {   	
    	FileWriter fileWriter = new FileWriter(new StringBuilder(traceFolder).append("/").append(GetThreadText()).toString(), true);
    	
    	String final_log = GetThreadText() + "[" + GetCurrentDate() + "]" + log + String.format("%n");
    	BufferedWriter bufWriter = new BufferedWriter(fileWriter);
    	bufWriter.write(final_log);   
    	bufWriter.close();
    }               

    private static void CreateTraceFolder()
    {
    	File dir = new File(traceFolder);
    	dir.delete();
    	dir.mkdir();    	
    }
    
    private static Long GetThreadId()
    {
    	return Thread.currentThread().getId();
    }
    
    private static Boolean IsOnTheList()
    {
        if (theardsId.isEmpty()) return false;
        return theardsId.containsKey(GetThreadId());        
    }
    
    private static String GetThreadText()
    {
        if (fileNameModeForTrace == TraceFileNameMode.PrefixThreadAndNumberOfthread)
        {
            if (IsOnTheList())
            {
               return theardsId.get(GetThreadId());
            };
        } else
        if (fileNameModeForTrace == TraceFileNameMode.OnlyThreadID)
        {                        
            return String.valueOf(GetThreadId());
        }

        return CreateNewThreadText();
    }
    
    private static String CreateNewThreadText()
    {
        String result = "";

        if (theardsId.isEmpty())
        {
          result = GenerateText(1);
          theardsId.put(GetThreadId(), result);          
          idText.add(result);
        } else
        {
            result = FindFirstFreeId();
            theardsId.put(GetThreadId(), result);          
            idText.add(result);
        };

        return result;
    }
    
    private static String FindFirstFreeId()
    {
        int id = 1;
        String search = GenerateText(id);        

        while(idText.isEmpty())
        {   
            for(String aSiteId: idText) 
            {
                if (aSiteId == search)
                {
                	search = "";
                	break;
                }
            }
            
            if (search == "")
            {
            	id++;
            	search = GenerateText(id);
            } else
            {
            	break;
            }            
        };
        
        return GenerateText(id);               
    }
    
    private static String GenerateText(int i)
    {
    	return "thread_" + String.valueOf(i);
    }
	
	public enum TraceFileNameMode { OnlyThreadID, PrefixThreadAndNumberOfthread }	
		
	private static boolean traceOn;
	private static Lock mutex = new ReentrantLock(); 
	private static HashMap<Long, String> theardsId = new HashMap<Long, String>(); 
	private static Set<String> idText = new HashSet<String>();	
	private static String traceFolder;   	
	private static TraceFileNameMode fileNameModeForTrace;
}

