package pl.checkers.checkersarea;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.event.MouseEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.JPanel;

public class CheckersArea extends JPanel 
{
	private MouseHandler mouseHandler = new MouseHandler();
	
    public CheckersArea()
    {
        setPreferredSize(new Dimension(450, 450));
        setBackground(Color.GREEN);
        this.addMouseListener(mouseHandler);
        this.addMouseMotionListener(mouseHandler);
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
}
