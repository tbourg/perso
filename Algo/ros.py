#from turtle import*
from tkinter import*
#colormode(255)

def ros(x0,y0,x1,y1,r,v,b):
        zone.create_arc(x0,y0,x1,y1)
        
fenetre=Tk()
x0=0
y0=0
x1=1
y1=1
zone=Canvas(fenetre,width=1000,height=1000)
for r in range(10,100):
    for v in range(10,100):
        for b in range(10,100):
                ros(x0,y0,x1,y1,r,v,b)
                x0=x1
                y0=y1
                x1=x1*1.2
                y1=y1*1.2
                zone.pack()
fenetre.mainloop()

