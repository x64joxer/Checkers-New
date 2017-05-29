package pl.checkers.checkersarea;
import java.awt.Color;

import java.awt.Graphics;
import java.awt.Dimension;
import java.awt.event.MouseEvent;
import java.awt.event.MouseAdapter;
import pl.boards.*;
import pl.notify.NotifyClass;
import pl.programvariables.ProgramVariables;
import pl.serverstate.*;
import Trace.Traces;
import javax.swing.JPanel;

public class CheckersArea extends JPanel 
{	
	
	public void AddToStateChangeNotification(final NotifyClass notify)
	{
		if (ProgramVariables.GetTraceFlagForClass_ServerConnection()) Traces.Debug("LOG: CheckersArea: public void AddToStateChangeNotification(final NotifyClass notify)");
		
		notifyVariable.AddToNotifyList(notify);
	}
	
    public CheckersArea()
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public CheckersArea()");
    	
    	mouseHandler.SetRefToCheckersArea(this);    	       
        
        previousBoard.Copy(board);
        cursorState = CursorState.Free;
        
        setPreferredSize(new Dimension(450, 450));
        setBackground(Color.GREEN);
        this.addMouseListener(mouseHandler);
        this.addMouseMotionListener(mouseHandler);
    }    
   
    public CursorState GetCursorState()
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public CursorState GetCursorState()");
    	
    	return cursorState;
    }
    
    public ServerState GetServerState()
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public ServerState GetServerState()");
    	
    	return new ServerState(serverState);
    }
    
    public void paintComponent(Graphics g) 
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public void paintComponent(Graphics g)");
    	
        super.paintComponent(g);
        g.drawOval(0, 0, getWidth(), getHeight());
        PaintFields(g);
        PaintPawn(g);
        PaintGrabbedBlackPawn(g);
    }
    
    private void PaintFields(Graphics painter)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void PaintFields(Graphics painter)");
    	
        int widthField = getWidth() / 8;
        int heightField = getHeight() / 8;
        boolean flag = false;

        for (int y = 0;y<8;y++)
        {
            for (int x = 0;x<8;x++)
            {
                if (flag)
                {
                    painter.setColor(field1);
                } else
                {
                    painter.setColor(field2);
                };

                flag = !flag;


                painter.fillRect(x*widthField,
                                 y*heightField,
                                 (x*widthField)+widthField,
                                 (y*heightField)+heightField);            

            };
            flag = !flag;
        };
    }

    private void PaintPawn(Graphics painter)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void PaintPawn(Graphics painter)");
    	
        Board boardToPaint = new Board();        
        
        if (cursorState == CursorState.WaitForIA)
        {
            boardToPaint.Copy(board);
        } else
        {
            if (displayedBoard == 0)
            {
              boardToPaint.Copy(board);
            } else
            {
              boardToPaint.Copy(previousBoard);
            };
        };


        int numberOfWhite = boardToPaint.GetNumberOfWhite();
        int numberOfBlack = boardToPaint.GetNumberOfBlack();
        int widthField = getWidth() / 8;
        int heightField = getHeight() / 8;
        PawnPos pos;

        for (int i=0;i<numberOfWhite;i++)
        {
            pos = boardToPaint.GetWhitePawnPos(i);

            DrawPawn(painter,
                     (pos.X() * widthField) + (widthField /2),
                     (pos.Y() * heightField) + (heightField /2),
                     widthField,
                     heightField,
                     true,
                     boardToPaint.GetWhitePawnPons(i));

        };

        for (int i=0;i<numberOfBlack;i++)
        {
            pos = boardToPaint.GetBlackPawnPos(i);

            DrawPawn(painter,
                     (pos.X() * widthField) + (widthField /2),
                     (pos.Y() * heightField) + (heightField /2),
                     widthField,
                     heightField,
                     false,
                     boardToPaint.GetBlackPawnPons(i));

        };
    }
    
    private void DrawPawn(Graphics painter, final int x, final int y, final int widthField, final int heightField, final boolean blackWhite, final boolean pons)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void DrawPawn(Graphics painter, final int x, final int y, final int widthField, final int heightField, final boolean blackWhite, final boolean pons)");
    	
        if (blackWhite)
        {            
            painter.setColor(pawn1);
        } else
        {
           
            painter.setColor(pawn2);
            
        };

        if (!pons)
        {            
            painter.fillOval(x - (int) (widthField / 2),
            				y - (int) (heightField / 2),
                    		2 * (int) (widthField / 2),
                    		2 * (int) (heightField / 2));

            painter.fillOval(x - (int) (widthField / 2.7),
            				y - (int) (heightField / 2.7),
            				2 * (int) (widthField / 2.7),
            				2 * (int) (heightField / 2.7));
            
           painter.setColor(Color.BLACK); 
           
           painter.drawOval(x - (int) (widthField / 2),
   							y - (int) (heightField / 2),
   							2 * (int) (widthField / 2),
           					2 * (int) (heightField / 2));

           painter.drawOval(x - (int) (widthField / 2.7),
   							y - (int) (heightField / 2.7),
   							2 * (int) (widthField / 2.7),
   							2 * (int) (heightField / 2.7));                        
        } else
        {        	        
            painter.fillOval(x - (int) (widthField / 2.2), 
            				y - (int) (heightField / 2.2),
            				2 * (int) (widthField / 2.2),
            				2 * (int) (heightField / 2.2));

            painter.fillOval(x - (int) (widthField / 2.3), 
            				 y - (int) (heightField / 2.3),
                             2 * (int) (widthField / 2.3),
                             2 * (int) (heightField / 2.3));
            
            painter.setColor(Color.BLACK);
            
            painter.fillOval(x - (int) (widthField / 2.2), 
    						y - (int) (heightField / 2.2),
    						2 * (int) (widthField / 2.2),
    						2 * (int) (heightField / 2.2));

            painter.fillOval(x - (int) (widthField / 2.3), 
            				y - (int) (heightField / 2.3),
            				2 * (int) (widthField / 2.3),
            				2 * (int) (heightField / 2.3));            
        };
    }
    
    private void PaintGrabbedBlackPawn(Graphics painter)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void PaintGrabbedBlackPawn(Graphics painter)");
    	
        if (cursorState == CursorState.Grab)
        {
            int widthField = getWidth() / 8;
            int heightField = getHeight() / 8;

            DrawPawn(painter, mouseX, mouseY, widthField, heightField, false, board.GetBlackPawnPons(grabbed));
        };
    }
    
    private void TakeMouseClickEvent(MouseEvent e)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void TakeMouseClickEvent(MouseEvent e)");
    	
        int widthField = getWidth() / 8;
        int heightField = getHeight() / 8;

        int x = (e.getX() / widthField);
        int y = (e.getY() / heightField);

        if (cursorState == CursorState.Free)
        {
            if (!(serverState.IsBlackWins() | serverState.IsWhiteWins()))
            {
            	//TODO
            	//if (agentTCP->GecConnectionState() == TCPHandler::ConState::UPDATED)
                //{
                    if (board.IsBlackPawnOnPos(x,y))
                    {
                        grabbed = board.GetBlackPawnNumber(x,y);

                        if (possibleMoves.CanIGrab(grabbed, board))
                        {
                            cursorState = CursorState.Grab;                            
                        };
                    };
                //}
            }
        }
    }
    
    public void TakeMouseMoveEvent(MouseEvent e)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public void TakeMouseMoveEvent(MouseEvent e)");
    	
        mouseX = e.getX();
        mouseY = e.getY();

        if (cursorState == CursorState.Grab) repaint();

    }
    
    public void TakeMouseReleaseEvent(MouseEvent e)
    {   
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public void TakeMouseReleaseEvent(MouseEvent e)");
    	
        int widthField = getWidth() / 8;
        int heightField = getHeight() / 8;

        int x = (e.getX() / widthField);
        int y = (e.getY() / heightField);

        if (cursorState == CursorState.Grab)
        {                
            Traces.Debug("if (cursorState == Grab)");
            previousBoard.Copy(board);

            if (possibleMoves.CanIPutHere(grabbed, x, y, board))
            {
                int killed = 0;
                if (possibleMoves.IsKill(grabbed, x, y, board, killed))
                {
                    board.RemoveWhitePawn(killed);

                    board.SetBlackPawnPos(grabbed,x,y);
                    if (possibleMoves.CheckHitTopLeft(x,y,board) | possibleMoves.CheckHitTopRight(x,y,board))
                    {
                        Traces.Debug("After kill possibleMoves.CheckHitTopLeft(x,y,board) | possibleMoves.CheckHitTopRight(x,y,board) is true, cursorState = Free");
                        cursorState = CursorState.Free;
                    } else
                    if (board.GetBlackPawnPons(grabbed))
                    {
                        if (possibleMoves.CheckHitBottomLeft(x,y,board) | possibleMoves.CheckHitBottomRight(x,y,board))
                        {
                            Traces.Debug("cursorState = Free");
                            cursorState = CursorState.Free;
                        } //Error here
                        else
                        {
                            if (board.GetNumberOfWhite()>0)
                            {
                                Traces.Debug("cursorState = WaitForSerwerStateUpdate");                            
                                cursorState = CursorState.WaitForSerwerStateUpdate;
                                SendingJob("");
                                StartThinking();                            
                            } else cursorState = CursorState.Free;
                        };
                    }
                    else
                    {
                        if (board.GetNumberOfWhite()>0)
                        {
                            Traces.Debug("cursorState = WaitForSerwerStateUpdate");                        
                            cursorState = CursorState.WaitForSerwerStateUpdate;
                            SendingJob("");
                            StartThinking();                        
                        } else cursorState = CursorState.Free;
                    };
                } else
                {
                    if (board.GetNumberOfWhite()>0)
                    {
                        Traces.Debug("cursorState = WaitForSerwerStateUpdate");
                        board.SetBlackPawnPos(grabbed,x,y);                    
                        cursorState = CursorState.WaitForSerwerStateUpdate;
                        SendingJob("");
                        StartThinking();
                    } else cursorState = CursorState.Free;
                }
            } else
            {
                cursorState = CursorState.Free;
                Traces.Debug("cursorState = Free");
            };
        };    
        repaint();
    }
    
    private class MouseHandler extends MouseAdapter 
    {     
        public void mousePressed(MouseEvent e) 
        {
        	refsToCheckersArea.TakeMouseClickEvent(e);        	           
        }
        
        public void mouseReleased(MouseEvent e) 
        {
        	refsToCheckersArea.TakeMouseReleaseEvent(e);        	      
        }

        public void mouseDragged(MouseEvent e) 
        {
        	refsToCheckersArea.TakeMouseMoveEvent(e);        
        }        
        
        public void SetRefToCheckersArea(CheckersArea ref)
        {
        	refsToCheckersArea = ref;
        }
        
        private CheckersArea refsToCheckersArea;
    }
    
    private void StartThinking()
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void StartThinking()");
    	
    	
        //TODO
    }

    private void SendingJob(final String server)
    {
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: private void SendingJob(final String server)");
    	serverState.SetBoard(board);
    	notifyVariable.NotifyAll();
    	//TODO
    }
    
    public synchronized void SetSeverState(final ServerState state)
    {   
    	if (ProgramVariables.GetTraceFlagForClass_CheckerArea()) Traces.Debug("LOG: CheckerArea: public synchronized void SetSeverState(final ServerState state)");
    	
    	serverState.Copy(state);
    	board.Copy(serverState.GetBoard());    	    	    
    	repaint();
    }
    
    private Color field1 = Color.BLACK;
    private Color field2 = Color.WHITE;    
    private Color pawn1 = Color.RED;
    private Color pawn2 = Color.BLUE;
    
    private MouseHandler mouseHandler = new MouseHandler();    
    private int mouseX = 0;
    private int mouseY = 0;
    private int grabbed = 0;
    
    private Board board = new Board();
    
    private Board previousBoard = new Board();
    private int displayedBoard;
    private PossibleMoves possibleMoves = new PossibleMoves();
    private ServerState serverState = new ServerState();
    private volatile NotifyClass notifyVariable = new NotifyClass();
    public enum CursorState { Free, Grab, WaitForSerwerStateUpdate, WaitForIA };
    private volatile CursorState cursorState;
}
