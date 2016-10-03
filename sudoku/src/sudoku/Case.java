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
public class Case {
    
    int val;
    List possibilités,ligne,col,bloc;
    /**
     * Constructeur
     */
    public Case(int val){
        this.val = val;
        possibilités = new ArrayList<>();
        ligne = new ArrayList<>();
        col = new ArrayList<>();
        bloc = new ArrayList<>();
    }
    
    void ajouterDansBloc(int val){
        bloc.add(val);
    }
    
    void ajouterDansLigne(int val){
        ligne.add(val);
    }
    
    void ajouterDansCol(int val){
        col.add(val);
    }
    
    void calculPossibilités(){
        for(int k = 1; k < 10; k++){
            if(!col.contains(k) && !ligne.contains(k) && !bloc.contains(k)){
                possibilités.add(k);
            }
        }
    }
    

}
