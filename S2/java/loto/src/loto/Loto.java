/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package loto;

import java.util.*;

/**
 *
 * @author p1500925
 */
public class Loto {

    public static final int BORNINF = 1;
    public static final int BORNESUP = 49;
    public static final int NBRE = 6;
    ArrayList<Integer> tirage;
    Integer[] tab;

    public Loto() {
        tirage = new ArrayList<>();
        tab = new Integer[NBRE];
    }

    public void remplirTab() {
        int tmp;
        for (int i = 0; i < NBRE; i++) {
            do {
                tmp = (int) ((BORNESUP - BORNINF + 1) * Math.random() + BORNINF);
                if (!tirage.contains(tmp)) {
                    tirage.add(tmp);
                }
            } while (!tirage.contains(tmp));
        }
        tab = tirage.toArray(tab);
    }

    public void triage() {
        Arrays.sort(tab);
    }

    @Override
    public String toString() {
        String ch = "";
        for (Integer tab1 : tab) {
            ch += tab1;
            ch += "\t";
        }
        return ch;
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        // TODO code application logic here
        Scanner sc = new Scanner(System.in);
        do {
            System.out.println("\tLE\n\t"
                    + "*     ****  *****  ****\n\t"
                    + "*     *  *    *    *  *\n\t"
                    + "****  ****    *    ****\n\t"
                    + "\n\t"
                    + "c'est facile...\n\t"
                    + "c'est pas cher...\n\t"
                    + "et ça peut rapporter gros!!!\n\t"
                    + "\n"
                    + "Voici un tirage aléatoire de "
                    + NBRE
                    + " chiffres:");
            Loto loto = new Loto();
            loto.remplirTab();
            System.out.println(loto);
            System.out.println("Voici ces "
                    + NBRE
                    + " chiffres dans l'ordre:");
            loto.triage();
            System.out.println(loto);
            System.out.print("Recommencer (o/n) ? ");
        } while (!sc.nextLine().equalsIgnoreCase("n"));
    }

}
