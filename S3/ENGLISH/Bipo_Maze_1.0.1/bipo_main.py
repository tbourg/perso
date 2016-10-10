# -*- coding: Latin-1 -*-
#-------------------------------------------------------------------------------
# Name:        Bipo Maze
# Author:      Jimmy (WoofWoofDude)
# Created:     2012/04/06
# Python:      3.2.2 (default, Sep  4 2011, 09:51:08) [MSC v.1500 32 bit (Intel)]
#-------------------------------------------------------------------------------
#!/usr/bin/env python

#-----------------------Libs import-----------------------#
from pygame import init, display, Color, key, quit, joystick, time, font
from pygame.locals import *
from sys import exit
# FIN

#-----------------------Local import-----------------------#
from tygame.main import StaticFrame, Button, Label, render_widgets, handle_widgets #But you can put in ..\Python\Lib\site-packages
from Classe import *

# FIN

def main(difficulty):
    #---------------------Pygame init--------------------#
    init()

    myfont = font.SysFont("monospace", 15)
    #Création de la fenêtre
    WW, WH = 640, 480
    Window = display.set_mode((WW, WH))

    #icone = image.load("Bipo.png")
    #icone.set_colorkey(const.pink)
    #display.set_icon(icone)
    display.set_caption("Bipo Maze")
    # FIN

    if difficulty == 1:
        taille = 10
    elif difficulty == 2:
        taille = 11
    elif difficulty == 3:
        taille = 12


    #---------------------Some variables--------------------#
    mylaby = laby(taille, taille)
    mylaby.generate_laby()
    perso = Perso(mylaby) 
        
    perso_time = 0
    display.flip()
    key.set_repeat(50, 55)
    # FIN


    while True:
        time.Clock().tick(30)
        Window.fill(const.Porange)

        time_play = time.get_ticks()/1000
        label = myfont.render(str(time_play), 1, (0,0,0))
        Window.blit(label, (200, 0))

        for event in handle_widgets():
            if event.type == QUIT:
                quit()
                exit()
            
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
                   

        perso.show(Window)
        render_widgets()
        display.flip()
        
        if perso.x == perso.laby.w - 1 and perso.y == perso.laby.h - 1:
            quit()
            exit()
