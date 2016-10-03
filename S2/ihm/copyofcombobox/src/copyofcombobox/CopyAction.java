/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package copyofcombobox;

import java.awt.event.ActionEvent;
import javax.swing.AbstractAction;

/**
 *
 * @author p1500925
 */
public class CopyAction extends AbstractAction {

    Combo cr;

    public CopyAction(Combo cr, String txt) {
        super(txt);
        this.cr =cr;
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        cr.box2.addItem(cr.box1.getSelectedItem());
    }
}
