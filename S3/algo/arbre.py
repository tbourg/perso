class noeud:
    def __init__(self, val, f_g, f_d):
        self.val = val
        self.f_g = f_g
        self.f_d = f_d

def parcours_prefixe(racine):
    if racine != None:
        print (racine.val)
        parcours_prefixe(racine.f_g)
        parcours_prefixe(racine.f_d)

def hauteur(n):
    if n == None:
        return -1
    else:
        return 1 + (hauteur(n.f_g) if hauteur(n.f_g) < hauteur(n.f_d) else hauteur(n.f_d))

def taille(noeud):
    if noeud == None:
        return 0
    else:
        return 1 + taille(noeud.f_g) + taille(noeud.f_d)

def recherche(noeud, a):
    if noeud == None:
        return False
    elif noeud.val == a:
        return True
    else:
        return recherche(noeud.f_g, a) or recherche(noeud.f_d, a)

def mini(noeud):
    if noeud == None:
        return float('inf')
    else:
        return min(mini(noeud.f_g), mini(noeud.f_d), noeud.val)

f1 = noeud(1, None, None)
f2 = noeud(2, None, None)
racine = noeud(3, f1, f2)
print ('parcours')
parcours_prefixe(racine)
print ('hauteur')
print (hauteur(racine))
print ('taille')
print (taille(racine))
print ('recherche de deux')
print (recherche(racine, 2))
print ('de zero')
print (recherche(racine, 0))
print ('minimum')
print (mini(racine))
