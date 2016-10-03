def affichage (grille):
    for i in range(9):
        for j in range(9): 
            print(str(grille[i][j])+'|',end='')
        print()
        if (((i+1)%3)!=0):
            print("------------------")
    print("\n\n")

def absentSurLigne (k, grille, i):
    for j in range(9):
        if (grille[i][j] == k):
            return false
    return true

def absentSurColonne (k, grille, j):
    for i in range(9):
        if (grille[i][j] == k):
            return false
    return true

def absentSurBloc (k, grille, i, j):
    ii = i-(i%3)
    jj = j-(j%3)
    for i in range(ii,ii+3):
        for j in range(jj,jj+3):
            if (grille[i][j] == k):
                return false
    return true

def estValide (grille, position):
    if (position == 9*9):
        return true

    i = position//9,
    j = position%9

    if (grille[i][j] != 0):
        return estValide(grille, position+1)

    for k in range(1,10):
        if (absentSurLigne(k,grille,i) and absentSurColonne(k,grille,j) and absentSurBloc(k,grille,i,j)):
            grille[i][j] = k

            if ( estValide (grille, position+1) ):
                return true
    grille[i][j] = 0

    return false

grille =[[9, 0, 0, 1, 0, 0, 0, 0, 5], [0, 0, 5, 0, 9, 0,2,0,1],[8,0,0,0,4,0,0,0,0],[0,0,0,0,8,0,0,0,0],[0,0,0,7,0,0,0,0,0],[0,0,0,0,2,6,0,0,9],[2,0,0,3,0,0,0,0,6],[0,0,0,2,0,0,9,0,0],[0,0,1,9,0,4,5,7,0]]

print("Grille avant\n")
affichage(grille)

estValide(grille,0)

print("Grille apres\n")
affichage(grille)

