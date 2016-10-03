/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package copyofcombobox;

import java.awt.event.*;
import javax.swing.*;

/**
 *
 * @author tbour_000
 */
public class Combo extends JFrame {

    JPanel pan;
    JComboBox box1;
    JComboBox box2;
    JButton b1;
    JButton b2;
    JButton b3;

    public Combo() {
        super("Combo");
        setSize(300, 350);
        setResizable(false);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        initCompo();
        setVisible(true);
    }

    public void initCompo() {
        pan = new JPanel();
        box1 = new JComboBox();
        box2 = new JComboBox();
        b1 = new JButton(new CopyAction(this,"cp"));
        b2 = new JButton(new DelAction(this,"del"));
        b3 = new JButton(new DelAllAction(this,"delAll"));

        box1.addItem("a");
        box1.addItem("z");
        box1.addItem("e");
        box1.addItem("r");

        pan.add(box1);
        pan.add(b1);
        pan.add(b2);
        pan.add(b3);
        pan.add(box2);

        setContentPane(pan);
    }
}
