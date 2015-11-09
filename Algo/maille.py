from turtle import*
from math import*

M=int(input())
N=int(input())
Pi=3.14159265359
compteur=1
listex=[]
listey=[]

for m in range(0,M+1):
	for n in range(0,N+1):
		(x, y, z) = (sin(Pi * m/M)*cos(2*Pi * n/N), sin(Pi * m/M)*sin(2*Pi * n/N), cos(Pi * m/M))
		x=sin(Pi*m/M)*cos(2*Pi*n/N)
		y=sin(Pi*m/M)*sin(2*Pi*n/N)
		z=cos(Pi*m/M)
		listex.append(10*x)
		listey.append(10*y)
		print("x=",x,"y=",y,"z=",z)
		print(x,y,z)
		compteur=compteur+1

ht()
up()
goto(listex[0],listey[0])
down()
print(compteur)
for i in range(1,compteur+1):
	goto(listex[i],listey[i])
	print(i)
