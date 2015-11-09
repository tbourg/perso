from turtle import*
colormode(255)
r=0
v=0
b=24
speed(0)
ht()
width(1)
compteur=1
bgcolor("blue")

for rayon in range(150,50,-1):
	color(r,v,b)
	circle(rayon)
	left(37)
	if 1<=compteur<=32:
		r=r+7
	elif 33<=compteur<=64:
		v=v+7
	else:
		b=b+6
	compteur=compteur+1
