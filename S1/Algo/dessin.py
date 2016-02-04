# -*- coding:utf-8 -*-
from turtle import *
clear()
speed(0)
colormode(255)
r=13
v=13
b=13
compteurTour=1
rayon=200
ht()

def arc(rayon):
	left(60)
	for i in range(1,4):
		circle(rayon,120)
		left(120)
	right(60)
	
up()
goto(0,-200)
down()
circle(200)
while rayon>=0:	
	for a in range(0,36):
		if 0<=compteurTour<=120:
			r = r + 2
		elif 120<=compteurTour<=240:
			v = v + 2
		else:
			b = b + 2
		color(r,v,b)
		arc(rayon)
		up()
		circle(200,10)
		down()
		compteurTour = compteurTour + 1
		print(compteurTour)
	rayon=rayon-40
ht()
exitonclick()
