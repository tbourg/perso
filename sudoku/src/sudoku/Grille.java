/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package sudoku;

import java.util.*;

/**
 *
 * @author tbour_000
 */
public class Grille {

    Case grille[][];

    public Grille(int[][] tab) {
        grille = new Case[9][9];
        int i, j;
        i = 0;
        for (int[] ligne : tab) {
            j = 0;
            for (int cell : ligne) {
                grille[i][j] = new Case(cell);
                j++;
            }
            i++;
        }
    }

    public void ligneEnColonne() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                Case tmp = grille[j][i];
                grille[j][i] = grille[i][j];
                grille[i][j] = tmp;
            }
        }
    }

    public void ligneEnBloc() {
        int cpl = 0, cpc = 0;
        for (int a = 1; a < 4; a++) {
            for (int b = 1; b < 4; b++) {
                int aa = a - 1 * 3 + 1;
                int bb = b - 1 * 3 + 1;
                cpl++;
                cpc = 0;
                for (int y = aa; aa < aa + 2; y++) {
                    for (int z = bb; z < bb + 2; z++) {
                        Case tmp = grille[cpl][cpc];
                        grille[cpc][cpl] = grille[y][z];
                        grille[y][z] = tmp;
                        cpc++;
                    }
                }
            }
        }
    }

    public void calculPossibilites() {
        for (Case[] ligne : grille) {
            for (Case elem : ligne) {
                elem.bloc.clear();
                elem.col.clear();
                elem.ligne.clear();
                elem.possibilités.clear();
            }
        }
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                for (int j_l = 0; j_l < 9; j_l++) {
                    if (grille[i][j_l].val != 0) {
                        grille[i][j].ajouterDansLigne(grille[i][j_l].val);
                    }
                }
                for (int i_c = 0; i_c < 9; i_c++) {
                    if (grille[i_c][j].val != 0) {
                        grille[i][j].ajouterDansCol(grille[i_c][j].val);
                    }
                }
                int _i = i - (i % 3), _j = j - (j % 3), i_b, j_b;  // ou encore : _i = 3*(i/3), _j = 3*(j/3);
                for (i_b = _i; i_b < _i + 3; i_b++) {
                    for (j_b = _j; j_b < _j + 3; j_b++) {
                        if (grille[i_b][j_b].val != 0) {
                            grille[i][j].ajouterDansBloc(grille[i_b][j_b].val);
                        }
                    }
                }
            }
        }
        for (Case[] ligne : grille) {
            for (Case elem : ligne) {
                if (elem.val == 0) {
                    elem.calculPossibilités();
                }
            }
        }
    }

    public void uneSeulePossibilite() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (grille[i][j].possibilités.size() == 1 && grille[i][j].val == 0) {
                    grille[i][j].val = (int) grille[i][j].possibilités.get(0);
                }
            }
        }
    }

    public void affichage() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                System.out.printf(((j + 1) % 3 == 0) ? "%d " : "%d|", grille[i][j].val);
            }
            System.out.println();
            if (((i + 1) % 3) == 0) {
                System.out.println("------------------");
            }
        }
    }

    public void groupeAutonome() {
        for (Case[] ligne : grille) {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (i != j) {
                        if (ligne[i].possibilités.equals(ligne[j].possibilités) && ligne[i].possibilités.size() == 2) {
                            for (int k = 0; k < 9; k++) {
                                if (k != i && k != j) {
                                    ligne[k].possibilités.remove(ligne[i].possibilités.get(0));
                                    ligne[k].possibilités.remove(ligne[i].possibilités.get(1));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    public void resolutionLin() {
        boolean continuer = true;
        int cpt_lin = 0;
        long debut = System.currentTimeMillis();
        while (continuer) {
            int vide_avt = this.nbreCaseVide();
            this.calculPossibilites();
            this.uneSeulePossibilite();
            this.calculPossibilites();
            this.positionUnique();
            int vide_apres = this.nbreCaseVide();
            continuer = vide_avt != vide_apres;
            cpt_lin++;
        }
        long fin = System.currentTimeMillis();
        System.out.println(cpt_lin + "tours (en linéaire)");
        System.out.println(fin - debut + " millisecondes en résolution linéaire");
    }

    public void resolutionSeq() {
        Grille avt, apr;
        int cpt_seq = 0;
        long debut = System.currentTimeMillis();
        do {
            avt = this;
            calculPossibilites();
            uneSeulePossibilite();
            calculPossibilites();
            positionUnique();
            this.groupeAutonome();
            this.uneSeulePossibilite();
            this.calculPossibilites();
            apr = this;
            cpt_seq++;
        } while (cpt_seq < 9);
        long fin = System.currentTimeMillis();
        System.out.println(cpt_seq + "tours (en séquentiel)");
        System.out.println(fin - debut + " millisecondes en résolution séquentielle");
    }

    public void positionUnique() {
        for (int nbre = 1; nbre < 10; nbre++) {
            for (int i = 0; i < 9; i++) {
                int nbre_sur_ligne = 0;
                for (int j = 0; j < 9; j++) {
                    if (grille[i][j].possibilités.contains(nbre)) {
                        nbre_sur_ligne++;
                    }
                }
                if (nbre_sur_ligne == 1) {
                    for (int j = 0; j < 9; j++) {
                        if (grille[i][j].possibilités.contains(nbre) && grille[i][j].val == 0) {
                            grille[i][j].val = nbre;
                        }
                    }
                }
                calculPossibilites();
            }
            for (int j = 0; j < 9; j++) {
                int nbre_sur_col = 0;
                for (int i = 0; i < 9; i++) {
                    if (grille[i][j].possibilités.contains(nbre)) {
                        nbre_sur_col++;
                    }
                }
                if (nbre_sur_col == 1) {
                    for (int i = 0; i < 9; i++) {
                        if (grille[i][j].possibilités.contains(nbre) && grille[i][j].val == 0) {
                            grille[i][j].val = nbre;
                        }
                    }
                }
                calculPossibilites();
            }
            for (int i = 0; i < 7; i = i + 3) {
                for (int j = 0; j < 7; j = j + 3) {
                    int nbre_sur_bloc = 0;
                    for (int i_int = i; i_int < i + 3; i_int++) {
                        for (int j_int = j; j_int < j + 3; j_int++) {
                            if (grille[i_int][j_int].possibilités.contains(nbre)) {
                                nbre_sur_bloc++;
                            }
                        }
                    }
                    if (nbre_sur_bloc == 1) {
                        for (int i_int = i; i_int < i + 3; i_int++) {
                            for (int j_int = j; j_int < j + 3; j_int++) {
                                if (grille[i_int][j_int].possibilités.contains(nbre)) {
                                    grille[i_int][j_int].val = nbre;
                                }
                            }
                        }
                    }
                    calculPossibilites();
                }
                calculPossibilites();
            }
        }
    }

    public int nbreCaseVide() {
        int nbre_vide = 0;
        for (Case[] ligne : grille) {
            for (Case elem : ligne) {
                if (elem.val == 0) {
                    nbre_vide++;
                }
            }
        }
        return nbre_vide;
    }

    public static void main(String[] args) {

        int grille_stock[][] = {
            {6, 0, 5, 0, 8, 0, 4, 9, 3},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 2, 0},
            {7, 1, 0, 0, 0, 3, 0, 5, 0},
            {0, 0, 6, 0, 0, 0, 3, 0, 0},
            {0, 3, 0, 8, 0, 0, 0, 0, 6},
            {0, 9, 0, 0, 0, 0, 7, 0, 0},
            {0, 8, 0, 1, 5, 0, 0, 0, 9},
            {0, 0, 0, 0, 7, 0, 8, 0, 1}
        };

        Grille sudoku = new Grille(grille_stock);

        System.out.printf("Grille avant\n");
        sudoku.affichage();
        sudoku.calculPossibilites();

        //sudoku.resolutionLin();
        //sudoku = new Grille(grille_stock);
        sudoku.resolutionSeq();

        System.out.printf("Grille apres\n");
        sudoku.affichage();
        System.out.println(sudoku.grille[8][0].possibilités.toString() + sudoku.grille[0][3].possibilités + sudoku.grille[0][5].possibilités);
    }
}
