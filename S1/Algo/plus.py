def compare(num, aDeviner, nbEssais):
	if num < aDeviner:
		print("plus")
		return(0)
	elif num > aDeviner:
		print("moins")
		return(0)
	else:
		print("Il vous a falu", nbEssais, "coups")
		return(1)


aDeviner = int(input("Entrez un nombre entre 1 et 100\n"))
nbEssais = 1
num = 101
a = 0
while a != 1:
	num = int(input("Votre nombre\n"))
	a = compare(num, aDeviner, nbEssais)
	nbEssais = nbEssais+1

a = input()
