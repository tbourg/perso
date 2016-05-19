/**
 *
 * @author p1500925
 */
package outils;

import java.util.Scanner;
import java.util.InputMismatchException;

public class Clavier {

    Scanner s;

    /**
     *
     * Constructeur
     */
    public Clavier() {
        s = new Scanner(System.in);
    }

    /**
     *
     * lecture d'un entier avec validation de saisie
     * @return un entier validé
     */
    public int getInt() {
        int i;
        i = 0;
        boolean ok;

        do {
            try {
                ok = true;
                i = s.nextInt();
            } catch (InputMismatchException e) {
                ok = false;
                System.out.println("Il faut entrer un entier valide");
                System.out.print("Entrer un entier : ");
            }
            s.nextLine();
        } while (!ok);
        return i;
    }

    /**
     *
     * lecture d'un réel simple précision avec validation de saisie
     * @return un réel simple précision validé
     */
    public float getFloat() {
        float f;
        f = 0.0f;
        boolean ok;

        do {
            try {
                ok = true;
                f = s.nextFloat();
            } catch (InputMismatchException e) {
                ok = false;
                System.out.println("Il faut entrer un réel valide");
                System.out.print("Entrer un réel (simple précision) : ");
            }
            s.nextLine();
        } while (!ok);
        return f;
    }

    /**
     *
     * lecture d'un réel double précision avec validation de saisie
     * @return un réel double précision validé
     */
    public double getDouble() {
        double d;
        d = 0.0;
        boolean ok;

        do {
            try {
                ok = true;
                d = s.nextDouble();
            } catch (InputMismatchException e) {
                ok = false;
                System.out.println("Il faut entrer un réel valide");
                System.out.print("Entrer un réel (double précision) : ");
            }
            s.nextLine();
        } while (!ok);
        return d;
    }

    /**
     *
     * lecture du premier caractère d'une chaîne
     * @return le premier caractère de la chaîne lue
     */
    public char getChar() {
        return (s.nextLine().charAt(0));
    }

    /**
     *
     * lecture d'une chaîne de caractères
     * @return une chaîne de caractères débarassée de la touche entrée
     */
    public String getString() {
        return (s.nextLine());
    }
}