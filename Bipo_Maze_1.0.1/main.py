# -*- coding: Latin-1 -*-
#-------------------------------------------------------------------------------
# Name:        Bipo Maze
# Author:      Jimmy (WoofWoofDude)
# Created:     2012/04/06
# Python:      3.2.2 (default, Sep  4 2011, 09:51:08) [MSC v.1500 32 bit (Intel)]
#-------------------------------------------------------------------------------
#!/usr/bin/env python

#-----------------------Libs import-----------------------#
from pygame import init, display, Color, key, quit, joystick, time
from pygame.locals import *
from sys import exit
# FIN

#-----------------------Local import-----------------------#
from tygame.main import StaticFrame, Button, Label, render_widgets, handle_widgets #But you can put in ..\Python\Lib\site-packages
from Classe import *
import Fonction
# FIN

#---------------------Buttons fonctions--------------------#
    
def But_X_Y(size):
    
    global mylaby
    global perso

    mylaby = laby(size[0], size[1])
    mylaby.generate_laby()
    perso = Perso(mylaby)

def But_path():

    global perso

    perso.che_jaune = []
    perso.reverse = 0
    perso.astar(((perso.laby.w - 1), (perso.laby.h - 1)))
    chemain = perso.get_astar((perso.x, perso.y), ((perso.laby.w - 1), (perso.laby.h - 1)))
    perso.go_to(chemain)
# FIN


#---------------------Pygame init--------------------#
init()


#Création de la fenêtre
WW, WH = 640, 480
Window = display.set_mode((WW, WH))

#icone = image.load("Bipo.png")
#icone.set_colorkey(const.pink)
#display.set_icon(icone)
display.set_caption("Bipo Maze")
# FIN

'''
#---------------------Typgame--------------------#
#generateur
Frame = StaticFrame(Window, colour = const.Pblue, header = False, bordercolor = const.Pgreen, borderwidth = 5, width = 229, height = 470)
Frame.place((401, 0))

Label_gen = Label(Frame, width = 209, height = 270, htitle = " Générateur ", htitlefont = "Verdana", htitlesize = 14, htitlecolor = Color(const.black[0], const.black[1], const.black[2]), colour = Color(const.Pgreen[0], const.Pgreen[1], const.Pgreen[2]))
Label_gen.place((10, 10))

But_5_5 = Button(Label_gen, text = "5 x 5", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (5, 5))
But_5_5.place((5, 5))

But_5_10 = Button(Label_gen, text = "5 x 10", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (5, 10))
But_5_10.place((110, 5))

But_10_10 = Button(Label_gen, text = "10 x 10", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (10, 10))
But_10_10.place((5, 45))

But_10_15 = Button(Label_gen, text = "10 x 15", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (10, 15))
But_10_15.place((110, 45))

But_15_15 = Button(Label_gen, text = "15 x 15", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (15, 15))
But_15_15.place((5, 85))

But_15_20 = Button(Label_gen, text = "15 x 20", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (15, 20))
But_15_20.place((110, 85))

But_20_10 = Button(Label_gen, text = "20 x 10", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (20, 10))
But_20_10.place((5, 125))

But_20_20 = Button(Label_gen, text = "20 x 20", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (20, 20))
But_20_20.place((110, 125))

But_20_30 = Button(Label_gen, text = "20 x 30", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (20, 30))
But_20_30.place((5, 165))

But_25_15 = Button(Label_gen, text = "25 x 15", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (25, 15))
But_25_15.place((110, 165))

But_25_25 = Button(Label_gen, text = "25 x 25", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (25, 25))
But_25_25.place((5, 205))

But_25_30 = Button(Label_gen, text = "25 x 30", width = 80, height = 25, bordercolor = const.Porange, colour = const.Pgreen, target = But_X_Y, args = (25, 30))
But_25_30.place((110, 205))
# FIN generateur

# pathfinding
Label_path = Label(Frame, width = 209, height = 155, htitle = " Recherche ", htitlefont = "Verdana", htitlesize = 14, htitlecolor = Color(const.black[0], const.black[1], const.black[2]), colour = Color(const.Pgreen[0], const.Pgreen[1], const.Pgreen[2]))
Label_path.place((10, 300))

But_path = Button(Label_path, text = " Trouvez le chemin ? ", width = 185, height = 120, bordercolor = const.Porange, colour = const.yellow, fontsize = 16, target = But_path)
But_path.place((5, 5))
# FIN pathfinding
# FIN
'''
#---------------------Some variables--------------------#
mylaby = laby(5, 5)
mylaby.generate_laby()
perso = Perso(mylaby) 
    
