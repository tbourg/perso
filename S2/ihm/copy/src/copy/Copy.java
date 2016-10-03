/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package copy;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/**
 *
 * @author tbour_000
 */
public class Copy extends JFrame {
    
    JPanel pan;
    JButton cp;
    JTextField txt;
    JLabel lab;
    
    public Copy() {
        super("Copy");
        setSize(300, 350);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        initCompo();
        setVisible(true);
    }
    
    public void initCompo() {
        pan = new JPanel();
        txt = new JTextField(10);
        cp = new JButton();
        lab = new JLabel();
        
        pan.add(txt);
        pan.add(cp);
        pan.add(lab);
        
        cp.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lab.setText(txt.getText());
            }
        });
        
        setContentPane(pan);
    }
}
