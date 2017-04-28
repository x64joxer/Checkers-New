package pl.tcp;

public class Peers 
{
	public enum  STATE 
	{ NONE(0), BUSY(1), FREE(2); 
	
		private final int value;
		
	    private STATE(int value) {
	        this.value = value;
	    }
	   
		public int getValue() {

			return value;
		}
	
	};
}
