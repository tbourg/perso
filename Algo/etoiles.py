etoile = "x"
espace = " "
i = 2
base = float(input())
hauteur = float(input())
print(etoile)
while i < hauteur:
    print(etoile, espace*0, etoile)
    i = i+1
print(etoile*base)
