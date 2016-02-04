from random import randint

refaire = True
while refaire == True :
	print("Bonjour !\n" +
		"Saisir le numéro correspondant au jeu désiré\n" +
		"1 pour le juste prix\n" +
		"2 pour le c'est plus c'est moins")
	numeroJeu = input()
	if numeroJeu == "1" :
		aDeviner = randint(0, 100)
		num = "101"
		while num != aDeviner :
			num = input("Votre choix")
			num = eval(num)
			if num < aDeviner :
				print("C'est plus")
			elif num > aDeviner :
				print("C'est moins")
			else :
				print("Bravo")
	else :
		print("Non disponible")
	refaire = input("Voulez-vous rejouer?")