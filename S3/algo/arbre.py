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

def insertion(val, noeud_):
    if noeud_.f_g == None:
        noeud_.f_g = noeud(val, None, None)
    else:
        insertion(val, noeud_.f_g)

def suppr(val, noeud):
    if noeud.val == val:
        while noeud.f_g != None:
            prec = noeud
            noeud.val = noeud.f_g.val
            noeud = noeud.f_g
        prec.f_g = None
    else:
        suppr(val, noeud.f_g)

def parcours_infixe(noeud):
    if noeud != None:
        parcours_infixe(noeud.f_g)
        print(noeud.val)
        parcours_infixe(noeud.f_d)

def planter(liste):
    racine = noeud(liste[0], None, None)
    cour = racine
    for i in range(1, len(liste)):
        cour = racine
        new = liste[i]
        while cour != None:
            prec = cour
            if new < cour.val:
                cour = cour.f_g
            else:
                cour = cour.f_d
        if new < prec.val:
            prec.f_g = noeud(new, None, None)
        else:
            prec.f_d = noeud(new, None, None)
    return racine

class tri:
    def __init__(self, tab):
        self.abr = planter(tab)
        self.liste = []
        self.transform_rec(self.abr)

    def transform_rec(self, ref):
        noeud = ref
        if noeud != None:
            self.transform_rec(noeud.f_g)
            self.liste.append(noeud.val)
            self.transform_rec(noeud.f_d)

    def __str__(self):
        return self.liste.__str__()


f1 = noeud(1, None, None)
f2 = noeud(2, None, None)
#racine = noeud(3, f1, f2)
#racine = noeud(5, noeud(1, None, noeud(2, None, noeud(3, None, noeud(4, None, None)))), noeud(6, None, None))
print(tri([5, 3, 7, 2, 6, 1, 4]))
racine = planter([5, 3, 7, 2, 6, 1, 4])
print ('parcours')
parcours_infixe(racine)
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
"""
insertion(7, racine)
print ('parcours')
parcours_prefixe(racine)
suppr(1, racine)
print ('parcours')
parcours_prefixe(racine)

#pour trier un tableau, on peut creer un abr à partir des valeurs du tableau puis un parcours infixe de cet arbre nous donnera la liste ordonnee des valeurs
"""
