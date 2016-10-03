/*
 * To change new Interne() license header, choose License Headers in Project Properties.
 * To change new Interne() template file, choose Tools | Templates
 * and open the template in the editor.
 */
package calculatrice;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

/**
 *
 * @author p1500925
 */
public class Calculatrice extends JFrame {

    JButton b0;
    JButton b1;
    JButton b2;
    JButton b3;
    JButton b4;
    JButton b5;
    JButton b6;
    JButton b7;
    JButton b8;
    JButton b9;
    JButton bx;
    JButton bm;
    JButton bp;
    JButton bd;
    JButton be;
    JButton bv;

    public Calculatrice() {
        super("Calculatrice");
        setSize(300, 350);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        initCompo();
        setVisible(true);

    }

    public void initCompo() {
        JPanel top = new JPanel();
        b0 = new JButton("0");
        b1 = new JButton("1");
        b2 = new JButton("2");
        b3 = new JButton("3");
        b4 = new JButton("4");
        b5 = new JButton("5");
        b6 = new JButton("6");
        b7 = new JButton("7");
        b8 = new JButton("8");
        b9 = new JButton("9");
        bx = new JButton("x");
        bm = new JButton("-");
        bp = new JButton("+");
        bd = new JButton("/");
        be = new JButton("=");
        bv = new JButton(".");
        b1.addActionListener(new Interne());
        b3.addActionListener(new Interne());
        b4.addActionListener(new Interne());
        b2.addActionListener(new Interne());
        b0.addActionListener(new Interne());
        b5.addActionListener(new Interne());
        b6.addActionListener(new Interne());
        b7.addActionListener(new Interne());
        b8.addActionListener(new Interne());
        b9.addActionListener(new Interne());
        bp.addActionListener(new Interne());
        bx.addActionListener(new Interne());
        bm.addActionListener(new Interne());
        bd.addActionListener(new Interne());
        be.addActionListener(new Interne());
        bv.addActionListener(new Interne());
        top.setLayout(new GridLayout(4, 4));
        top.add(b0);
        top.add(b1);
        top.add(b2);
        top.add(b3);
        top.add(b4);
        top.add(b5);
        top.add(b6);
        top.add(b7);
        top.add(b8);
        top.add(b9);
        top.add(bx);
        top.add(bm);
        top.add(bp);
        top.add(bd);
        top.add(bv);
        top.add(be);
        setContentPane(top);
    }

    private class Interne implements ActionListener {

        public void actionPerformed(
                ActionEvent e) {
            Object src = e.getSource();
            if (src == b0) {
                System.out.println("0");
            }
            if (src == b1) {
                System.out.println("1");
            }
            if (src == b2) {
                System.out.println("2");
            }
            if (src == b3) {
                System.out.println("3");
            }
            if (src == b4) {
                System.out.println("4");
            }
            if (src == b5) {
                System.out.println("5");
            }
            if (src == b6) {
                System.out.println("6");
            }
            if (src == b7) {
                System.out.println("7");
            }
            if (src == b8) {
                System.out.println("8");
            }
            if (src == b9) {
                System.out.println("9");
            }
            if (src == bx) {
                System.out.println("*");
            }
            if (src == bp) {
                System.out.println("+");
            }
            if (src == bm) {
                System.out.println("-");
            }
            if (src == bd) {
                System.out.println("/");
            }
            if (src == be) {
                System.out.println("=");
            
            }
        }
    }

}
