/**
 * Jeu du Taquin programmé en Java.
 * Le jeu se joue sur une grille de coté n, où se trouve n²-1 boutons
 * numérotés de 1 à n²-1 et une case videet le but est d'ordonner tous les 
 * boutons numérotés avec la case vide en haut à gauche ou en bas à droite.
 * Pour cela il est possible d'intervertir deux cases(un bouton et la case vide)
 * en cliquant sur un bouton voisin de la case vide.
 */
package taquin;

/**
 * @author Thomas BOURG
 */

import java.awt.*;
import java.awt.event.*;
import java.util.Arrays;
import javax.swing.*;

public class TaquinGraphique extends JFrame implements ActionListener {

    private final JButton[][] grilleGraphique;
    private final String[][] tab;
    private final String[][] solution1;
    private final String[][] solution2;
    private final JButton melanger;
    private final JButton retour;
    private final JPanel grille;
    private final JPanel menu;
    private int[] coordCaseVide;

    /**
     * Initialisation de toutes les données membres de la classe
     */
    public TaquinGraphique() {
        super("Taquin");
        grille = new JPanel();
        menu = new JPanel();
        retour = new JButton("Retour menu");
        melanger = new JButton("Mélanger");
        solution1 = new String[][]{{"", "1", "2"}, {"3", "4", "5"}, {"6", "7", "8"}};
        tab = new String[][]{{"", "1", "2"}, {"3", "4", "5"}, {"6", "7", "8"}};
        grilleGraphique = new JButton[3][3];
        solution2 = new String[][]{{"1", "2", "3"}, {"4", "5", "6"}, {"7", "8", ""}};
        setSize(350, 495);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        grille.setLayout(new GridLayout(3, 3));
        add(grille, BorderLayout.NORTH);
        menu.add(melanger);
        menu.add(retour);
        add(menu, BorderLayout.CENTER);


        //Ajout d'ActionListener pour les boutons
        melanger.addActionListener(this);
        retour.addActionListener(this);

        setVisible(true);

        //On appelle cette méthode pour initialiser le tableau
        initJLabel();
        //On appelle cette méthode pour mélanger les cases
        randomizer();
    }

    /**
     * Cette méthode initialise une grille de boutons correspondante
     * au tableau de jeu
     */
    public void initJLabel() {
        for (int i = 0; i < grilleGraphique.length; i++) {
            for (int j = 0; j < grilleGraphique[i].length; j++) {
                grilleGraphique[i][j] = new JButton();
                grilleGraphique[i][j].setPreferredSize(new Dimension(110, 110));
                grilleGraphique[i][j].addActionListener(this);
                grille.add(grilleGraphique[i][j]);
            }
        }
    }

    /**
     * Cette méthode met à jour l'affichage en réinitialisant le texte
     * de chaque bouton
     */
    public void updateJLabel() {
        for (int i = 0; i < tab.length; i++) {
            for (int j = 0; j < tab[i].length; j++) {
                grilleGraphique[i][j].setText((tab[i][j]));
            }
        }
        chkVictoire();

    }

    /**
     * Cette méthode mélange toutes les cases en les intervertissant
     * chacune avec une case au hasard
     */
    private void randomizer() {
        for (int i = 0; i < tab.length; i++) {
            for (int j = 0; j < tab[i].length; j++) {
                int rdmLig = (int) (Math.random() * tab.length);
                int rdmCol = (int) (Math.random() * tab[i].length);

                String tmp = tab[rdmLig][rdmCol];
                tab[rdmLig][rdmCol] = tab[i][j];
                tab[i][j] = tmp;
            }
        }
        updateJLabel();
    }

    /**
     * Cette méthode sert à mettre à jour les coordonnées de la case vide
     */
    public void updateVide() {
        for (int i = 0; i < tab.length; i++) {
            for (int j = 0; j < tab[i].length; j++) {
                String tmp = tab[i][j];
                if (tmp.equals("")) {
                    coordCaseVide = new int[]{i, j};
                }
            }
        }
    }

    /**
     * Cette méthode sert à intervertir la case vide avec le bouton
     * sur lequel on a cliqué, si tant est qu'il soit voisin de la case vide
     * @param i coordonnée x du bouton
     * @param j coordonnée y du bouton
     */
    public void invert(int i, int j) {
        updateVide();
        
        if ((coordCaseVide[0] == i && (coordCaseVide[1] == j-1 || coordCaseVide[1] == j + 1))
            || (coordCaseVide[1] == j && (coordCaseVide[0] == i - 1 || coordCaseVide[0] == i + 1))) {
            String tmp = tab[coordCaseVide[0]][coordCaseVide[1]];
            tab[coordCaseVide[0]][coordCaseVide[1]] = tab[i][j];
            tab[i][j] = tmp;
            updateJLabel();
        }
    }

    /**
     * 
     */
    public void chkVictoire() {
        if (Arrays.equals(tab, solution1) || Arrays.equals(tab, solution2)) {
            System.out.println("Gagné");
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        for (int i = 0; i < tab.length; i++) {
            for (int j = 0; j < tab[i].length; j++) {
                if (e.getSource() == grilleGraphique[i][j]) {
                    invert(i, j);
                }
            }
        }
        if (e.getSource() == melanger) {
            randomizer();
        }
        if (e.getSource() == retour) {
            new TaquinGraphique();

        }
    }

    public static void main(String[] args) {
        TaquinGraphique fen = new TaquinGraphique();

    }
}
