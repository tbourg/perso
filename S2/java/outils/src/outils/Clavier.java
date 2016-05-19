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
     * @return un entier valid�
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
     * lecture d'un r�el simple pr�cision avec validation de saisie
     * @return un r�el simple pr�cision valid�
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
                System.out.println("Il faut entrer un r�el valide");
                System.out.print("Entrer un r�el (simple pr�cision) : ");
            }
            s.nextLine();
        } while (!ok);
        return f;
    }

    /**
     *
     * lecture d'un r�el double pr�cision avec validation de saisie
     * @return un r�el double pr�cision valid�
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
                System.out.println("Il faut entrer un r�el valide");
                System.out.print("Entrer un r�el (double pr�cision) : ");
            }
            s.nextLine();
        } while (!ok);
        return d;
    }

    /**
     *
     * lecture du premier caract�re d'une cha�ne
     * @return le premier caract�re de la cha�ne lue
     */
    public char getChar() {
        return (s.nextLine().charAt(0));
    }

    /**
     *
     * lecture d'une cha�ne de caract�res
     * @return une cha�ne de caract�res d�barass�e de la touche entr�e
     */
    public String getString() {
        return (s.nextLine());
    }
}