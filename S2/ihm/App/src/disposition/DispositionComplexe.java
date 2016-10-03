/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package disposition;

import javax.swing.*;
import java.awt.*;
/**
 *
 * @author p1500925
 */
public class DispositionComplexe extends JFrame{
    
    public DispositionComplexe(){
        super("Composition de Disposition");
        setSize(420, 206);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        initCompo();
        setVisible(true); 
    }
    
    public void initCompo(){
        JPanel top = new JPanel();
        JButton b1 = new JButton("Cliquez-ici!");
        JButton b2 = new JButton("Ou là!");
        top.add(b1);
        top.add(b2);
        
        JPanel mid = new JPanel();
        JButton b3 = new JButton("Ou encore là!");
        mid.setLayout(new BorderLayout());
        mid.add(b3);
        
        JPanel bottom = new JPanel();
        Label l1 = new Label("Sud-ouest");
        Label l2 = new Label("Sud-est");
        bottom.setLayout(new BorderLayout());
        bottom.add(l1, BorderLayout.WEST);
        bottom.add(l2, BorderLayout.EAST);
        
        JPanel cp = new JPanel();
        cp.setLayout(new GridLayout(3, 1));
        cp.add(top);
        cp.add(mid);
        cp.add(bottom);
        
        setContentPane(cp);
    }
}
