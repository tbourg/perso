package compte;

/**
 *
 * @author tbour_000
 */
public class Compte {

    int numCompte;
    String nomClient;
    double solde;

    static int ctrComptes = 0;

    public Compte(String nC, double s) {
        numCompte = ++ctrComptes;
        nomClient = nC;
        solde = s;
    }

    public void debiter(double diff) {
        solde -= diff;
        System.out.println("On a débité le compte n° " + numCompte + " de \"" + nomClient + "\" de " + diff + " Euros");
    }

    public void crediter(double diff) {
        solde += diff;
        System.out.println("On a crédité le compte n° " + numCompte + " de \"" + nomClient + "\" de " + diff + " Euros");
    }

    @Override
    public String toString() {
        return "Le compte n° " + numCompte + " de \"" + nomClient + "\" a un solde de " + solde + " Euros";
    }

    @Override
    public boolean equals(Object o) {
        if (this.getClass() == o.getClass()) {
            return false;
        }
        Compte c = (Compte) o;
        return (this.nomClient == c.nomClient
                && this.numCompte == c.numCompte
                && this.solde == c.solde);
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Le nombre courant de comptes crées est :" + ctrComptes);
        Compte c1, c2;
        c1 = new Compte("Jean DUPONT", 1000);
        c2 = new Compte("Jacques DURAND", 5000);
        System.out.println(c1);
        System.out.println(c2);
        System.out.println("Le nombre courant de comptes crées est :" + ctrComptes);
        c1.crediter(100);
        System.out.println(c1);
        c2.debiter(50);
        System.out.println(c2);
        System.out.println("TEST DE LA METHODE equals");
        if (c1.equals(c2)) {
            System.out.println("Les deux comptes sont identiques");
        } else {
            System.out.println("Les deux comptes sont différents");
        }
    }

}
