def affichedamier(x,y,c,d):
	a=0
	for l in range(y):
		for m in range(y):
			for i in range(x):
				for j in range(2):
					for k in range(x):
						if a==0:
							if i%2==0 and j%2==0:
								print(c)
							elif i%2==0 and j%2!=0:
								print(d)
							else:
								print(" ")
						else:
							if i%2!=0 and j%2!=0:
								print(c)
							elif i%2!=0 and j%2==0:
								print(d)
							else:
								print(" ")
		if a==0:
			a=1
		else:
			a=0



print("Bienvenue dans le dessinateur à damier")
x=int(input("Choisissez un nombre de colonnes "))
y=int(input("Le nombre de lignes "))
c=input("Premier caractere")
d=input("Second caractere")
affichedamier(x,y,c,d)
