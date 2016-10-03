/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package stringManipulation;

import java.awt.*;
import javax.swing.*;
import java.awt.event.*;

/**
 *
 * @author tbour_000
 */
public class StringTest extends JFrame {

    JPanel cp;
    JButton[] tb;
    static final String[] ts = {"MAJUSCULES", "Minuscules", "Retrait Blancs", "Longueur", "Quitte", "AjoutFin", "AjoutDebut", "Remplace"};
    JLabel lb;
    JTextField fd;
    StringMoteur moteur;

    public StringTest() {
        super("String Test");
        setSize(600, 150);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        moteur = new StringMoteur(this);
        initCompo();
        setVisible(true);
    }

    public void initCompo() {
        tb = new JButton[8];
        for (int i = 0; i < ts.length; i++) {
            tb[i] = new JButton(ts[i]);
        }
        lb = new JLabel("Welcome to the String Tester");
        fd = new JTextField(20);

        JPanel top = new JPanel();
        for (int i = 0; i < 5; i++) {
            top.add(tb[i]);
        }

        JPanel mid = new JPanel();
        mid.setLayout(new BorderLayout());
        mid.add(lb);

        JPanel bottom = new JPanel();
        bottom.add(fd);
        for (int i = 5; i < tb.length; i++) {
            bottom.add(tb[i]);
        }

        cp = new JPanel();
        cp.setLayout(new GridLayout(3, 1));
        cp.add(top);
        cp.add(mid);
        cp.add(bottom);

        initEvent();
        
        setContentPane(cp);
    }

    public void initEvent() {
        tb[0].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.maj());
            }
        });
        tb[1].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.min());
            }
        });
        tb[2].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.ret());
            }
        });
        tb[3].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.taille());
            }
        });
        tb[4].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                moteur.quitte();
            }
        });
        tb[5].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.ajFin());
                fd.setText("");
            }
        });
        tb[6].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.ajDeb());
                fd.setText("");
            }
        });
        tb[7].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                lb.setText(moteur.remplace());
                fd.setText("");
            }
        });
    }

    public String getStringEntree() {
        return fd.getText();
    }

    public String getStringAffichee() {
        return lb.getText();
    }
    
    public static void main(String[] args) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                StringTest fenetre = new StringTest();
                fenetre.setVisible(true);
            }
        });
    }

}
