/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package stringManipulation;


/**
 *
 * @author tbour_000
 */
public class StringMoteur {

    StringTest interfaceUtilisateur;
    
    public StringMoteur(StringTest objetInterface){
        interfaceUtilisateur = objetInterface;
    }
    
    public String maj(){
        return interfaceUtilisateur.getStringAffichee().toUpperCase();
    }
    
    public String min(){
        return interfaceUtilisateur.getStringAffichee().toLowerCase();
    }
    
    public String ret(){
        return interfaceUtilisateur.getStringAffichee().trim();
    }
    
    public String taille(){
        return Integer.toString(interfaceUtilisateur.getStringAffichee().length());
    }
    
    public void quitte(){
        System.exit(0);
    }
    
    public String ajDeb(){
        return interfaceUtilisateur.getStringEntree() + interfaceUtilisateur.getStringAffichee();
    }
    
    public String ajFin(){
        return interfaceUtilisateur.getStringAffichee() + interfaceUtilisateur.getStringEntree();
    }
    
    public String remplace(){
        return interfaceUtilisateur.getStringEntree();
    }
}