perso_time = 0

liste_b1 = Fonction.liste_bip(WW, WH)
liste_b2 = Fonction.fill_liste_b2(liste_b1)

display.flip()
key.set_repeat(50, 55)
# FIN


if joystick.get_count() > 0:
    Joy = joystick.Joystick(0)
    Joy.init()


while True:
    time.Clock().tick(30)
    Window.fill(const.Porange)    

    for event in handle_widgets():
        if event.type == QUIT:
            quit()
            exit()
                    
        elif event.type == JOYAXISMOTION:
            if event.axis == 1:
                if round(event.value) < 0:
                    if not perso.che_jaune:
                        perso.move(const.up)
                if round(event.value) > 0:
                    if not perso.che_jaune:
                        perso.move(const.down)
            if event.axis == 0:
                if round(event.value) < 0:
                    if not perso.che_jaune:
                        perso.move(const.left)
                if round(event.value) > 0:
                    if not perso.che_jaune:
                        perso.move(const.right)
        
        elif event.type == JOYBUTTONDOWN:
            if event.button == 8:
            
                perso.che_jaune = []
                perso.reverse = 0
                perso.astar(((perso.laby.w - 1), (perso.laby.h - 1)))
                chemain = perso.get_astar((perso.x, perso.y), ((perso.laby.w - 1), (perso.laby.h - 1)))
                perso.go_to(chemain)
                
            if event.button == 0:
            
                mylaby = laby(5, 5)
                mylaby.generate_laby()
                perso = Perso(mylaby)
                
            if event.button == 1:
            
                mylaby = laby(10, 10)
                mylaby.generate_laby()
                perso = Perso(mylaby) 
 
            if event.button == 2:
            
                mylaby = laby(20, 20)
                mylaby.generate_laby()
                perso = Perso(mylaby) 
                
            if event.button == 3:
            
                mylaby = laby(25, 30)
                mylaby.generate_laby()
                perso = Perso(mylaby)  

    keys = key.get_pressed()
    if keys:
        if keys[K_UP]:
            if not perso.che_jaune:
                perso.move(const.up)
        if keys[K_DOWN]:
            if not perso.che_jaune:
                perso.move(const.down)
        if keys[K_LEFT]:
            if not perso.che_jaune:
                perso.move(const.left)
        if keys[K_RIGHT]:
            if not perso.che_jaune:
                perso.move(const.right)
                

    if time.get_ticks() - perso_time >= const.time_perso_poll:
        perso_time = time.get_ticks()
        perso.poll()


    perso.show(Window)
    render_widgets()
    display.flip()
    
    if perso.x == perso.laby.w - 1 and perso.y == perso.laby.h - 1:
        time.delay(300)       
        
        '''for Bip in liste_b2:
            Bip.show(Window)
            display.flip()'''                     
            
    
        mylaby = laby(5, 5)
        mylaby.generate_laby()
        perso = Perso(mylaby)
        
        '''while True:
            Window.fill(const.Porange)
            perso.show(Window)
            render_widgets()
            
            if not liste_b2: break
            
            i = 0
            while i < 24:
                liste_b2.remove(choice(liste_b2))  
                i += 1
           
            
            for Bip in liste_b2:
                Bip.show(Window)
            
            display.flip()
            
        liste_b2 = Fonction.fill_liste_b2(liste_b1)'''
