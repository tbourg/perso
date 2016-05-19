/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package morpion;

import java.awt.*;
import java.awt.event.*;
import java.util.Arrays;
import javax.swing.*;

/**
 *
 * @author tbour_000
 */
public class Plateau extends JFrame implements ActionListener{
    Pion [][] tab;
    JPanel paneau;
    GridLayout grille;
    JButton
    
    public Plateau(){
        tab = new Pion[3][3];
    }
    
    public void ajoutPion(int i,int j, boolean rond){
        tab[i][j] = new Pion(rond);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }
    
}
