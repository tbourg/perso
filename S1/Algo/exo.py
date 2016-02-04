def droite(xa,ya,xb,yb):
	coeff=(yb-ya)/(xb-xa)
	return coeff

def separ(duree):
	jour=int(duree/86400)
	heure=int((duree-86400*jour)/3600)
	min=int((duree-86400*jour-3600*heure)/60)
	sec=duree-86400*jour-3600*heure-60*min
	return(jour,heure,min,sec)

def add(j,h,m,s):
	temps=86400*j+3600*h+60*m+s
	return temps
	
