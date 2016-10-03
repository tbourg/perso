# -*- coding: Latin-1 -*-
#-------------------------------------------------------------------------------
# Name:        Bipo Maze
# Author:      Jimmy (WoofWoofDude)
# Created:     2012/04/06
# Python:      3.2.2 (default, Sep  4 2011, 09:51:08) [MSC v.1500 32 bit (Intel)]
#-------------------------------------------------------------------------------
#!/usr/bin/env python
from Classe import Bipo


def liste_bip(WW, WH):
    liste_b = []
    
    X = WW
    Y = WH
    
    Xi = 0
    Yi = 0
    
    while Yi < int(WH/16):
        while Xi < int(WW/16):
            liste_b.append(Bipo((Xi * 16, Yi * 16)))
            Xi += 1
            
        Xi = 0
        Yi += 1    
        
    return liste_b

def fill_liste_b2(liste_b1):
    liste_b2 = []
    for i in liste_b1:
        liste_b2.append(i)    
        
    return liste_b2