'''
def tri(tab):
	tabtrie=[]
	ind=-1
	for i in range(len(tab)):
		min=tab[0]
		for j in range(len(tab)):
			if min<tab[j] and j!=ind:
				min=tab[j]
				ind=j
		tabtrie[i]=min
	return tabtrie
'''

def select_tri(tab):
        min=tab[0]
        for j in range(len(tab)):
                if min>tab[j]:
                        min=tab[j]
                        ind=j
        x=tab[ind]
        tab[ind]=tab[0]
        tab[0]=x
        if len(tab)!=1:
                tab=tab[0]+select_tri(tab[1:])
        return tab

print(select_tri([6, 5, 4, 2, 1]))



