/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package firstproject;
import java.io.*;
import java.util.*;
/**
 *
 * @author tbour_000
 */
public class FirstProject {

    private final String name;
    private final int sal;

    public FirstProject(String name,int sal){
        this.name = name;
        this.sal = sal;
    }
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        double x, somme = 0, pdt = 1;
        System.out.println("vous avez entre "+ args.length +" arguments:");
        for(String arg:args){
            x = Double.parseDouble(arg);
            System.out.printf("%.1f\n", x);
            somme += x;
            pdt *= x;
        }
        System.out.println("la sommme de ces arguments est "+ somme +" et leur produit est "+ pdt);
        FirstProject e = new FirstProject("j.dupont", 3500);
        // TODO code application logic here
        ArrayList 
    }
    
    @Override
    public String toString(){
        
    }
    
}
