/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ecouteur;

import java.awt.event.*;
import javax.swing.*;

/**
 *
 * @author p1500925
 */
public class Ecouteur extends JFrame implements ActionListener {

    JButton b1;
    JButton b2;

    public Ecouteur() {
        super("Composition de Disposition");
        setSize(420, 206);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        initCompo();
        setVisible(true);
    }

    public void initCompo() {
        JPanel top = new JPanel();
        b1 = new JButton("Ici");
        b2 = new JButton("Là");
        b1.addActionListener(this);
        b2.addActionListener(this);
        top.add(b1);
        top.add(b2);
        setContentPane(top);

    }

    public void actionPerformed(ActionEvent e) {
        if (e.getSource() == b1) {
            System.out.println("Vous avez cliquez ici.");
        } else {
            System.out.println("Vous avez cliquez là.");
        }

    }
}
