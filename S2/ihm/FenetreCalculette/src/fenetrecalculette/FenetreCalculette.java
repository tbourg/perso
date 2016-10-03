/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package fenetrecalculette;

import java.awt.*;
import javax.swing.*;

/**
 *
 * @author p1500925
 */
public class FenetreCalculette extends JFrame {

    JPanel pan;
    JTextField nbre1;
    JTextField nbre2;
    JComboBox operateur;
    JButton go;
    Label result;

    public FenetreCalculette() {
        super();
        init();
    }

    public void init() {
        setTitle("Calculatrice");
        setSize(400, 200);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        pan = new JPanel();
        nbre1= new JTextField(7);
        nbre2= new JTextField(7);
        result= new Label("=.........");
        operateur = new JComboBox();
        operateur.addItem("+");
        operateur.addItem("-");
        operateur.addItem("×");
        operateur.addItem("/");
        go = new JButton("=");
        pan.add(nbre1);
        pan.add(operateur);
        pan.add(nbre2);
        pan.add(go);
        pan.add(result);
        setContentPane(pan);
        setVisible(true);
    }
}
