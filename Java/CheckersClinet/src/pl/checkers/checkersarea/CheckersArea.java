package pl.checkers.checkersarea;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Dimension;
import java.awt.event.MouseEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import pl.boards.*;

import javax.swing.JPanel;

public class CheckersArea extends JPanel 
{
	private MouseHandler mouseHandler = new MouseHandler();
	
    public CheckersArea()
    {
        board = new Board("| |w| |w| |w| |w|" +
        "|w| |w| |w| |w| |" +
        "| |w| |w| |w| |w|" +
        "| | | | | | | | |" +
        "| | | | | | | | |" +
        "|b| |b| |b| |b| |" +
        "| |b| |b| |b| |b|" +
        "|b| |b| |b| |b| |");
        
        previousBoard = board;
        cursorState = CursorState.Free;
        
        setPreferredSize(new Dimension(450, 450));
        setBackground(Color.GREEN);
        this.addMouseListener(mouseHandler);
        this.addMouseMotionListener(mouseHandler);
    }    
    
    public void paintComponent(Graphics g) 
    {
        super.paintComponent(g);
        g.drawOval(0, 0, getWidth(), getHeight());
        PaintFields(g);
        PaintPawn(g);
    }
    
    private void PaintFields(Graphics painter)
    {
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

    void PaintPawn(Graphics painter)
    {
        Board boardToPaint;

        if (cursorState == CursorState.WaitForIA)
        {
            boardToPaint = board;
        } else
        {
            if (displayedBoard == 0)
            {
              boardToPaint = new Board(board);
            } else
            {
              boardToPaint = new Board(previousBoard);
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
    
    void DrawPawn(Graphics painter, final int x, final int y, final int widthField, final int heightField, final boolean blackWhite, final boolean pons)
    {
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
    
    private class MouseHandler extends MouseAdapter 
    {     
        public void mousePressed(MouseEvent e) 
        {
        	System.out.println("press");
            repaint();
        }
        
        public void mouseReleased(MouseEvent e) 
        {
        	System.out.println("releas");
            repaint();
        }

        public void mouseDragged(MouseEvent e) 
        {
        	System.out.println("drag");
        }
    }
    
    Color field1 = Color.BLACK;
    Color field2 = Color.WHITE;
    
    Color pawn1 = Color.RED;
    Color pawn2 = Color.BLUE;
    
    private Board board;
    private Board previousBoard;
    private int displayedBoard;
    
    private enum CursorState { Free, Grab, WaitForSerwerStateUpdate, WaitForIA };
    private CursorState cursorState;
}
