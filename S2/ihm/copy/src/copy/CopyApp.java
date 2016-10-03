/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package copy;

/**
 *
 * @author tbour_000
 */
public class CopyApp {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
                Copy fenetre = new Copy();
                fenetre.setVisible(true);
            }
        });
    }

}
